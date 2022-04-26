#include "pch.h"

//Project includes
#include "ERenderer.h"

//My includes
#include "Texture.h"
#include "Triangle.h"
#include "MeshRepresentation.h"


Elite::Renderer::Renderer(SDL_Window * pWindow, Elite::Camera* pCamera)
	: m_pWindow{ pWindow }
	, m_Width{}
	, m_Height{}
	, m_IsInitialized{ false }
	, m_pCamera{ pCamera }
	, m_Angle{0.f}
	, m_RotateSpeed{ Elite::ToRadians(45.f) }
	, m_IsRotating{ true }
	, m_ShowFireFX{ true }
{
	int width, height = 0;
	SDL_GetWindowSize(pWindow, &width, &height);
	m_Width = static_cast<uint32_t>(width);
	m_Height = static_cast<uint32_t>(height);

	m_pFrontBuffer = SDL_GetWindowSurface(pWindow);
	m_DepthBuffer = new float[width * height]{};
	ResetDepthBuffer();
	m_pBackBuffer = SDL_CreateRGBSurface(0, m_Width, m_Height, 32, 0, 0, 0, 0);
	m_pBackBufferPixels = (uint32_t*)m_pBackBuffer->pixels;

	InitializeDirectX();
	
	// Vehicle
	m_pVehicle = new MeshRepresentation(m_pDevice, "Resources/vehicle.obj", FVector3(0,0,50));
	m_pDiffuseTexture = new Texture("Resources/vehicle_diffuse.png", m_pDevice);
	m_pNormalTexture = new Texture("Resources/vehicle_normal.png", m_pDevice);
	m_pSpecularTexture = new Texture("Resources/vehicle_specular.png", m_pDevice);
	m_pGlossinessTexture = new Texture("Resources/vehicle_gloss.png", m_pDevice);

	m_pVehicle->SetDiffuseMap(m_pDiffuseTexture->GetResourceView());
	m_pVehicle->SetNormalMap(m_pNormalTexture->GetResourceView());
	m_pVehicle->SetSpecularMap(m_pSpecularTexture->GetResourceView());
	m_pVehicle->SetGlossinessMap(m_pGlossinessTexture->GetResourceView());

	// FireFX
	m_pFireFX = new MeshRepresentation(m_pDevice, "Resources/fireFX.obj", FVector3(0,0,50), true);
	m_pFireFXDiffuse = new Texture("Resources/fireFX_diffuse.png", m_pDevice);

	m_pFireFX->SetDiffuseMap(m_pFireFXDiffuse->GetResourceView());
}

Elite::Renderer::~Renderer()
{
	// SoftwareRasterizer
	delete[] m_DepthBuffer;

	// DirectX
	if (m_pVehicle) delete m_pVehicle;
	if (m_pDiffuseTexture) delete m_pDiffuseTexture;
	if (m_pNormalTexture) delete m_pNormalTexture;
	if (m_pSpecularTexture) delete m_pSpecularTexture;
	if (m_pGlossinessTexture) delete m_pGlossinessTexture;

	if (m_pFireFX) delete m_pFireFX;
	if (m_pFireFXDiffuse) delete m_pFireFXDiffuse;

	if (m_pRenderTargetView) m_pRenderTargetView->Release();
	if (m_pRenderTargetBuffer) m_pRenderTargetBuffer->Release();
	if (m_pDepthStencilView) m_pDepthStencilView->Release();
	if (m_pDepthStencilBuffer) m_pDepthStencilBuffer->Release();
	if (m_pSwapChain) m_pSwapChain->Release();
	if (m_pDXGIFactory) m_pDXGIFactory->Release();

	if (m_pDeviceContext) 
	{
		m_pDeviceContext->ClearState();
		m_pDeviceContext->Flush();
		m_pDeviceContext->Release();
	}
	if (m_pDevice) m_pDevice->Release();
}

