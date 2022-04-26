/*=============================================================================*/
// Copyright 2017-2019 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// ERenderer.h: class that holds the surface to render too + DirectX initialization.
/*=============================================================================*/
#ifndef ELITE_RAYTRACING_RENDERER
#define	ELITE_RAYTRACING_RENDERER

#include <cstdint>

#include "ECamera.h"

#include "structs.h"
#include <vector>

struct SDL_Window;
struct SDL_Surface;

class Geometry;
class Triangle;
class MeshRepresentation;
class Texture;

namespace Elite
{
	class Renderer final
	{
	public:
		Renderer(SDL_Window* pWindow, Elite::Camera* pCamera);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void Render();
		void Update(float dt);

		void SwitchSampleFilter();
		void SwitchCullMode();
		void SwitchRenderMode();
		void ToggleRotating();
		void ToggleFireFX();

	private:
		SDL_Window* m_pWindow;
		uint32_t m_Width;
		uint32_t m_Height;

		bool m_IsInitialized;

		// Other
		bool m_IsRotating;
		float m_Angle;
		float m_RotateSpeed;

		// Camera
		Camera* m_pCamera;

		
		#pragma region SoftwareRasterizing
		SDL_Surface* m_pFrontBuffer = nullptr;
		SDL_Surface* m_pBackBuffer = nullptr;
		float* m_DepthBuffer;
		uint32_t* m_pBackBufferPixels = nullptr;

		void RenderTriangleMesh();
		void ResetDepthBuffer();
		void RenderTriangle(Geometry* pGeometry);
		void Phong(const Elite::FVector3& trueNormal, const Elite::FVector3& interpolatedViewDirection, const Triangle* pTriangle, const Elite::FVector2& interpolatedUV, Elite::RGBColor& color);
		void VertexTransformationFunction(const std::vector<Vertex_Input>& inputVertices, std::vector<Vertex_Input>& outputVertices) const;
		bool IsPointInTriangle(const std::vector<Vertex_Input>& triangleVertices, const Elite::FPoint2& point) const;
		Elite::RGBColor PixelShading(const std::vector<Vertex_Input>& v, const Elite::FVector3& interPolatedNormal);
		#pragma endregion
		

		#pragma region DirectX
		// DirectX stuff
		ID3D11Device* m_pDevice;
		ID3D11DeviceContext* m_pDeviceContext;
		IDXGIFactory* m_pDXGIFactory;
		IDXGISwapChain* m_pSwapChain;
		ID3D11Texture2D* m_pDepthStencilBuffer;
		ID3D11DepthStencilView* m_pDepthStencilView;
		ID3D11Resource* m_pRenderTargetBuffer;
		ID3D11RenderTargetView* m_pRenderTargetView;

		TextureSamplingState m_RenderTechnique = TextureSamplingState::POINT;
		CullMode m_CullMode = CullMode::BACKFACE;
		#pragma endregion


		#pragma region Meshes
		// Meshes
		//Vehicle
		MeshRepresentation* m_pVehicle;
		Texture* m_pDiffuseTexture;
		Texture* m_pNormalTexture;
		Texture* m_pSpecularTexture;
		Texture* m_pGlossinessTexture;
		//FireFX
		bool m_ShowFireFX;
		MeshRepresentation* m_pFireFX;
		Texture* m_pFireFXDiffuse;
		#pragma endregion

		

		// Sampling
		RenderMode m_RenderMode = RenderMode::HARDWARE;

		// Member Functions
		void InitializeDirectX();
	};
}

#endif