#include "TopInclude.h"
#include "Memory.h"
#include "Engine.h"
#include "EventManager.h"
#include "SimplyManaged.h"
#include "MemManaged.h"

#include "Main.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Engine::get();

	EventManager::get()->queueEvent(EVENT_WINPARAMS, new WinParams(hInstance, hPrevInstance, lpCmdLine, nCmdShow));

	Engine::get()->run(0);

	return S_OK;
};