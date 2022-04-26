#pragma once
#include "EMath.h"
#include "Ray.h"

using namespace Elite;


class PerspectiveCamera final
{
public:
	PerspectiveCamera(const FVector3& position, float fov, float screenWidth, float screenHeight);
	~PerspectiveCamera() = default;

	PerspectiveCamera(const PerspectiveCamera&) = delete;
	PerspectiveCamera(PerspectiveCamera&&) noexcept = delete;
	PerspectiveCamera& operator=(const PerspectiveCamera&) = delete;
	PerspectiveCamera& operator=(PerspectiveCamera&&) noexcept = delete;

	void Update(float elapsedSec);

	void GenerateRay(Ray& ray, uint32_t width, uint32_t height);

private:
	FMatrix4 m_LookAtMatrix;
	float m_AspectRatio;
	float m_Fov;

	float m_MovementSpeed;
	float m_RotateSpeed;

	float m_ScreenWidth;
	float m_ScreenHeight;

};

