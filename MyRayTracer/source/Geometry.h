#pragma once

#include "ERGBColor.h"
#include "Ray.h"
#include <tuple>
#include "BRDF.h"

using namespace Elite;

class Geometry 
{
public:
	Geometry(BRDF* pMaterial);
	~Geometry();

	Geometry(const Geometry& other) = delete;
	Geometry& operator=(const Geometry& other) = delete;
	Geometry(Geometry&& other) = delete;
	Geometry& operator=(Geometry&& other) = delete;

	virtual bool IsHit(const Ray& ray, HitRecord& hitPoint) = 0;
	virtual void Update(float deltaT) = 0;
	virtual FVector3 GetNormal(const Elite::FPoint3& point) = 0;
	BRDF* GetMaterial() const;

protected:

	BRDF* m_pMaterial;

private:

};

