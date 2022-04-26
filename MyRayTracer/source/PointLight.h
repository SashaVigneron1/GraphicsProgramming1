#pragma once

#include "Light.h"

class PointLight final : public Light
{
public:
	PointLight(const Elite::FPoint3& position, const Elite::RGBColor& color, float intensity);
	~PointLight() = default;

	PointLight(const PointLight& other) = delete;
	PointLight& operator=(const PointLight& other) = delete;
	PointLight(PointLight&& other) = delete;
	PointLight& operator=(PointLight&& other) = delete;

	virtual Elite::RGBColor GetERGB(const Elite::FPoint3& pointToShade) const override;
	virtual Elite::FVector3 GetDirection(const Elite::FPoint3& pointToShade) const override;
	virtual float GetDistance(const Elite::FPoint3& pointToShade) const override;

private:
	
};

