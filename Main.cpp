#include "TopInclude.h"
#include "Memory.h"
#include "Engine.h"
#include "EventManager.h"
#include "SimplyManaged.h"
#include "MemManaged.h"
#include "ProcessManager.h"
#include "Timer.h"

#include "Main.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MemoryManager::get();

	Engine::get();

	EventManager::get()->queueEvent(EVENT_WINPARAMS, new WinParams(hInstance, hPrevInstance, lpCmdLine, nCmdShow));

	while (Engine::get()->isRunning())
	{
		ProcessManager::get()->run(Timer::get()->getElapsed());
	}

	Engine::get()->manualRelease();

	MemoryManager::get()->manualRelease();

	return S_OK;
};