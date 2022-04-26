#pragma once

#include "EMath.h"
#include "ERGBColor.h"

class Light
{
public:
	Light(const Elite::FPoint3& position, const Elite::RGBColor& color, float intensity);
	virtual ~Light() = default;

	Light(const Light& other) = delete;
	Light& operator=(const Light& other) = delete;
	Light(Light&& other) = delete;
	Light& operator=(Light&& other) = delete;

	virtual Elite::RGBColor GetERGB(const Elite::FPoint3 & pointToShade) const = 0;
	virtual Elite::FVector3 GetDirection(const Elite::FPoint3& pointToShade) const = 0;
	virtual float GetDistance(const Elite::FPoint3& pointToShade) const = 0;

	Elite::FPoint3 GetPosition() const;

	bool IsActive() const;
	void SetActive(bool value);

protected:
	Elite::RGBColor m_Color;
	float m_Intensity;
	Elite::FPoint3 m_Position;

	bool m_IsActive;
};

