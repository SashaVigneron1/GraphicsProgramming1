#pragma once
#include "EMath.h"

// Materials
#include "BRDF.h"
#include "Material_Lambert.h"

using namespace Elite;

//Fwd Declarations
class Geometry;

struct Ray
{
	Ray(const FPoint3& origin, const FVector3& direction);
	~Ray() = default;

	Ray(const Ray& other) = delete;
	Ray& operator=(const Ray& other) = delete;
	Ray(Ray&& other) = delete;
	Ray& operator=(Ray&& other) = delete;

	

	FPoint3 origin;
	FVector3 direction;
	static const float tMin;
	static const float tMax;
};

struct HitRecord 
{
	HitRecord();
	~HitRecord() = default;

	HitRecord(const HitRecord& other) = delete;
	HitRecord& operator=(const HitRecord& other);
	HitRecord(HitRecord&& other) = delete;
	HitRecord& operator=(HitRecord&& other) = delete;

	FPoint3 hitPoint;
	FVector3 normal;
	BRDF* pMaterial;
	float tValue;
};