#include "Geometry.h"

Geometry::Geometry(BRDF* pMaterial)
	: m_pMaterial{ pMaterial }
{
}

Geometry::~Geometry()
{
	//delete m_pMaterial; This is done from the materialmanager now
}

BRDF* Geometry::GetMaterial() const
{
	return m_pMaterial;
}
