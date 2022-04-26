#include "BRDF.h"
#include "Ray.h"

BRDF::BRDF(const Elite::RGBColor& color)
	: m_AlbedoColor{color}
{
}

Elite::RGBColor BRDF::GetColor() const
{
	return m_AlbedoColor;
}

Elite::RGBColor BRDF::Lambert(float diffuseReflectance) const
{
	return m_AlbedoColor * diffuseReflectance / (float)E_PI;
}

Elite::RGBColor BRDF::Lambert(float diffuseReflectance, const Elite::RGBColor& albedo) const
{
	return  albedo * diffuseReflectance / (float)E_PI;
}

Elite::RGBColor BRDF::Phong(float ks, float exp, const Elite::FVector3& lightDirection, const Elite::FVector3& v, const Elite::FVector3& n) const
{
	Elite::FVector3 l{ Elite::GetNormalized(lightDirection) };

	auto reflect = l - (2 * Elite::Dot(n, l) * n);
	Elite::FVector3 r{ reflect };
	float cos = Elite::Dot(r, Elite::GetNormalized(v));

	float result = ks * std::powf(cos, exp);

	return RGBColor{ 1,1,1 } * result;
}
