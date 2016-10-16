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

#include "Main.h"

bool gIsRunning;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MemoryManager::get();
	
	EventManager::get()->queueEvent(EVENT_WINPARAMS, new WinParams(hInstance, hPrevInstance, lpCmdLine, nCmdShow));

	bool isRunning = true;
	RunningListener* pRunningListener = new RunningListener(&isRunning);

	Controller::get();

	Timer::get();

	CStrike::get();

	bool allowInstancing = false;
	EventManager::get()->queueEvent(EVENT_ALLOWINSTANCING, new SimplyManaged<bool>(allowInstancing));

	EventManager::get()->queueEvent(EVENT_RELEASE, NULL);

	while (isRunning)
	{
		ProcessManager::get()->run(Timer::get()->getElapsed());
	}

	delete pRunningListener;

	EventManager::get()->ManualRelease();

	ProcessManager::get()->ManualRelease();

	MemoryManager::get()->manualRelease();

	return S_OK;
};