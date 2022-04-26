#include "LightManager.h"
#include <iostream>

//// Lights
#include "PointLight.h"
#include "DirectionalLight.h"


LightManager::LightManager()
	: m_IsUsingHardShadows{true}
	, m_LightMode{LightMode::All}
{
	Elite::RGBColor lightColor{ 1.0f,1.0f,1.0f };

	AddLight(new PointLight{ Elite::FPoint3{0,5.f,-5.f}, Elite::RGBColor{1.f, .61f, .45f}, 50.f });
	AddLight(new PointLight{ Elite::FPoint3{-2.5f,5.f,5.f}, Elite::RGBColor{1.f, .8f, .45f}, 70.f });
	AddLight(new PointLight{ Elite::FPoint3{2.5f,2.5f,5.f}, Elite::RGBColor{.34f, .47f, .68f}, 50.f });
	//AddLight(new DirectionalLight{ Elite::FVector3{0,-1,0}, lightColor, 1.f });
	//m_pLights[2]->SetActive(!m_pLights[2]->IsActive());
}

LightManager::~LightManager()
{
	for (Light* pLight : m_pLights)
	{
		delete pLight;
	}
}

void LightManager::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	switch (e.keysym.sym)
	{
	case SDLK_k:
		m_pLights[0]->SetActive(!m_pLights[0]->IsActive());
		break;
	case SDLK_l:
		m_pLights[1]->SetActive(!m_pLights[1]->IsActive());
		break;
	case SDLK_j:
		m_pLights[2]->SetActive(!m_pLights[2]->IsActive());
		break;
	case SDLK_q:
		m_IsUsingHardShadows = !m_IsUsingHardShadows;
		break;
	case SDLK_e:
		m_LightMode = static_cast<LightMode>((static_cast<int>(m_LightMode) + 1) % 3);

		switch (m_LightMode)
		{
		case LightManager::LightMode::IrradianceOnly:
			std::cout << "LightMode set to: IRRADIANCE ONLY\n";
			break;
		case LightManager::LightMode::BRDFOnly:
			std::cout << "LightMode set to: BRDF ONLY\n";
			break;
		case LightManager::LightMode::All:
			std::cout << "LightMode set to: ALL\n";
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void LightManager::AddLight(Light* pLight)
{
	m_pLights.push_back(pLight);
}

std::vector<Light*> LightManager::GetActiveLights() const
{
	std::vector<Light*> activeLights;

	for (size_t i = 0; i < m_pLights.size(); i++)
	{
		if (m_pLights[i]->IsActive())
		{
			activeLights.push_back(m_pLights[i]);
		}
	}

	return activeLights;
}

LightManager::LightMode LightManager::GetLightMode() const
{
	return m_LightMode;
}

bool LightManager::IsUsingHardShadows() const
{
	return m_IsUsingHardShadows;
}
