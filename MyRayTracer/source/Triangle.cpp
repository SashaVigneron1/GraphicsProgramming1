#include "Triangle.h"

#include <iostream>

Triangle::Triangle(const FPoint3& position, CullMode cullmode, BRDF* pMaterial, bool calculateAngle, const std::vector<FVector3>& triangleVertices)
	: Geometry{ pMaterial }
	, m_v0_Offset{ (triangleVertices.size() > 0) ? triangleVertices[0] : FVector3{-.75f, 1.5f, 0.f} }
	, m_v1_Offset{ (triangleVertices.size() > 1) ? triangleVertices[1] : FVector3{-.75f, 0.f, 0.f} }
	, m_v2_Offset{ (triangleVertices.size() > 2) ? triangleVertices[2] : FVector3{.75f, 0.f, 0.f} }
	, m_CullMode{ cullmode }
	, m_Center{ position }
	, m_CalculateAngle{ calculateAngle }
	, m_Angle{ 0.f }
{
}

bool Triangle::IsHit(const Ray& ray, HitRecord& hitPoint)
{
	// Initialization
	auto rotMatrix{ Elite::MakeRotationY(m_Angle) };
	Elite::FVector3 v0{ m_Center + rotMatrix * m_v0_Offset };
	Elite::FVector3 v1{ m_Center + rotMatrix * m_v1_Offset };
	Elite::FVector3 v2{ m_Center + rotMatrix * m_v2_Offset };

	FVector3 a = v1 - v0;
	FVector3 b = v2 - v0;
	FVector3 normal = Elite::GetNormalized(Elite::Cross(a, b));

	// Culling
	if (m_CullMode == Triangle::CullMode::BackFace && (Elite::Dot(normal, ray.direction) > 0)) return false;
	if (m_CullMode == Triangle::CullMode::FrontFace && (Elite::Dot(normal, ray.direction) < 0)) return false;
	if (Elite::Dot(normal, ray.direction) == 0) return false; // ray.direction = v

	FVector3 L = v0 - FVector3{ ray.origin };
	float t{ Elite::Dot(L, normal) / Elite::Dot(ray.direction, normal) };

	if (t < Ray::tMin || t > Ray::tMax) 
		return false;

	Elite::FPoint3 point{ ray.origin + t * ray.direction };

	if (IsPointInTriangle(point, normal)) 
	{
		hitPoint.hitPoint = point;
		hitPoint.normal = normal;
		hitPoint.pMaterial = m_pMaterial;
		hitPoint.tValue = t;
		return true;
	}
	return false;
}

void Triangle::Update(float totalElapsedTime)
{
	if (m_CalculateAngle) m_Angle = (std::cosf(totalElapsedTime) + 1) * (float)E_PI;
}

bool Triangle::IsPointInTriangle(const Elite::FPoint3& point, const FVector3& normal) const
{
	auto rotMatrix{ Elite::MakeRotationY(m_Angle) };
	Elite::FVector3 v0{ m_Center + rotMatrix * m_v0_Offset };
	Elite::FVector3 v1{ m_Center + rotMatrix * m_v1_Offset };
	Elite::FVector3 v2{ m_Center + rotMatrix * m_v2_Offset };

	Elite::FVector3 u = Elite::Cross(FVector3{ point } - v1, v2 - v1);
	Elite::FVector3 v = Elite::Cross(FVector3{ point } - v2, v0 - v2);
	Elite::FVector3 w = Elite::Cross(FVector3{ point } - v0, v1 - v0);
	if (Elite::Dot(normal, u) > 0.0f) { return false; }
	if (Elite::Dot(normal, v) > 0.0f) { return false; }
	if (Elite::Dot(normal, w) > 0.0f) { return false; }
	return true;
}

FVector3 Triangle::GetNormal(const Elite::FPoint3& point)
{
	auto rotMatrix{ Elite::MakeRotationY(m_Angle) };
	Elite::FVector3 v0{ m_Center + rotMatrix * m_v0_Offset };
	Elite::FVector3 v1{ m_Center + rotMatrix * m_v1_Offset };
	Elite::FVector3 v2{ m_Center + rotMatrix * m_v2_Offset };

	FVector3 a = v1 - v0;
	FVector3 b = v2 - v0;
	return Elite::Cross(a, b);
}

void Triangle::SetPositionOffset(const std::vector<FVector3>& offset)
{
	m_v0_Offset = offset[0];
	m_v1_Offset = offset[1];
	m_v2_Offset = offset[2];
}

void Triangle::SetAngle(float angle)
{
	m_Angle = angle;
}

void Triangle::PrintVertices() const
{
	std::cout << "Point 1: (" << m_v0_Offset.x << ", " << m_v0_Offset.y << ", " << m_v0_Offset.z << ")\n";
	std::cout << "Point 2: (" << m_v1_Offset.x << ", " << m_v1_Offset.y << ", " << m_v1_Offset.z << ")\n";
	std::cout << "Point 3: (" << m_v2_Offset.x << ", " << m_v2_Offset.y << ", " << m_v2_Offset.z << ")\n\n\n";
}
