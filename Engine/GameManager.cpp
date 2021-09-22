#include "GameManager.h"
#include "WindowManager.h"
#include "Logging/Logging.h"
#include "Rendering/RenderingSystems.h"
#include <chrono>
#include "SDL2/SDL.h"
#include "ECS/flecs.h"
extern flecs::world ecs;

float GameManager::FPS = 0;
float GameManager::GameTime = 0;
int GameManager::Frame = 0;
static flecs::world ecs;
void GameManager::MainGameLoop()
{
	LOGVERBOSE("GameManager::MainGameLoop()", "Main game loop started");

	auto rendererEntity = ecs.entity("BasicRenderer")
		.add<BasicRenderer>();
	auto basicRenderer = rendererEntity.get_mut<BasicRenderer>();
	BasicRendererSystem::Initialize(basicRenderer);
	auto sys = ecs.system<BasicRenderer>()
		.kind(flecs::PreUpdate)
		.each(BasicRendererSystem::Render);


	EventTick();
	ManagerTick();
	SDL_ShowWindow(WindowManager::GetSDLWindow());
	SDL_GL_SetSwapInterval(-1);
	while (bMainLoopRunning)
	{
		EventTick();
		ManagerTick();
	}
}

int GameManager::GetGameFPS()
{
	return (int)FPS;
}

void GameManager::EventTick()
{
	SDL_Event Event;
	while (SDL_PollEvent(&Event))
	{
		switch (Event.type)
		{
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				break;
		default:
			break;
		}
	}
}

void GameManager::ManagerTick()
{

	static long long OldTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	long long Time = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	long long TimeDiff = Time - OldTime;
	OldTime = Time;

	float DeltaTime = (float)TimeDiff / 1000000000.f;
	ecs.frame_begin(DeltaTime);
	ecs.progress();
	FPS = 1.f / DeltaTime;
	GameTime += DeltaTime;
	Frame++;
	ecs.frame_end();


}

int GameManager::GetFrame()
{
	return Frame;
}

float GameManager::GetFPS()
{
	return FPS;
}

float GameManager::GetTime()
{
	return GameTime;
}