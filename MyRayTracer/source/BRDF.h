#pragma once
#include "ERGBColor.h"
#include "EMath.h"

struct HitRecord;

class BRDF
{
public:
	BRDF(const Elite::RGBColor& color);
	virtual ~BRDF() = default;

	BRDF(const BRDF& other) = delete;
	BRDF& operator=(const BRDF& other) = delete;
	BRDF(BRDF&& other) = delete;
	BRDF& operator=(BRDF&& other) = delete;

	virtual Elite::RGBColor Shade(const HitRecord& hitRecord, const Elite::FVector3& llightDirection, const Elite::FVector3& v) const = 0;
	Elite::RGBColor GetColor() const;

protected:
	Elite::RGBColor m_AlbedoColor;

	Elite::RGBColor Lambert(float diffuseReflectance) const;
	Elite::RGBColor Lambert(float diffuseReflectance, const Elite::RGBColor& albedo) const;
	Elite::RGBColor Phong(float specularReflectance, float phongExponent, const Elite::FVector3& lightDirection, const Elite::FVector3& v, const Elite::FVector3& normal) const;
};

