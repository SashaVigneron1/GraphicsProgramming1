#pragma once
#include <vector>

class MaterialManager;
class Scene;

class SceneManager final
{
public:
	SceneManager(MaterialManager* pMaterialManager);
	~SceneManager();

	SceneManager(const SceneManager&) = delete;
	SceneManager(SceneManager&&) noexcept = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	SceneManager& operator=(SceneManager&&) noexcept = delete;

	Scene* GetScene(int sceneIndex);

	void Update(int sceneIndex, float deltaT);
private:
	std::vector<Scene*> m_pScenes;
	MaterialManager* m_pMaterialManager;
};

