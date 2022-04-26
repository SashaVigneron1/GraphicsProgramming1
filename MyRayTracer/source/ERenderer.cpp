//External includes
#include "SDL.h"
#include "SDL_surface.h"

//Project includes
#include "ERenderer.h"
#include "ERGBColor.h"

// My Includes

#include "Scene.h"
#include "Geometry.h"
#include "Ray.h"
#include "PerspectiveCamera.h"
#include "LightManager.h"



Elite::Renderer::Renderer(SDL_Window * pWindow)
{
	//Initialize
	m_pWindow = pWindow;
	m_pFrontBuffer = SDL_GetWindowSurface(pWindow);
	int width, height = 0;
	SDL_GetWindowSize(pWindow, &width, &height);
	m_Width = static_cast<uint32_t>(width);
	m_Height = static_cast<uint32_t>(height);
	m_pBackBuffer = SDL_CreateRGBSurface(0, m_Width, m_Height, 32, 0, 0, 0, 0);
	m_pBackBufferPixels = (uint32_t*)m_pBackBuffer->pixels;
}

void Elite::Renderer::Render(Scene* pScene, PerspectiveCamera* pCamera, LightManager* pLightManager, uint32_t startStep, uint32_t heightStep)
{
	SDL_LockSurface(m_pBackBuffer);

	Ray ray{ FPoint3{0.0f, 0.0f, 0.f}, FVector3{0.f,0.f,-1.f} };
	HitRecord hitRecord{}, closestHit{};
	Geometry* pHitGeometry{ nullptr };

	//Loop over all the pixels
	for (uint32_t r = startStep; r < heightStep; ++r)
	{
		for (uint32_t c = 0; c < m_Width; ++c)
		{
			// Spawn Ray
			
			pCamera->GenerateRay(ray, c, r);
			closestHit.tValue = FLT_MAX;
			pHitGeometry = nullptr;
			closestHit.pMaterial = nullptr;
			Elite::RGBColor color{ 0,0,0 };

			// Check if geometry hit
			for (Geometry* pGeometry : pScene->GetGeometry())
			{
				if (pGeometry->IsHit(ray, hitRecord))
				{
					if (hitRecord.tValue < closestHit.tValue)
					{
						pHitGeometry = pGeometry;
						closestHit = hitRecord;
					}
				}
			}

			// Add Lighting
			if (pHitGeometry)
			{
				for (Light* pLight : pLightManager->GetActiveLights())
				{
					bool lightWasBlocked{ false };

					Ray rayToLight{ FPoint3{ 0.0f, 0.0f, 0.f }, FVector3{ 0.f,0.f,-1.f } };
					HitRecord hitRecordToLight{};

					if (pLightManager->IsUsingHardShadows())
					{

						rayToLight.origin = closestHit.hitPoint;
						rayToLight.direction = Elite::GetNormalized(pLight->GetDirection(rayToLight.origin));

						float distanceToLight{ pLight->GetDistance(rayToLight.origin) };

						for (Geometry* pGeometry : pScene->GetGeometry())
						{
							if (pGeometry->IsHit(rayToLight, hitRecordToLight) && hitRecordToLight.tValue < distanceToLight)
							{
								lightWasBlocked = true;
								break;
							}
						}
					}

					if (!lightWasBlocked)
					{
						float irradiance{ Elite::Dot(closestHit.normal, pLight->GetDirection(closestHit.hitPoint)) };
						if (irradiance > 0)
						{
							LightManager::LightMode lightMode{ pLightManager->GetLightMode() };

							switch (lightMode)
							{
							case LightManager::LightMode::IrradianceOnly:
								color += pLight->GetERGB(closestHit.hitPoint) * irradiance;
								break;
							case LightManager::LightMode::BRDFOnly:
								color += closestHit.pMaterial->Shade(closestHit, pLight->GetDirection(closestHit.hitPoint), Elite::GetNormalized(FVector3{ ray.origin - closestHit.hitPoint })) * irradiance;
								break;
							case LightManager::LightMode::All:
								color += pLight->GetERGB(closestHit.hitPoint) * closestHit.pMaterial->Shade(closestHit, pLight->GetDirection(closestHit.hitPoint), Elite::GetNormalized(FVector3{ ray.origin - closestHit.hitPoint })) * irradiance;
								break;
							default:
								break;
							}

						}
					}

				}
			}

			color.MaxToOne();

			m_pBackBufferPixels[c + (r * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
				static_cast<uint8_t>(color.r * 255),
				static_cast<uint8_t>(color.g * 255),
				static_cast<uint8_t>(color.b * 255));
		}
	}

	SDL_UnlockSurface(m_pBackBuffer);
	SDL_BlitSurface(m_pBackBuffer, 0, m_pFrontBuffer, 0);
	SDL_UpdateWindowSurface(m_pWindow);
}


bool Elite::Renderer::SaveBackbufferToImage() const
{
	return SDL_SaveBMP(m_pBackBuffer, "BackbufferRender.bmp");
}
