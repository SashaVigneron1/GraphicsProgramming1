#include "Plane.h"
#include "EMath.h"


Plane::Plane(const FPoint3& pos, const FVector3& normal, BRDF* pMaterial)
	: Geometry{ pMaterial }
	, m_Position{ pos }
	, m_Normal{ normal }
{
}

bool Plane::IsHit(const Ray& ray, HitRecord& hitPoint)
{
	float t{ Elite::Dot(m_Position - ray.origin, m_Normal) / Elite::Dot(ray.direction, m_Normal) };

	if (t > ray.tMin && t < ray.tMax)
	{
		hitPoint.hitPoint = ray.origin + t * ray.direction;
		hitPoint.tValue = t;
		hitPoint.pMaterial = m_pMaterial;
		hitPoint.normal = GetNormal(hitPoint.hitPoint);

		return true;
	}
	hitPoint.tValue = FLT_MAX;
	return false;
}

void Plane::Update(float deltaT)
{
}

FVector3 Plane::GetNormal(const Elite::FPoint3& point)
{
	return Elite::GetNormalized(m_Normal);
}
