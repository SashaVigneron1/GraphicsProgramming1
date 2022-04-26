//External includes
#include "vld.h"
#include "SDL.h"
#include "SDL_surface.h"
#undef main

//Standard includes
#include <iostream>

//Project includes
#include "ETimer.h"
#include "ERenderer.h"

// My Includes
#include <thread>
#include "SceneManager.h"
#include "MaterialManager.h"
#include "LightManager.h"
#include "PerspectiveCamera.h"

#define MultiThreading_Enabled

void ShutDown(SDL_Window* pWindow)
{
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Unreferenced parameters
	(void)argc;
	(void)args;

	//Create window + surfaces
	SDL_Init(SDL_INIT_VIDEO);

	const uint32_t width = 640;
	const uint32_t height = 480;
	SDL_Window* pWindow = SDL_CreateWindow(
		"RayTracer - Vigneron Sasha",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width, height, 0);

	if (!pWindow)
		return 1;

	//Initialize "framework"
	Elite::Timer* pTimer = new Elite::Timer();
	Elite::Renderer* pRenderer = new Elite::Renderer(pWindow);

	MaterialManager* pMaterialManager = new MaterialManager{};
	SceneManager* pSceneManager = new SceneManager{ pMaterialManager };
	LightManager* pLightManager = new LightManager{};
	PerspectiveCamera* pCamera = new PerspectiveCamera{ FVector3{0.0f,3.0f,9.0f}, Elite::ToRadians(45.0f), width, height};

	int currScene = 0;

	//Start loop
	pTimer->Start();
	float printTimer = 0.f;
	bool isLooping = true;
	bool takeScreenshot = false;
	while (isLooping)
	{
		//--------- Get input events ---------
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				isLooping = false;
				break;
			case SDL_KEYUP:
				if (e.key.keysym.scancode == SDL_SCANCODE_X)
					takeScreenshot = true;
				pLightManager->ProcessKeyUpEvent(e.key);
				break;
			}
		}

		//--------- Render ---------

		// MultiThreading works fine in scene 0, but the way scene 1 was built is not optimized for multithreading, and will cause undefined behaviour
#ifdef MultiThreading_Enabled
		// MultiThreading
		const int nrThreads = std::thread::hardware_concurrency();
		const uint32_t heightStep{ height / nrThreads };

		std::vector<std::thread> threads;
		for (int i = 0; i < nrThreads; i++)
		{
			threads.push_back(std::thread(&Elite::Renderer::Render, pRenderer, pSceneManager->GetScene(currScene), pCamera, pLightManager,
				i * heightStep, (i + 1) * heightStep));
		}

		for (std::thread& t : threads)
		{
			if (t.joinable())
			{
				t.join();
			}
		}
#else
		pRenderer->Render(pSceneManager->GetScene(currScene), pCamera, pLightManager, 0, height);
#endif
	


		//--------- Timer ---------
		pTimer->Update();
		printTimer += pTimer->GetElapsed();
		if (printTimer >= 1.f)
		{
			printTimer = 0.f;
			std::cout << "FPS: " << pTimer->GetFPS() << std::endl;
		}

		// Update Objects
		pCamera->Update(pTimer->GetElapsed());
		pSceneManager->Update(currScene, pTimer->GetTotal());

		//Save screenshot after full render
		if (takeScreenshot)
		{
			if (!pRenderer->SaveBackbufferToImage())
				std::cout << "Screenshot saved!" << std::endl;
			else
				std::cout << "Something went wrong. Screenshot not saved!" << std::endl;
			takeScreenshot = false;
		}

	}
	pTimer->Stop();

	//Shutdown "framework"
	delete pCamera;
	delete pMaterialManager;
	delete pSceneManager;
	delete pLightManager;
	delete pRenderer;
	delete pTimer;

	ShutDown(pWindow);
	return 0;
}