void Elite::Renderer::Render()
{
	if (m_RenderMode == RenderMode::HARDWARE) 
	{
		if (!m_IsInitialized)
			return;

		//Clear Buffers
		RGBColor clearColor = RGBColor(0.1f, 0.1f, 0.1f);
		m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, &clearColor.r);
		m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		// Initialize Variables
		FVector3 vehiclePos = m_pVehicle->GetPosition();

		FMatrix4 world = (FMatrix4)Elite::MakeTranslation(vehiclePos);
		world *= (FMatrix4)Elite::MakeRotationY(m_Angle);

		FMatrix4 worldViewPort = m_pCamera->GetWorldViewProjection();
		FMatrix4 viewInv = m_pCamera->GetViewToWorld();

		// Render
		m_pCamera->SetWorldMatrix(world);
		m_pVehicle->SetWorldViewMatrix(reinterpret_cast<float*>(worldViewPort.data));
		m_pVehicle->SetWorldMatrix(reinterpret_cast<float*>(world.data));
		m_pVehicle->SetViewInverseMatrix(reinterpret_cast<float*>(viewInv.data));
		m_pVehicle->DoRender(m_pDeviceContext);

		if (m_ShowFireFX)
		{
			m_pFireFX->SetWorldViewMatrix(reinterpret_cast<float*>(worldViewPort.data));
			m_pFireFX->SetWorldMatrix(reinterpret_cast<float*>(world.data));
			m_pFireFX->SetViewInverseMatrix(reinterpret_cast<float*>(viewInv.data));
			m_pFireFX->DoRender(m_pDeviceContext);
		}

		//Present
		m_pSwapChain->Present(0, 0);
	}
	else 
	{
		SDL_LockSurface(m_pBackBuffer);

		ResetDepthBuffer();

		// Reset to black
		SDL_FillRect(m_pBackBuffer, nullptr, 0xFF191919);

		RenderTriangleMesh();

		SDL_UnlockSurface(m_pBackBuffer);
		SDL_BlitSurface(m_pBackBuffer, 0, m_pFrontBuffer, 0);
		SDL_UpdateWindowSurface(m_pWindow);
	}
	
	

}

void Elite::Renderer::Update(float dt)
{
	if (m_IsRotating) m_Angle += dt * m_RotateSpeed;
}

#pragma region DirectX
void Elite::Renderer::SwitchSampleFilter()
{
	m_RenderTechnique = TextureSamplingState((int)m_RenderTechnique + 1);
	if (m_RenderTechnique == TextureSamplingState::LASTELEMENT) m_RenderTechnique = TextureSamplingState(0);

	switch (m_RenderTechnique)
	{
	case TextureSamplingState::POINT:
		std::cout << "Now rendering using the POINT filter.\n";
		break;
	case TextureSamplingState::LINEAR:
		std::cout << "Now rendering using the LINEAR filter.\n";
		break;
	case TextureSamplingState::ANISOTROPIC:
		std::cout << "Now rendering using the ANISOTROPIC filter.\n";
		break;
	default:
		std::cerr << "Something went wrong switching Sampling Filter.\n";
		break;
	}

	m_pVehicle->SetTextureSamplingState(m_RenderTechnique);
	m_pFireFX->SetTextureSamplingState(m_RenderTechnique);
}

void Elite::Renderer::SwitchCullMode()
{
	m_CullMode = CullMode((int)m_CullMode + 1);
	if (m_CullMode == CullMode::LASTELEMENT) m_CullMode = CullMode(0);

	switch (m_CullMode)
	{
	case CullMode::BACKFACE:
		std::cout << "Now rendering using BACKFACE CULLING.\n";
		break;
	case CullMode::FRONTFACE:
		std::cout << "Now rendering using the FRONTFACE CULLING.\n";
		break;
	case CullMode::NONE:
		std::cout << "Now rendering using the NO CULLING.\n";
		break;
	default:
		std::cerr << "Something went wrong switching Culling Mode.\n";
		break;
	}

	m_pVehicle->SetCullMode(m_CullMode);
	m_pFireFX->SetCullMode(m_CullMode);
}

