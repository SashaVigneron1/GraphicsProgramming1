#pragma once
#include "Geometry.h"

class Sphere final : public Geometry
{
public:
	Sphere(const FPoint3& pos, const float radius, BRDF* pMaterial);
	~Sphere() = default;

	Sphere(const Sphere& other) = delete;
	Sphere& operator=(const Sphere& other) = delete;
	Sphere(Sphere&& other) = delete;
	Sphere& operator=(Sphere&& other) = delete;

	virtual bool IsHit(const Ray& ray, HitRecord& hitPoint) override;
	virtual void Update(float deltaT) override;

	virtual FVector3 GetNormal(const Elite::FPoint3& point) override;

protected:

	FPoint3 m_Position;
	float m_Radius;

};

