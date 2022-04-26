#pragma once
#include "EMath.h"
#include <vector>

class Geometry
{
public:
	Geometry(const Elite::FPoint3& pos, const std::vector<Elite::FPoint4>& localVertices = {});
	virtual ~Geometry() = default;

	Geometry(const Geometry&) = delete;
	Geometry(Geometry&&) noexcept = delete;
	Geometry& operator=(const Geometry&) = delete;
	Geometry& operator=(Geometry&&) noexcept = delete;

	virtual std::vector<Elite::FPoint4> GetPositions() const = 0;

protected:
	Elite::FPoint4 m_Position;
	std::vector<Elite::FPoint4> m_LocalVertices;
};

