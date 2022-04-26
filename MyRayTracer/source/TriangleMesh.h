#pragma once

#include <string>
#include <vector>
#include "EMath.h"

#include "Triangle.h"

class BRDF;

class TriangleMesh final : public Geometry
{
public:
	TriangleMesh(const std::string& objFilePath, const Elite::FVector3& position, BRDF* pMaterial);
	~TriangleMesh() = default;

	TriangleMesh(const TriangleMesh& other) = delete;
	TriangleMesh& operator=(const TriangleMesh& other) = delete;
	TriangleMesh(TriangleMesh&& other) = delete;
	TriangleMesh& operator=(TriangleMesh&& other) = delete;

	virtual bool IsHit(const Ray& ray, HitRecord& hitPoint);
	virtual void Update(float deltaT);
	virtual FVector3 GetNormal(const Elite::FPoint3& point);


private:
	Elite::FPoint3 m_Position;

	int m_HitTriangleIndex;

	Triangle m_Triangle;

	std::vector<IVector3> m_IndexBuffer;
	std::vector<FVector3> m_VertexBuffer;

	float m_Angle;

	void Parse(const std::string& objFilePath);
};

