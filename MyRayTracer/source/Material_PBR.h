#pragma once
#include "BRDF.h"

class Material_PBR : public BRDF
{
public:
	Material_PBR(const Elite::RGBColor& color, float metalness, float roughness);
	virtual ~Material_PBR() = default;

	Material_PBR(const Material_PBR& other) = delete;
	Material_PBR& operator=(const Material_PBR& other) = delete;
	Material_PBR(Material_PBR&& other) = delete;
	Material_PBR& operator=(Material_PBR&& other) = delete;

	virtual Elite::RGBColor Shade(const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v) const override;

private:
	Elite::RGBColor Fresnel(const Elite::FVector3& l, const Elite::FVector3& v, const Elite::FVector3& halfVector, const Elite::RGBColor& f0) const;
	float NormalDistribution(const Elite::FVector3& l, const Elite::FVector3& v, const Elite::FVector3& n, const Elite::FVector3& halfVector, float roughness) const;
	float Geometry_Smith(const Elite::FVector3& l, const Elite::FVector3& v, const Elite::FVector3& n, float roughness) const;
	float Geometry_SchlickGGX(const Elite::FVector3& v, const Elite::FVector3& n, float roughness) const;

	Elite::RGBColor CookTorrance(const Elite::FVector3& l, const Elite::FVector3& v, const Elite::FVector3& n,
									const Elite::RGBColor& fresnel, float normal, float geometry) const;

	float m_Metalness;
	float m_Roughness;
};

