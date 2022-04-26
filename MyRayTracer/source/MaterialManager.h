#pragma once

#include "BRDF.h"
#include <map>
#include <iostream>
#include <string>

class MaterialManager
{
public:
	MaterialManager();
	~MaterialManager();

	MaterialManager(const MaterialManager&) = delete;
	MaterialManager(MaterialManager&&) noexcept = delete;
	MaterialManager& operator=(const MaterialManager&) = delete;
	MaterialManager& operator=(MaterialManager&&) noexcept = delete;

	void AddMaterial(const std::string& name, BRDF* pMaterial);
	BRDF* GetMaterial(const std::string& name);


private:
	std::map<std::string, BRDF*> m_pMaterials;
};

