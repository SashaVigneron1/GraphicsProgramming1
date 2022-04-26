#include "Sphere.h"
#include "EMath.h"

Sphere::Sphere(const FPoint3& pos, const float radius, BRDF* pMaterial)
	: Geometry{ pMaterial }
	, m_Position{ pos }
	, m_Radius{ radius }
{
}

bool Sphere::IsHit(const Ray& ray, HitRecord& hitPoint)
{
	FVector3 L = m_Position - ray.origin;
	float tca = Elite::Dot(L, ray.direction);
	FVector3 length = Reject(L, ray.direction);
	float odSquared = length.x * length.x + length.y * length.y + length.z * length.z;

	hitPoint.tValue = FLT_MAX;
	if (odSquared > m_Radius * m_Radius) return false;

	float thc = sqrtf(m_Radius * m_Radius - odSquared);
	float t0 = tca - thc;
	float t1 = tca + thc;

	if (std::abs(t0) < t1 && t0 > ray.tMin && t0 < ray.tMax) 
	{
		hitPoint.hitPoint = ray.origin + t0 * ray.direction;
		hitPoint.tValue = t0;
		hitPoint.pMaterial = m_pMaterial;
		hitPoint.normal = GetNormal(hitPoint.hitPoint);
		return true;
	}
	else if (t1 > ray.tMin && t1 < ray.tMax) 
	{
		hitPoint.hitPoint = ray.origin + t1 * ray.direction;
		hitPoint.tValue = t1;
		hitPoint.pMaterial = m_pMaterial;
		hitPoint.normal = GetNormal(hitPoint.hitPoint);
		return true;
	}
	return false;
}

void Sphere::Update(float deltaT)
{
}

FVector3 Sphere::GetNormal(const Elite::FPoint3& point)
{
	return Elite::GetNormalized(FVector3{ point - m_Position });
}
