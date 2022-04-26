#include "PerspectiveCamera.h"
#include <iostream>
#include "SDL.h"

//ToDo: Yaw & Pitch

PerspectiveCamera::PerspectiveCamera(const FVector3& position, float fov, float screenWidth, float screenHeight)
	: m_LookAtMatrix{ FMatrix4{ {-1,0,0,0}, {0,1,0,0}, {0,0,-1,0}, {position,1} } }
	, m_Fov{ std::atanf(fov/2) }
	, m_AspectRatio{ screenWidth / screenHeight}

	, m_MovementSpeed{ 5.f }
	, m_RotateSpeed{ 90.f }

	, m_ScreenWidth{ screenWidth }
	, m_ScreenHeight{ screenHeight }

{
}

void PerspectiveCamera::Update(float elapsedSec)
{
	// Calculate matrices
	FVector3 cameraForward = { m_LookAtMatrix[2].x, m_LookAtMatrix[2].y, m_LookAtMatrix[2].z };
	FVector3 worldUp = { 0,1,0 };
	FVector3 cameraRight = Elite::GetNormalized(Elite::Cross(worldUp, cameraForward));
	m_LookAtMatrix[0] = { cameraRight.x, cameraRight.y, cameraRight.z, 0 };
	
	cameraRight = { m_LookAtMatrix[0].x, m_LookAtMatrix[0].y, m_LookAtMatrix[0].z };

	FVector3 cameraUp = Elite::GetNormalized(Elite::Cross(cameraForward, cameraRight));
	m_LookAtMatrix[1] = { cameraUp.x, cameraUp.y, cameraUp.z, 0};

	#pragma region KeyboardInput
	// Keyboard input
	const Uint8* state = SDL_GetKeyboardState(NULL);

	//fwd-bckw
	if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP])
	{
		m_LookAtMatrix[3] += Elite::GetNormalized(m_LookAtMatrix[2]) * elapsedSec * m_MovementSpeed;
	}
	else if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN])
	{
		m_LookAtMatrix[3] -= Elite::GetNormalized(m_LookAtMatrix[2]) * elapsedSec * m_MovementSpeed;
	}
	// Left-Right
	if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT])
	{
		m_LookAtMatrix[3] += Elite::GetNormalized(m_LookAtMatrix[0]) * elapsedSec * m_MovementSpeed;
	}
	else if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT])
	{
		m_LookAtMatrix[3] -= Elite::GetNormalized(m_LookAtMatrix[0]) * elapsedSec * m_MovementSpeed;
	}
	#pragma endregion KeyboardInput
	
	#pragma region MouseInput
	// Mouse input
	int xDiff, yDiff;
	Uint32 RelativeMouseState = SDL_GetRelativeMouseState(&xDiff, &yDiff);

	if ((RelativeMouseState & SDL_BUTTON_LMASK) && (RelativeMouseState & SDL_BUTTON_RMASK))
	{
		//up-down
		m_LookAtMatrix[3].y += -yDiff * elapsedSec * m_MovementSpeed;
	}
	else if (RelativeMouseState & SDL_BUTTON_LMASK) 
	{
		//fwd-bwd
		m_LookAtMatrix[3] += m_LookAtMatrix[2] * elapsedSec * m_MovementSpeed;

		//left-right
		float angleToRotate{ -Elite::ToRadians( xDiff * elapsedSec * m_RotateSpeed) };
		FMatrix3 rotationMatrix{
			{std::cosf(angleToRotate),0, std::sinf(angleToRotate)},
			{0,1,0},
			{-std::sinf(angleToRotate),0,std::cosf(angleToRotate)}
		};
		FVector3 fwdVector{ m_LookAtMatrix[2].x,m_LookAtMatrix[2].y,m_LookAtMatrix[2].z };
		FVector3 result = rotationMatrix * fwdVector;
		m_LookAtMatrix[2].x = result.x;
		m_LookAtMatrix[2].y = result.y;
		m_LookAtMatrix[2].z = result.z;
	}
	else if (RelativeMouseState & SDL_BUTTON_RMASK)
	{
		//Rotate in all directions

		//left-right
		float xAngle{ Elite::ToRadians(xDiff * elapsedSec * m_RotateSpeed) };
		FMatrix3 yRotationMatrix{ Elite::MakeRotation(xAngle,{0,1,0}) };
		FVector3 fwdVector{ m_LookAtMatrix[2].xyz };
		FVector3 xResult = yRotationMatrix * fwdVector;
		m_LookAtMatrix[2].x = xResult.x;
		m_LookAtMatrix[2].y = xResult.y;
		m_LookAtMatrix[2].z = xResult.z;

		//up-down
		float yAngle{ -Elite::ToRadians(-yDiff * elapsedSec * m_RotateSpeed) };
		
		fwdVector = m_LookAtMatrix[2].xyz;
		FVector3 rightVector = m_LookAtMatrix[0].xyz;
		FMatrix3 xRotationMatrix{ Elite::MakeRotation(yAngle, rightVector)};
		FVector3 yResult = xRotationMatrix * fwdVector;
		m_LookAtMatrix[2].x = yResult.x;
		m_LookAtMatrix[2].y = yResult.y;
		m_LookAtMatrix[2].z = yResult.z;
	}
	#pragma endregion MouseInput
}

void PerspectiveCamera::GenerateRay(Ray& ray, uint32_t c, uint32_t r)
{
	// World Space
	float worldSpaceX{ (2 * (c + 0.5f) / m_ScreenWidth) - 1 };
	worldSpaceX *= m_AspectRatio * m_Fov;
	float worldSpaceY{ (1 - (2 * (r + 0.5f) / m_ScreenHeight)) };
	worldSpaceY *= m_Fov;

	ray.origin = FPoint3{ m_LookAtMatrix[3].x, m_LookAtMatrix[3].y, m_LookAtMatrix[3].z };

	FVector4 pixelWorldPos{ worldSpaceX, worldSpaceY, 1.0f, 0.0f };
	FVector4 transformed{ m_LookAtMatrix * pixelWorldPos };

	ray.direction = Elite::GetNormalized(FVector3{ transformed[0], transformed[1], transformed[2]});
}