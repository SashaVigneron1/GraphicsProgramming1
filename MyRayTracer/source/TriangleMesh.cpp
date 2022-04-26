#include "TriangleMesh.h"

#include "BRDF.h"

#include <fstream>
#include <iostream>

TriangleMesh::TriangleMesh(const std::string& objFilePath, const Elite::FVector3& position, BRDF* pMaterial)
	: Geometry{ pMaterial }
	, m_Position{ position }
	, m_HitTriangleIndex{ 0 }
	, m_Triangle{ Elite::FPoint3{position}, Triangle::CullMode::NoCulling, m_pMaterial, false }
	, m_Angle{0}
{
	// Test Case
	/*m_VertexBuffer.push_back(FVector3{ -.75f, 1.5f, 0.f });
	m_VertexBuffer.push_back(FVector3{ -.75f, 0.f, 1.f });
	m_VertexBuffer.push_back(FVector3{ .75f, 0.f, 0.f });
	m_VertexBuffer.push_back(FVector3{ .75f, 1.5f, 0.f });
	m_IndexBuffer.push_back(IVector3{ 0,1,2 });
	m_IndexBuffer.push_back(IVector3{ 0,2,3 });*/

	Parse(objFilePath);
}

bool TriangleMesh::IsHit(const Ray& ray, HitRecord& hitPoint)
{
	HitRecord closestHit;
	closestHit.tValue = FLT_MAX;
	bool succes{ false };

	for (size_t i = 0; i < m_IndexBuffer.size(); i++)
	{
		std::vector<FVector3> positionOffset;
		positionOffset.push_back(m_VertexBuffer[m_IndexBuffer[i].x]);
		positionOffset.push_back(m_VertexBuffer[m_IndexBuffer[i].y]);
		positionOffset.push_back(m_VertexBuffer[m_IndexBuffer[i].z]);
		m_Triangle.SetPositionOffset(positionOffset);
		m_Triangle.SetAngle(m_Angle);

		//m_Triangle.PrintVertices();

		if (m_Triangle.IsHit(ray, hitPoint)) 
		{
			if (hitPoint.tValue < closestHit.tValue) 
			{
				closestHit = hitPoint;
				succes = true;
			}
		}

	}

	if (succes) 
	{
		hitPoint = closestHit;
		return true;
	}
	return false;
}

void TriangleMesh::Update(float totalElapsedTime)
{
	m_Angle = (std::cosf(totalElapsedTime) + 1) * (float)E_PI;
}

FVector3 TriangleMesh::GetNormal(const Elite::FPoint3& point)
{
	// Set triangle to this pos
	std::vector<FVector3> trianglePos;
	trianglePos.push_back(m_VertexBuffer[m_IndexBuffer[m_HitTriangleIndex].x]);
	trianglePos.push_back(m_VertexBuffer[m_IndexBuffer[m_HitTriangleIndex].y]);
	trianglePos.push_back(m_VertexBuffer[m_IndexBuffer[m_HitTriangleIndex].z]);
	m_Triangle.SetPositionOffset(trianglePos);
	m_Triangle.SetAngle(m_Angle);

	return m_Triangle.GetNormal(point);
}


void TriangleMesh::Parse(const std::string& objFilePath)
{
	std::ifstream inputStream;
	inputStream.open(objFilePath);

	while (!inputStream.eof())
	{
		std::string line;
		std::getline(inputStream, line);

		if (line.rfind("v", 0) == 0)
		{
			// Parse Vertices

			line = line.substr(3, line.size());

			FVector3 vertice{};
			std::string tempLine;
			int currAxis = 0;

			for (size_t i = 0; i < line.size(); i++)
			{

				if (line[i] == ' ' || i == line.size() - 1)
				{
					if (i == line.size() - 1) tempLine += line[i];

					switch (currAxis)
					{
					case 0:
						vertice.x = std::stof(tempLine);
						currAxis++;
						break;

					case 1:
						vertice.y = std::stof(tempLine);
						currAxis++;
						break;

					case 2:
						vertice.z = std::stof(tempLine);
						break;

					default:
						std::cerr << "[GeometryObj.cpp] Something went wrong while parsing. Line 51\n\n";
						break;
					}

					tempLine = "";
				}
				else
				{
					tempLine += line[i];
				}
			}

			m_VertexBuffer.push_back(vertice);
		}
		else if (line.rfind("f", 0) == 0)
		{
			// Parse Faces
			line = line.substr(2, line.size());

			std::string tempLine;
			int currAxis = 0;
			int xIndex = 0;
			int yIndex = 0;
			int zIndex = 0;

			for (size_t i = 0; i < line.size(); i++)
			{

				if (line[i] == ' ' || i == line.size() - 1)
				{
					if (i == line.size() - 1) tempLine += line[i];

					switch (currAxis)
					{
					case 0:
						xIndex = std::stoi(tempLine);
						currAxis++;
						break;

					case 1:
						yIndex = std::stoi(tempLine);
						currAxis++;
						break;

					case 2:
						zIndex = std::stoi(tempLine);
						break;

					default:
						std::cerr << "[GeometryObj.cpp] Something went wrong while parsing. Line 51\n\n";
						break;
					}

					tempLine = "";
				}
				else
				{
					tempLine += line[i];
				}
			}

			if (xIndex > 0 && yIndex > 0 && zIndex > 0)
			{
				m_IndexBuffer.push_back(IVector3{ xIndex-1, yIndex-1, zIndex-1 });
			}
		}



	}
	inputStream.close();

}