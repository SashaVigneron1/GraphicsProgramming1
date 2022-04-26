#pragma once
#include "pch.h"
#include "EffectClass.h"

class ShadingEffect final : public EffectClass
{
public:
	ShadingEffect(ID3D11Device* pDevice, std::wstring assetFile);
	virtual ~ShadingEffect();

	ShadingEffect(const ShadingEffect&) = delete;
	ShadingEffect(ShadingEffect&&) noexcept = delete;
	ShadingEffect& operator=(const ShadingEffect&) = delete;
	ShadingEffect& operator=(ShadingEffect&&) noexcept = delete;

	virtual void SetTechnique(TextureSamplingState renderTechnique);

private:

	ID3DX11EffectTechnique* m_pPointTechnique;
	ID3DX11EffectTechnique* m_pLinearTechnique;
	ID3DX11EffectTechnique* m_pAnisotropicTechnique;

};
