#include "MaterialManager.h"

MaterialManager::MaterialManager()
{
}

MaterialManager::~MaterialManager()
{
	for (auto pair : m_pMaterials) 
	{
		delete pair.second;
	}
}

void MaterialManager::AddMaterial(const std::string& name, BRDF* pMaterial)
{
	m_pMaterials.emplace(name, pMaterial);
}

BRDF* MaterialManager::GetMaterial(const std::string& name)
{
	return m_pMaterials.find(name)->second;
}
