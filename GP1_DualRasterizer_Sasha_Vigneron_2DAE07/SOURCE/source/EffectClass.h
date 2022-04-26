#pragma once
#include "pch.h"
#include "structs.h"

class EffectClass abstract
{
public:
	EffectClass(ID3D11Device* pDevice, std::wstring assetFile);
	~EffectClass();

	EffectClass(const EffectClass&) = delete;
	EffectClass(EffectClass&&) noexcept = delete;
	EffectClass& operator=(const EffectClass&) = delete;
	EffectClass& operator=(EffectClass&&) noexcept = delete;

	ID3DX11Effect* GetEffect();
	ID3DX11EffectTechnique* GetTechnique();
	ID3DX11EffectVariable* GetVariableByName(LPCSTR name);

	virtual void SetTechnique(TextureSamplingState renderTechnique) = 0;

protected:
	ID3DX11Effect* m_pEffect;
	ID3DX11EffectShaderResourceVariable* m_pShaderResourceVariable;
	ID3DX11EffectTechnique* m_pActiveTechnique;

private:
	static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);
};
