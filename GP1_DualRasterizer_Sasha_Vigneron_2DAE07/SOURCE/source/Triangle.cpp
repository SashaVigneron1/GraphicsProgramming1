#include "pch.h"
#include "Triangle.h"

Triangle::Triangle(const Elite::FPoint3& pos, const std::vector<Vertex_Input>& localVertices)
	: Geometry{ pos }
	, m_LocalVertices{ localVertices }
{

}

Triangle::~Triangle()
{
	
}

std::vector<Elite::FPoint4> Triangle::GetPositions() const
{
	std::vector<Elite::FPoint4> positions;

	for (size_t i = 0; i < m_LocalVertices.size(); i++)
	{
		Elite::FPoint4 pos{ m_Position.x + m_LocalVertices[i].Position.x
							, m_Position.y + m_LocalVertices[i].Position.y
							, m_Position.z + m_LocalVertices[i].Position.z
							, m_LocalVertices[i].Position.w };
		positions.push_back(pos);
	}

	return positions;
}

std::vector<Vertex_Input> Triangle::GetColoredVertices() const
{
	return m_LocalVertices;
}

void Triangle::SetLocalVertices(const std::vector<Vertex_Input>& newLocalVertices)
{
	m_LocalVertices = newLocalVertices;
}
