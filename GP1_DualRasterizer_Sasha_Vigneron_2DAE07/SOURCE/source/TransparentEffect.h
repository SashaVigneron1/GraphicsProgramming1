#pragma once
#include "pch.h"
#include "EffectClass.h"

class TransparentEffect final : public EffectClass
{
public:
	TransparentEffect(ID3D11Device* pDevice, std::wstring assetFile);
	virtual ~TransparentEffect();

	TransparentEffect(const TransparentEffect&) = delete;
	TransparentEffect(TransparentEffect&&) noexcept = delete;
	TransparentEffect& operator=(const TransparentEffect&) = delete;
	TransparentEffect& operator=(TransparentEffect&&) noexcept = delete;

	virtual void SetTechnique(TextureSamplingState renderTechnique);

private:

	ID3DX11EffectTechnique* m_pPointTechnique;
	ID3DX11EffectTechnique* m_pLinearTechnique;
	ID3DX11EffectTechnique* m_pAnisotropicTechnique;

};