void Elite::Renderer::SwitchRenderMode()
{
	m_RenderMode = RenderMode((int)m_RenderMode + 1);
	if (m_RenderMode == RenderMode::LASTELEMENT) m_RenderMode = RenderMode(0);

	switch (m_RenderMode)
	{
	case RenderMode::HARDWARE:
		std::cout << "Now rendering using HARDWARE RASTERIZER.\n";
		break;
	case RenderMode::SOFTWARE:
		std::cout << "Now rendering using the SOFTWARE RASTERIZER.\n";
		break;
	default:
		std::cerr << "Something went wrong switching Culling Mode.\n";
		break;
	}
}

void Elite::Renderer::ToggleRotating()
{
	m_IsRotating = !m_IsRotating;
	if (m_IsRotating) std::cout << "Rotating is now ENABLED.\n";
	else std::cout << "Rotating is now DISABLED.\n";
}

void Elite::Renderer::ToggleFireFX()
{
	m_ShowFireFX = !m_ShowFireFX;
	if (m_ShowFireFX) std::cout << "FireFX is now ENABLED.\n";
	else std::cout << "FireFX is now DISABLED.\n";
}

void Elite::Renderer::InitializeDirectX()
{
	//Initialize DirectX pipeline
	#pragma region Device
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	uint32_t createDeviceFlags = 0;
	#if defined(DEBUG) || defined(_DEBUG)
		createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
	#endif
	HRESULT result = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, createDeviceFlags, 0, 0, D3D11_SDK_VERSION, &m_pDevice, &featureLevel, &m_pDeviceContext);
	if (FAILED(result)) 
	{
		std::cerr << "Something went wrong creating DirectX Device!\n";
		return;
	}
	#pragma endregion

	#pragma region SwapChain
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&m_pDXGIFactory));
	if (FAILED(result))
	{
		std::cerr << "Something went wrong creating the Factory!\n";
		return;
	}

	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferDesc.Width = m_Width;
	swapChainDesc.BufferDesc.Height = m_Height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 1;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 60;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	SDL_SysWMinfo sysWMInfo{};
	SDL_VERSION(&sysWMInfo.version);
	SDL_GetWindowWMInfo(m_pWindow, &sysWMInfo);
	swapChainDesc.OutputWindow = sysWMInfo.info.win.window;

	result = m_pDXGIFactory->CreateSwapChain(m_pDevice, &swapChainDesc, &m_pSwapChain);
	if (FAILED(result))
	{
		std::cerr << "Something went wrong creating the Swap Chain!\n";
		return;
	}
	#pragma endregion

	#pragma region DepthStencils
	D3D11_TEXTURE2D_DESC depthStencilDesc{};
	depthStencilDesc.Width = m_Width;
	depthStencilDesc.Height = m_Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = depthStencilDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = m_pDevice->CreateTexture2D(&depthStencilDesc, 0, &m_pDepthStencilBuffer);
	if (FAILED(result))
	{
		std::cerr << "Something went wrong creating the Depth Stencils\n";
		return;
	}
	result = m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView);
	if (FAILED(result))
	{
		std::cerr << "Something went wrong creating the Depth Stencils\n";
		return;
	}
	#pragma endregion

	#pragma region RenderTargetView
	// Create the RenderTargetView
	result = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_pRenderTargetBuffer));
	if (FAILED(result))
	{
		std::cerr << "Something went wrong creating the Render Target View\n";
		return;
	}
	result = m_pDevice->CreateRenderTargetView(m_pRenderTargetBuffer, 0, &m_pRenderTargetView);
	if (FAILED(result))
	{
		std::cerr << "Something went wrong creating the Render Target View\n";
		return;
	}
	#pragma endregion

	// Bind the views to the output merger stage
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	#pragma region Viewport
	D3D11_VIEWPORT viewport{};
	viewport.Width = static_cast<float>(m_Width);
	viewport.Height = static_cast<float>(m_Height);
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	m_pDeviceContext->RSSetViewports(1, &viewport);
	#pragma endregion

	m_IsInitialized = true;
	std::cout << "DirectX is ready!\n";
}
#pragma endregion

