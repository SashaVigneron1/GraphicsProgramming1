#include "PointLight.h"

#include <iostream>

PointLight::PointLight(const Elite::FPoint3& position, const Elite::RGBColor& color, float intensity)
	: Light{position, color, intensity}
{
}

Elite::RGBColor PointLight::GetERGB(const Elite::FPoint3& pointToShade) const
{
	// Dot product of itself = magnitude²
	return m_Color * (m_Intensity / Elite::Dot(m_Position - pointToShade, m_Position - pointToShade));
}

Elite::FVector3 PointLight::GetDirection(const Elite::FPoint3& pointToShade) const
{
	return Elite::GetNormalized(m_Position - pointToShade);
}

float PointLight::GetDistance(const Elite::FPoint3& pointToShade) const
{
	return Elite::Distance(m_Position, pointToShade);
}
