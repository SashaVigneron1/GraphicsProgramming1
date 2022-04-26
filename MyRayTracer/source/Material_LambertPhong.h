#pragma once
#include "BRDF.h"

class Material_LambertPhong : public BRDF
{
public:
	Material_LambertPhong(const Elite::RGBColor& color, float diffuseReflectance, float specularReflectance, float phongExponent);
	virtual ~Material_LambertPhong() = default;

	Material_LambertPhong(const Material_LambertPhong& other) = delete;
	Material_LambertPhong& operator=(const Material_LambertPhong& other) = delete;
	Material_LambertPhong(Material_LambertPhong&& other) = delete;
	Material_LambertPhong& operator=(Material_LambertPhong&& other) = delete;

	virtual Elite::RGBColor Shade(const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v) const override;

private:
	float m_DiffuseReflectance;
	float m_SpecularReflectance;
	float m_PhongExponent;
};

