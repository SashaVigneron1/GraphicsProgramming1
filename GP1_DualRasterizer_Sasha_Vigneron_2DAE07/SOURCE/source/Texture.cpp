#include "pch.h"
#include "Texture.h"
#include "SDL_image.h"

Texture::Texture(const std::string& filePath, ID3D11Device* pDevice)
{
	m_pSurface = IMG_Load(filePath.c_str());

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = m_pSurface->w;
	desc.Height = m_pSurface->h;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = m_pSurface->pixels;
	initData.SysMemPitch = static_cast<UINT>(m_pSurface->pitch);
	initData.SysMemSlicePitch = static_cast<UINT>(m_pSurface->h * m_pSurface->pitch);


	HRESULT result = pDevice->CreateTexture2D(&desc, &initData, &m_pTexture);
	if (FAILED(result))
	{
		std::cerr << "Something went wrong creating the Texture2D.\n";
		return;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc{};
	SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;

	result = pDevice->CreateShaderResourceView(m_pTexture, &SRVDesc, &m_pTextureResourceView);
	if (FAILED(result))
	{
		std::cerr << "Something went wrong creating the Texture2D.\n";
		return;
	}

}

Texture::~Texture()
{
	m_pTextureResourceView->Release();
	m_pTexture->Release();
	SDL_FreeSurface(m_pSurface);
}

ID3D11ShaderResourceView* Texture::GetResourceView() const
{
	return m_pTextureResourceView;
}

ID3D11Texture2D* Texture::GetTexture() const
{
	return m_pTexture;
}

Elite::RGBColor Texture::Sample(const Elite::FVector2& uv)
{
	Uint8 r;
	Uint8 g;
	Uint8 b;

	Uint32 pixelId = GetPixelIndex(uv);
	SDL_GetRGB(((uint32_t*)m_pSurface->pixels)[pixelId], m_pSurface->format, &r, &g, &b);

	return Elite::RGBColor{ r / 255.f, g / 255.f, b / 255.f };
}

Uint32 Texture::GetPixelIndex(const Elite::FVector2& uv) const
{
	return (Uint32(std::floor(uv.x * m_pSurface->w)) + Uint32(std::floor(uv.y * m_pSurface->h) * m_pSurface->w));
}
