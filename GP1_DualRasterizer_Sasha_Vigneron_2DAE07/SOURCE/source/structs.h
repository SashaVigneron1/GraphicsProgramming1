#pragma once

struct Vertex_Input
{
	Elite::FPoint4 Position{};
	Elite::FVector2 UV{};
	Elite::FVector3 Normal{};
	Elite::FVector3 Tangent{};
	Elite::FVector3 viewDirection{};
};

enum class TextureSamplingState
{
	POINT = 0,
	LINEAR = 1,
	ANISOTROPIC = 2,
	LASTELEMENT = 3
};

enum class CullMode
{
	BACKFACE = 0,
	FRONTFACE = 1,
	NONE = 2,
	LASTELEMENT = 3
};


enum class RenderMode
{
	HARDWARE = 0,
	SOFTWARE = 1,
	LASTELEMENT = 2
};