#include "Material_Lambert.h"

Material_Lambert::Material_Lambert(const Elite::RGBColor& color, float diffuseReflectance)
	: BRDF{color}
	, m_DiffuseReflectance{ diffuseReflectance }
{
}

Elite::RGBColor Material_Lambert::Shade(const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v) const
{
	return BRDF::Lambert(m_DiffuseReflectance);
}
