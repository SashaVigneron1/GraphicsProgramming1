#include "SceneManager.h"

#include "Scene.h"

//// Geometry
#include "Geometry.h"
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"	
#include "TriangleMesh.h"

//// Lights
#include "Light.h"	
#include "PointLight.h"
#include "DirectionalLight.h"

//// Materials
#include "MaterialManager.h"
#include "BRDF.h"
#include "Material_Lambert.h"
#include "Material_LambertPhong.h"
#include "Material_PBR.h"

SceneManager::SceneManager(MaterialManager* pMaterialManager)
	: m_pMaterialManager{ pMaterialManager }
{
	// Materials
	m_pMaterialManager->AddMaterial("PBR_RoughMetal", new Material_PBR{ Elite::RGBColor{.972f, .960f, .915f}, 1.0f, 1.0f });
	m_pMaterialManager->AddMaterial("PBR_MediumMetal", new Material_PBR{ Elite::RGBColor{.972f, .960f, .915f}, 1.0f, 0.6f });
	m_pMaterialManager->AddMaterial("PBR_SmoothMetal", new Material_PBR{ Elite::RGBColor{.972f, .960f, .915f}, 1.0f, 0.1f });
	m_pMaterialManager->AddMaterial("PBR_RoughPlastic", new Material_PBR{ Elite::RGBColor{0.75f, .75f, .75f}, 0.0f, 1.0f });
	m_pMaterialManager->AddMaterial("PBR_MediumPlastic", new Material_PBR{ Elite::RGBColor{0.75f, .75f, .75f}, 0.0f, .4f });
	m_pMaterialManager->AddMaterial("PBR_SmoothPlastic", new Material_PBR{ Elite::RGBColor{0.75f, .75f, .75f}, 0.0f, .1f });
	m_pMaterialManager->AddMaterial("Lambert_GreyBlue", new Material_Lambert{ Elite::RGBColor{0.49f, 0.57f, .57f }, 1.f });
	m_pMaterialManager->AddMaterial("Lambert_White", new Material_Lambert{ Elite::RGBColor{1.f, 1.f, 1.f }, 1.f });

	//// Scene 1
	Scene* pScene = new Scene{};

	// Walls
	pScene->AddGeometry(new Plane{ FPoint3{0.f, 0.f, 0.f}, FVector3{0.f, 1.0f, 0.f}, m_pMaterialManager->GetMaterial("Lambert_GreyBlue")});
	pScene->AddGeometry(new Plane{FPoint3{0.f, 0.f, -10.f}, FVector3{0.f, 0.0f, 1.f}, m_pMaterialManager->GetMaterial("Lambert_GreyBlue") });
	pScene->AddGeometry(new Plane{ FPoint3{0.f, 10.f, 0.f}, FVector3{0.f, -1.f, 0.0f}, m_pMaterialManager->GetMaterial("Lambert_GreyBlue") });
	pScene->AddGeometry(new Plane{ FPoint3{5.f, 0.f, 0.f}, FVector3{-1.f, 0.f, 0.f}, m_pMaterialManager->GetMaterial("Lambert_GreyBlue") });
	pScene->AddGeometry(new Plane{ FPoint3{-5.f, 0.f, 0.f}, FVector3{1.f, 0.f, 0.f}, m_pMaterialManager->GetMaterial("Lambert_GreyBlue") });

	// Spheres
	Elite::RGBColor silver{ 0.972f,0.960f,0.915f };
	pScene->AddGeometry(new Sphere{ FPoint3{-1.75f, 1.f, 0.f}, .75f, m_pMaterialManager->GetMaterial("PBR_RoughMetal") });
	pScene->AddGeometry(new Sphere{ FPoint3{0.f, 1.f, 0.f}, .75f, m_pMaterialManager->GetMaterial("PBR_MediumMetal") });
	pScene->AddGeometry(new Sphere{ FPoint3{1.75f, 1.f, 0.f}, .75f, m_pMaterialManager->GetMaterial("PBR_SmoothMetal") });
	pScene->AddGeometry(new Sphere{ FPoint3{-1.75f, 3.f, 0.f}, .75f, m_pMaterialManager->GetMaterial("PBR_RoughPlastic") });
	pScene->AddGeometry(new Sphere{ FPoint3{0.f, 3.f, 0.f}, .75f,  m_pMaterialManager->GetMaterial("PBR_MediumPlastic") });
	pScene->AddGeometry(new Sphere{ FPoint3{1.75f, 3.f, 0.f}, .75f, m_pMaterialManager->GetMaterial("PBR_SmoothPlastic") });

	// Triangles
	pScene->AddGeometry(new Triangle{ FPoint3{ -1.75f, 4.5f, 0.f}, Triangle::CullMode::BackFace, m_pMaterialManager->GetMaterial("Lambert_White") });
	pScene->AddGeometry(new Triangle{ FPoint3{ 0.f, 4.5f, 0.f}, Triangle::CullMode::FrontFace, m_pMaterialManager->GetMaterial("Lambert_White") });
	pScene->AddGeometry(new Triangle{ FPoint3{ 1.75f, 4.5f, 0.f}, Triangle::CullMode::NoCulling, m_pMaterialManager->GetMaterial("Lambert_White") });

	m_pScenes.push_back(pScene);

	//// Scene 2
	pScene = new Scene{};

	// Walls
	pScene->AddGeometry(new Plane{ FPoint3{0.f, 0.f, 0.f}, FVector3{0.f, 1.0f, 0.f}, m_pMaterialManager->GetMaterial("Lambert_GreyBlue") });
	pScene->AddGeometry(new Plane{ FPoint3{0.f, 0.f, -10.f}, FVector3{0.f, 0.0f, 1.f}, m_pMaterialManager->GetMaterial("Lambert_GreyBlue") });
	pScene->AddGeometry(new Plane{ FPoint3{0.f, 10.f, 0.f}, FVector3{0.f, -1.f, 0.0f}, m_pMaterialManager->GetMaterial("Lambert_GreyBlue") });
	pScene->AddGeometry(new Plane{ FPoint3{5.f, 0.f, 0.f}, FVector3{-1.f, 0.f, 0.f}, m_pMaterialManager->GetMaterial("Lambert_GreyBlue") });
	pScene->AddGeometry(new Plane{ FPoint3{-5.f, 0.f, 0.f}, FVector3{1.f, 0.f, 0.f}, m_pMaterialManager->GetMaterial("Lambert_GreyBlue") });

	pScene->AddGeometry(new TriangleMesh{ "lowpoly_bunny.obj" , FVector3{0,0,0}, m_pMaterialManager->GetMaterial("Lambert_White") });

	m_pScenes.push_back(pScene);
}

SceneManager::~SceneManager()
{
	for (Scene* scene : m_pScenes) 
	{
		delete scene;
	}
}

Scene* SceneManager::GetScene(int sceneIndex)
{
	return m_pScenes[sceneIndex];
}

void SceneManager::Update(int sceneIndex, float deltaT)
{
	if (sceneIndex < m_pScenes.size())
	{
		for (Geometry* pGeometry : m_pScenes[sceneIndex]->GetGeometry())
		{
			pGeometry->Update(deltaT);
		}
	}
	
}
