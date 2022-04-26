#include "Light.h"

Light::Light(const Elite::FPoint3& position, const Elite::RGBColor& color, float intensity)
	: m_Color{ color }
	, m_Intensity{ intensity }
	, m_Position{ position }
	, m_IsActive{ true }
{
}

Elite::FPoint3 Light::GetPosition() const
{
	return m_Position;
}

bool Light::IsActive() const
{
	return m_IsActive;
}

void Light::SetActive(bool value)
{
	m_IsActive = value;
}
