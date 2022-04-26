#pragma once
#include "Geometry.h"

class Plane final : public Geometry
{
public:
	Plane(const FPoint3& pos, const FVector3& normal, BRDF* pMaterial);
	~Plane() = default;

	Plane(const Plane& other) = delete;
	Plane& operator=(const Plane& other) = delete;
	Plane(Plane&& other) = delete;
	Plane& operator=(Plane&& other) = delete;

	virtual bool IsHit(const Ray& ray, HitRecord& hitPoint) override;
	virtual void Update(float deltaT) override;

	virtual FVector3 GetNormal(const Elite::FPoint3& point) override;
protected:

	FPoint3 m_Position;
	FVector3 m_Normal;
	
};

