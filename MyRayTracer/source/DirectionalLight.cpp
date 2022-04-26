#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const Elite::FVector3& direction, const Elite::RGBColor& color, float intensity)
	: Light{ Elite::FPoint3{0, 0, 0}, color, intensity }
	, m_Direction{ direction }
{
}

Elite::RGBColor DirectionalLight::GetERGB(const Elite::FPoint3& pointToShade) const
{
	return m_Color * m_Intensity;
}

Elite::FVector3 DirectionalLight::GetDirection(const Elite::FPoint3& pointToShade) const
{
	return Elite::GetNormalized(m_Direction);
}

float DirectionalLight::GetDistance(const Elite::FPoint3& pointToShade) const
{
	return FLT_MAX;
}
