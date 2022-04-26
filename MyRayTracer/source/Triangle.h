#pragma once
#include "Geometry.h"

#include <vector>

class Triangle final : public Geometry
{
public:
	enum class CullMode {
		BackFace = 0,
		FrontFace = 1,
		NoCulling = 2
	};

	Triangle(const FPoint3& position, CullMode cullmode, BRDF* pMaterial, bool calculateAngle = true, const std::vector<FVector3>& triangleVertices = std::vector<FVector3>());
	~Triangle() = default;

	Triangle(const Triangle& other) = delete;
	Triangle& operator=(const Triangle& other) = delete;
	Triangle(Triangle&& other) = delete;
	Triangle& operator=(Triangle&& other) = delete;

	virtual bool IsHit(const Ray & ray, HitRecord & hitPoint) override;
	virtual void Update(float totalElapsedTime) override;
	bool IsPointInTriangle(const Elite::FPoint3& point, const FVector3& normal) const;
	virtual FVector3 GetNormal(const Elite::FPoint3& point) override;

	void SetPositionOffset(const std::vector<FVector3>& offset);
	void SetAngle(float angle);

	void PrintVertices() const;

protected:

	CullMode m_CullMode;

	float m_Angle;
	bool m_CalculateAngle;

	FVector3 m_Center;

	FVector3 m_v0_Offset;
	FVector3 m_v1_Offset;
	FVector3 m_v2_Offset;
};

