#pragma once
#include "pch.h"
#include "EffectClass.h"
#include <sstream>

EffectClass::EffectClass(ID3D11Device* pDevice, std::wstring assetFile)
{
	m_pEffect = LoadEffect(pDevice, assetFile);
}

EffectClass::~EffectClass()
{
	m_pEffect->Release();
}

ID3DX11Effect* EffectClass::GetEffect()
{
	return m_pEffect;
}

ID3DX11EffectTechnique* EffectClass::GetTechnique()
{
	return m_pActiveTechnique;
}

ID3DX11EffectVariable* EffectClass::GetVariableByName(LPCSTR name)
{
	return m_pEffect->GetVariableByName(name);
}

ID3DX11Effect* EffectClass::LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile)
{
	HRESULT result = S_OK;
	ID3D10Blob* pErrorBlob = nullptr;
	ID3DX11Effect* pEffect;

	DWORD shaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	result = D3DX11CompileEffectFromFile(assetFile.c_str(),
		nullptr,
		nullptr,
		shaderFlags,
		0,
		pDevice,
		&pEffect,
		&pErrorBlob);
	if (FAILED(result))
	{
		if (pErrorBlob != nullptr)
		{
			char* pErrors = (char*)pErrorBlob->GetBufferPointer();

			std::wstringstream ss;
			for (unsigned int i = 0; i < pErrorBlob->GetBufferSize(); i++)
				ss << pErrors[i];

			OutputDebugStringW(ss.str().c_str());
			pErrorBlob->Release();
			pErrorBlob = nullptr;

			std::wcout << ss.str() << std::endl;
		}
		else
		{
			std::wstringstream ss;
			ss << "EffectLoader: Failed to CreateEffectFromFIle!\nPath:" << assetFile;
			std::wcout << ss.str() << std::endl;
			return nullptr;
		}
	}
	return pEffect;
}
