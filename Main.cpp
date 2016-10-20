#include "TopInclude.h"
#include "Memory.h"
#include "EventManager.h"
#include "SimplyManaged.h"
#include "MemManaged.h"
#include "ProcessManager.h"
#include "Timer.h"
#include "EventListener.h"
#include "CStrike.h"
#include "Controller.h"
#include "WinParams.h"
#include "WindowManager.h"

#include "Main.h"

bool gIsRunning;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MemoryManager::get();

	bool isRunning = true;
	RunningListener* pRunningListener = new RunningListener(&isRunning);

	Controller::get();

	Timer::get();

	WindowManager::get();

	CStrike::get();

	bool allowInstancing = false;
	EventManager::get()->queueEvent(EVENT_ALLOWINSTANCING, new SimplyManaged<bool>(allowInstancing));

	EventManager::get()->queueEvent(EVENT_WINPARAMS, new WinParams(hInstance, hPrevInstance, lpCmdLine, nCmdShow));

	EventManager::get()->queueEvent(EVENT_GAMEINIT, NULL);

	while (isRunning)
	{
		ProcessManager::get()->run(Timer::get()->getElapsed());
	}

	SAFE_RELEASE(pRunningListener);

	EventManager::get()->ManualRelease();

	ProcessManager::get()->ManualRelease();

	MemoryManager::get()->manualRelease();

	return S_OK;
};