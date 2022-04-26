#pragma once
#include "BRDF.h"

class Material_Lambert : public BRDF
{
public:
	Material_Lambert(const Elite::RGBColor& color, float diffuseReflectance);
	virtual ~Material_Lambert() = default;

	Material_Lambert(const Material_Lambert& other) = delete;
	Material_Lambert& operator=(const Material_Lambert& other) = delete;
	Material_Lambert(Material_Lambert&& other) = delete;
	Material_Lambert& operator=(Material_Lambert&& other) = delete;

	virtual Elite::RGBColor Shade(const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v) const override;

private:
	float m_DiffuseReflectance;
};

