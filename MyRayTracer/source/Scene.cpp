#include "Scene.h"
#include "Geometry.h"
#include "Light.h"

Scene::~Scene()
{
	for (Geometry* pGeometry : m_pGeometry)
	{
		delete pGeometry; 
		pGeometry = nullptr;
	}
}

void Scene::AddGeometry(Geometry* pGeometry)
{
	m_pGeometry.push_back(pGeometry);
}

std::vector<Geometry*> Scene::GetGeometry() const
{
	return m_pGeometry;
}

