#pragma once
#include "EMath.h"
#include "Light.h"
#include <vector>
#include "SDL.h"

class LightManager final
{
public:

	enum class LightMode 
	{
		IrradianceOnly = 0,
		BRDFOnly = 1,
		All = 2
	};

	LightManager();
	~LightManager();

	LightManager(const LightManager& other) = delete;
	LightManager& operator=(const LightManager& other) = delete;
	LightManager(LightManager&& other) = delete;
	LightManager& operator=(LightManager&& other) = delete;

	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e);

	void AddLight(Light* pLight);
	std::vector<Light*> GetActiveLights() const;

	LightMode GetLightMode() const;

	bool IsUsingHardShadows() const;
private:
	std::vector<Light*> m_pLights;
	bool m_IsUsingHardShadows;


	LightMode m_LightMode;
};