#pragma region SoftwareRasterizer
void Elite::Renderer::RenderTriangleMesh()
{
	std::vector<Vertex_Input> vertices{ m_pVehicle->GetVertexBuffer() };
	std::vector<uint32_t> indexes{ m_pVehicle->GetIndexBuffer() };
	for (size_t i = 0; i < indexes.size() - 2; i += 3)
	{
		m_pVehicle->SetLocalVertices({ vertices[indexes[i]], vertices[indexes[i + 1]], vertices[indexes[i + 2]] });
		RenderTriangle(m_pVehicle->GetTriangle());
	}
}

void Elite::Renderer::RenderTriangle(Geometry* pGeometry)
{
	Triangle* pTriangle = dynamic_cast<Triangle*>(pGeometry);
	if (pTriangle)
	{
		std::vector<Vertex_Input> transformedVertices;

		#pragma region Transformation
		// Transform vertices to view space
		VertexTransformationFunction(pTriangle->GetColoredVertices(), transformedVertices);

		// Frustum Culling
		for (size_t i = 0; i < transformedVertices.size(); i++)
		{
			if (transformedVertices[i].Position.x > 1 || transformedVertices[i].Position.x < -1) return;
			if (transformedVertices[i].Position.y > 1 || transformedVertices[i].Position.y < -1) return;
			if (transformedVertices[i].Position.z > 1 || transformedVertices[i].Position.z < 0) return;
		}

		// To NDC Space
		for (Vertex_Input& vertex : transformedVertices)
		{
			vertex.Position.x = (1 + vertex.Position.x) / 2 * m_Width;
			vertex.Position.y = (1 - vertex.Position.y) / 2 * m_Height;
		}
		#pragma endregion

		#pragma region Boundingbox
		// Bounding Box
		// x = minX
		// y = maxX
		// z = minY
		// w = maxY
		Elite::IVector4 boundingBox{ INT_MAX, 0, INT_MAX, 0 };

		for (const Vertex_Input& vertex : transformedVertices)
		{
			boundingBox.x = std::min(boundingBox.x, (int)vertex.Position.x);
			boundingBox.y = std::max(boundingBox.y, (int)vertex.Position.x + 1);
			boundingBox.z = std::min(boundingBox.z, (int)vertex.Position.y);
			boundingBox.w = std::max(boundingBox.w, (int)vertex.Position.y + 1);
		}
		// Clamp 
		boundingBox.x = std::max(boundingBox.x, 0);
		boundingBox.y = std::min(boundingBox.y, (int)m_Width);
		boundingBox.z = std::max(boundingBox.z, 0);
		boundingBox.w = std::min(boundingBox.w, (int)m_Height);
		#pragma endregion 

		// Loop over all the pixels in the box
		for (int r = boundingBox.z; r < boundingBox.w; ++r)
		{
			for (int c = boundingBox.x; c < boundingBox.y; ++c)
			{
				Elite::RGBColor color{ 0,0,0 };

				Elite::FPoint2 pixelPos{ (float)c + 0.5f, (float)r + 0.5f }; // + 0.5f for center of pixel

				if (IsPointInTriangle(transformedVertices, pixelPos))
				{
					// Color
					Elite::FVector2 v0 = Elite::FVector2{ transformedVertices[0].Position.xy };
					Elite::FVector2 v1 = Elite::FVector2{ transformedVertices[1].Position.xy };
					Elite::FVector2 v2 = Elite::FVector2{ transformedVertices[2].Position.xy };

					FVector2 screenPoint{ pixelPos.x, pixelPos.y };

					float v0w = Elite::Cross(v2 - v1, screenPoint - v1);
					float v2w = Elite::Cross(v1 - v0, screenPoint - v0);
					float v1w = Elite::Cross(v0 - v2, screenPoint - v2);
					float totalArea = v0w + v1w + v2w;

					v0w /= totalArea;
					v1w /= totalArea;
					v2w /= totalArea;

					float zDepth = 1 / (
						((1 / transformedVertices[0].Position.z) * v0w) +
						((1 / transformedVertices[1].Position.z) * v1w) +
						((1 / transformedVertices[2].Position.z) * v2w)
						);

					float wInterpolated = 1 / (
						((1 / transformedVertices[0].Position.w) * v0w) +
						((1 / transformedVertices[1].Position.w) * v1w) +
						((1 / transformedVertices[2].Position.w) * v2w)
						);

					if (zDepth < m_DepthBuffer[c + r * m_Width])
					{
						m_DepthBuffer[c + r * m_Width] = zDepth;


						Elite::FVector2 interpolatedUV{ (((transformedVertices[0].UV / transformedVertices[0].Position.w) * v0w) +
													((transformedVertices[1].UV / transformedVertices[1].Position.w) * v1w) +
													((transformedVertices[2].UV / transformedVertices[2].Position.w) * v2w))
													* wInterpolated };

						color = m_pDiffuseTexture->Sample(interpolatedUV);

						// Interpolation
						#pragma region PixelShading
						Elite::FVector3 interpolatedNormal{
												(((transformedVertices[0].Normal / transformedVertices[0].Position.w) * v0w) +
												((transformedVertices[1].Normal / transformedVertices[1].Position.w) * v1w) +
												((transformedVertices[2].Normal / transformedVertices[2].Position.w) * v2w))
												* wInterpolated
						};
						interpolatedNormal = Elite::GetNormalized(interpolatedNormal);
						Elite::FVector3 interpolatedTangent{
												(((transformedVertices[0].Tangent / transformedVertices[0].Position.w) * v0w) +
												((transformedVertices[1].Tangent / transformedVertices[1].Position.w) * v1w) +
												((transformedVertices[2].Tangent / transformedVertices[2].Position.w) * v2w))
												* wInterpolated
						};
						interpolatedTangent = Elite::GetNormalized(interpolatedTangent);

						Elite::FVector3 binormal = Elite::Cross(interpolatedNormal, interpolatedTangent);
						Elite::FMatrix3 tangentSpaceAxis = FMatrix3(interpolatedTangent, binormal, interpolatedNormal);
						Elite::RGBColor normalSample = m_pNormalTexture->Sample(interpolatedUV);
						Elite::FVector3 trueNormal{ tangentSpaceAxis * FVector3{2 * normalSample.r - 1, 2 * normalSample.g - 1, 2 * normalSample.b - 1} };

						// Normal Map
						color *= PixelShading(transformedVertices, trueNormal);

						// Add Phong
						FVector3 interpolatedViewDirection =
							(((transformedVertices[0].viewDirection / pTriangle->GetColoredVertices()[0].Position.w) * v0w) +
								((transformedVertices[1].viewDirection / pTriangle->GetColoredVertices()[1].Position.w) * v1w) +
								((transformedVertices[2].viewDirection / pTriangle->GetColoredVertices()[2].Position.w) * v2w))
							* wInterpolated;
						interpolatedViewDirection = Elite::GetNormalized(interpolatedViewDirection);

						Phong(trueNormal, interpolatedViewDirection, pTriangle, interpolatedUV, color);
						#pragma endregion

						// Ambient Color
						const RGBColor ambientColor = { 0.025f, 0.025f, 0.025f };
						color += ambientColor;
						color.MaxToOne();

						//Fill the pixels - pixel access demo
						m_pBackBufferPixels[c + (r * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
							static_cast<uint8_t>(color.r * 255),
							static_cast<uint8_t>(color.g * 255),
							static_cast<uint8_t>(color.b * 255));
					}
				}
			}
		}
	}
}

void Elite::Renderer::Phong(const Elite::FVector3& trueNormal, const Elite::FVector3& interpolatedViewDirection, const Triangle* pTriangle, const Elite::FVector2& interpolatedUV, Elite::RGBColor& color)
{
	const float shinyness = 25.0f;
	const FVector3 lightDirection = { .577f, -.577f, .577f };

	FVector3 reflect = lightDirection - (2 * Elite::Dot(trueNormal, lightDirection) * trueNormal);

	float cos = Elite::Dot(reflect, interpolatedViewDirection);

	if (cos > 0)
	{
		float specular = m_pSpecularTexture->Sample(interpolatedUV).r;
		float exp = m_pGlossinessTexture->Sample(interpolatedUV).r;

		float phong = specular * std::powf(cos, exp * shinyness);
		Elite::RGBColor specularColor{ phong, phong, phong };
		color += specularColor;
	}
}

// Function that transforms the vertices from the mesh in world space into screen space
void Elite::Renderer::VertexTransformationFunction(const std::vector<Vertex_Input>& inputVertices, std::vector<Vertex_Input>& outputVertices) const
{
	// Initialize variables
	const uint32_t width{ m_Width };
	const uint32_t height{ m_Height };
	const float fov{ m_pCamera->GetFov() };
	const float aspectRatio = width / (float)height;

	outputVertices = inputVertices;
	for (size_t i = 0; i < outputVertices.size(); i++)
	{
		auto rotMatrix{ Elite::MakeRotationY(m_Angle) };
		outputVertices[i].Position = Elite::FPoint4(rotMatrix * outputVertices[i].Position.xyz); // Rotate
		outputVertices[i].Position += m_pVehicle->GetPosition(); // Translate
		outputVertices[i].Tangent = (rotMatrix * outputVertices[i].Tangent);
		outputVertices[i].Normal = (rotMatrix * outputVertices[i].Normal);

		outputVertices[i].viewDirection = m_pCamera->GetPosition() - FVector3(outputVertices[i].Position.xyz);


		outputVertices[i].Position = m_pCamera->GetProjectionMatrix() * m_pCamera->GetWorldToView() * outputVertices[i].Position;
		outputVertices[i].Position.x /= outputVertices[i].Position.w;
		outputVertices[i].Position.y /= outputVertices[i].Position.w;
		outputVertices[i].Position.z /= outputVertices[i].Position.w;
	}
}

bool Elite::Renderer::IsPointInTriangle(const std::vector<Vertex_Input>& triangleVertices, const Elite::FPoint2& point) const
{
	Elite::FVector3 v0 = Elite::FVector3{ triangleVertices[0].Position.xy };
	Elite::FVector3 v1 = Elite::FVector3{ triangleVertices[1].Position.xy };
	Elite::FVector3 v2 = Elite::FVector3{ triangleVertices[2].Position.xy };

	Elite::FVector3 screenPoint{ point.x, point.y, 0 };

	Elite::FVector3 u = Elite::Cross(v2 - v1, screenPoint - v1);
	Elite::FVector3 v = Elite::Cross(v1 - v0, screenPoint - v0);
	Elite::FVector3 w = Elite::Cross(v0 - v2, screenPoint - v2);

	if (u.z > 0.0f) { return false; }
	if (v.z > 0.0f) { return false; }
	if (w.z > 0.0f) { return false; }
	return true;
}

Elite::RGBColor Elite::Renderer::PixelShading(const std::vector<Vertex_Input>& v, const Elite::FVector3& interPolatedNormal)
{
	const FVector3 lightDirection = { .577f, -.577f, .577f };
	const float lightIntensity = 7.0f / (float)E_PI;
	const Elite::RGBColor lightColor = { 1,1,1 };

	const float observedArea = std::max(Elite::Dot(-interPolatedNormal, lightDirection), 0.0f);

	Elite::RGBColor finalColor = lightColor * lightIntensity * observedArea;

	return finalColor;
}

void Elite::Renderer::ResetDepthBuffer()
{
	for (uint32_t i = 0; i < m_Height * m_Width; i++)
	{
		m_DepthBuffer[i] = FLT_MAX;
	}
}
#pragma endregion

