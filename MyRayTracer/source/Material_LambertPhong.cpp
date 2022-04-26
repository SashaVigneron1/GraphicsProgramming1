#include "Material_LambertPhong.h"
#include "Ray.h"

Material_LambertPhong::Material_LambertPhong(const Elite::RGBColor& color, float diffuseReflectance, float specularReflectance, float phongExponent)
	: BRDF{ color }
	, m_DiffuseReflectance{ diffuseReflectance }
	, m_SpecularReflectance{ specularReflectance }
	, m_PhongExponent{ phongExponent }
{
}

Elite::RGBColor Material_LambertPhong::Shade(const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v) const
{
	return BRDF::Lambert(m_DiffuseReflectance) 
		+ BRDF::Phong(m_SpecularReflectance, m_PhongExponent, l, v, hitRecord.normal);
}
