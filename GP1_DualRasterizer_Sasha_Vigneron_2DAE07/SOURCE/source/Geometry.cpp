#include "pch.h"
#include "Geometry.h"

Geometry::Geometry(const Elite::FPoint3& pos, const std::vector<Elite::FPoint4>& localVertices)
	: m_Position{ pos }
	, m_LocalVertices{ localVertices }
{
}
