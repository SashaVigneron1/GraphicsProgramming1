#include "Material_PBR.h"
#include "Ray.h"

Material_PBR::Material_PBR(const Elite::RGBColor& color, float metalness, float roughness)
	: BRDF{ color }
	, m_Metalness{ metalness }
	, m_Roughness{ roughness }
{
	if (m_Roughness <= 0) m_Roughness = 0.01f;
}

Elite::RGBColor Material_PBR::Shade(const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v) const
{
	// f0 for fresnel
	Elite::RGBColor f0 = (m_Metalness < 0.01f) ? Elite::RGBColor{ 0.04f ,0.04f ,0.04f } : m_AlbedoColor;

	// Halfvector
	Elite::FVector3 halfVector{ (v + l) / (Elite::Magnitude(v + l)) };

	// Specular
	Elite::RGBColor fresnel{ Fresnel(l,v,halfVector,f0) };
	float normalDistribution{ NormalDistribution(l,v, hitRecord.normal, halfVector, m_Roughness)};
	float geometry{ Geometry_Smith(l,v,hitRecord.normal, m_Roughness) };

	Elite::RGBColor specular{CookTorrance(l,v,hitRecord.normal,fresnel, normalDistribution, geometry) };
	
	// Diffuse
	float kd{};
	if (m_Metalness) kd = 0;
	else kd = 1 - fresnel.g;

	Elite::RGBColor diffuse{ BRDF::Lambert(kd, m_AlbedoColor) };

	// Result
	return diffuse + specular;
	
}

Elite::RGBColor Material_PBR::Fresnel(const Elite::FVector3& l, const Elite::FVector3& v, const Elite::FVector3& halfVector, const Elite::RGBColor& f0) const
{
	float dotProduct{ Elite::Dot(halfVector,v) };
	//if (dotProduct < 0) return f0;
	return Elite::RGBColor{ f0 + (Elite::RGBColor{1.f ,1.f ,1.f}) * (powf((1.f - (dotProduct)),5)) };
}

float Material_PBR::NormalDistribution(const Elite::FVector3& l, const Elite::FVector3& v, const Elite::FVector3& n, const Elite::FVector3& halfVector, float roughness) const
{
	float a = roughness * roughness;
	float a2 = a * a;

	float dotProduct{ Elite::Dot(n, halfVector) };
	//if (dotProduct < 0) return 0.f;
	float result = a2 / ((float)E_PI * (powf(dotProduct * dotProduct
											 * (a2 - 1.f) + 1.f,
										2.f)));
	return result;
}

float Material_PBR::Geometry_SchlickGGX(const Elite::FVector3& v, const Elite::FVector3& n, float roughness) const
{
	float a = roughness * roughness;
	float k = (a + 1.f) * (a + 1.f) / 8;
	float dotProduct{ Elite::Dot(n, v) };

	float g = Elite::Dot(n, v) / (Elite::Dot(n, v) * (1.f - k) + k);
	return g;
}

float Material_PBR::Geometry_Smith(const Elite::FVector3& l, const Elite::FVector3& v, const Elite::FVector3& n, float roughness) const
{
	return Geometry_SchlickGGX(n, v, roughness) * Geometry_SchlickGGX(n, l, roughness);
}


Elite::RGBColor Material_PBR::CookTorrance(const Elite::FVector3& l, const Elite::FVector3& v, const Elite::FVector3& n, const Elite::RGBColor& fresnel, float normal, float geometry) const
{
	Elite::RGBColor specular{};

	specular = fresnel * normal * geometry / (4 * (Elite::Dot(v, n) * Elite::Dot(l, n)));

	return specular;
}
