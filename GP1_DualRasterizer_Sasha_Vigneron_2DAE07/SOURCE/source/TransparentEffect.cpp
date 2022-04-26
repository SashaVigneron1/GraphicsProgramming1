#include "pch.h"
#include <sstream>
#include "TransparentEffect.h"

TransparentEffect::TransparentEffect(ID3D11Device* pDevice, std::wstring assetFile)
	: EffectClass{ pDevice, assetFile }
{
	m_pPointTechnique = m_pEffect->GetTechniqueByName("PointTechnique");
	m_pLinearTechnique = m_pEffect->GetTechniqueByName("LinearTechnique");
	m_pAnisotropicTechnique = m_pEffect->GetTechniqueByName("AnisotropicTechnique");

	if (!m_pPointTechnique->IsValid())
		std::wcout << L"Point Technique not valid\n";
	if (!m_pLinearTechnique->IsValid())
		std::wcout << L"Linear Technique not valid\n";
	if (!m_pAnisotropicTechnique->IsValid())
		std::wcout << L"Anisotropic Technique not valid\n";

	m_pActiveTechnique = m_pPointTechnique;
}

TransparentEffect::~TransparentEffect()
{
	m_pPointTechnique->Release();
	m_pLinearTechnique->Release();
	m_pAnisotropicTechnique->Release();
	m_pEffect->Release();
}

void TransparentEffect::SetTechnique(TextureSamplingState renderTechnique)
{
	switch (renderTechnique)
	{
	case TextureSamplingState::POINT:
		m_pActiveTechnique = m_pPointTechnique;
		break;
	case TextureSamplingState::LINEAR:
		m_pActiveTechnique = m_pLinearTechnique;
		break;
	case TextureSamplingState::ANISOTROPIC:
		m_pActiveTechnique = m_pAnisotropicTechnique;
		break;
	}

}
