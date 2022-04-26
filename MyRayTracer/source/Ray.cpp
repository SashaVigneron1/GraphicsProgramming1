#include "Ray.h"
#include "Geometry.h"


const float Ray::tMin = 0.1f;
const float Ray::tMax = FLT_MAX;

Ray::Ray(const FPoint3& orig, const FVector3& dir)
	: origin{ orig }
	, direction { dir }
{

}


HitRecord::HitRecord()
	: hitPoint{ FPoint3{0.f,0.f,0.f} }
	, pMaterial{ nullptr }
	, tValue{ FLT_MAX }
	, normal{}
{

}

HitRecord& HitRecord::operator=(const HitRecord& other)
{
	hitPoint = other.hitPoint;
	pMaterial = other.pMaterial;
	tValue = other.tValue;
	normal = Elite::GetNormalized(other.normal);
	return *this;
}


