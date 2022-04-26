#pragma once
#include <vector>

class Geometry;
class Light;

class Scene final
{
public:
	Scene() = default;
	~Scene();

	Scene(const Scene& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(Scene&& other) = delete;

	void AddGeometry(Geometry* pGeometry);
	std::vector<Geometry*> GetGeometry() const;

private:
	std::vector<Geometry*> m_pGeometry;
};

