#pragma once

#include "Light.h"

class DirectionalLight final : public Light
{
public:
	DirectionalLight(const Elite::FVector3& direction, const Elite::RGBColor& color, float intensity);
	~DirectionalLight() = default;

	DirectionalLight(const DirectionalLight& other) = delete;
	DirectionalLight& operator=(const DirectionalLight& other) = delete;
	DirectionalLight(DirectionalLight&& other) = delete;
	DirectionalLight& operator=(DirectionalLight&& other) = delete;

	virtual Elite::RGBColor GetERGB(const Elite::FPoint3& pointToShade = Elite::FPoint3{0,0,0}) const override;
	virtual Elite::FVector3 GetDirection(const Elite::FPoint3& pointToShade) const override;
	virtual float GetDistance(const Elite::FPoint3& pointToShade) const override;


private:
	Elite::FVector3 m_Direction;
};

