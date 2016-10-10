#include "TopInclude.h"
#include "EventManager.h"
#include "ProcessManager.h"
#include "Timer.h"
#include "Parser.h"
#include "Memory.h"
#include "String.h"
#include "CoreComponent.h"
#include "windowmgr.h"
#include "Controller.h"

#include "Engine.h"

MemoryManager* gpMemoryManager;

MemoryManager* getMemoryManager() {
	return gpMemoryManager;
}

Engine::Engine() : mRunning(true),mTickSpeed(CORETICKSPEED)
{
	mpEngine = this;
	mpEventManager = new EventManager();
	mpProcessManager = new ProcessManager();
	mpWindowManager = new WindowManager();
	mpTimer = new Timer();
	mpController = new Controller();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	gpMemoryManager = new MemoryManager();
	Engine* pEngine = new Engine();
	WinParams* pWinParams = new WinParams(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	pEngine->getEventManager()->queueEvent(EVENT_UPDATE_WINPARAMS, pWinParams,false);
	pEngine->getEventManager()->queueEvent(EVENT_RESTORE, NULL, false);
	while (pEngine->isRunning())
	{
		pEngine->getProcessManager()->handleProcess(pEngine->getTimer()->getElapsed());
	}

	delete pWinParams;
	delete pEngine;
	delete gpMemoryManager;
	return S_OK;
};

HRESULT Engine::restore(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (!get()->mpWinParams) get()->mpWinParams = new WinParams(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	if (!get()->mpExtra) get()->mpExtra = new EngineExtra(L"Counter Engine", 800, 600, false, 10);
	get()->loadExtra(NULL);
	if (!get()->mProcess_WaitTimer) get()->mProcess_WaitTimer = ProcessManager::addProcess(&waitTimer, 0);
	get()->mRunning = true;
}

HRESULT Engine::handleProcess(TIME elapsed)
{
	mpTimer->wait(mTickSpeed - elapsed);
	return S_OK;
}

//#define CASE(Name,Exec)if (*pObjName==Name) {Exec; return S_OK; }
//
//HRESULT Engine::readExtra(void* p0, String<char>* pObjName, void* pObj) {
//	CASE("Fullscreen", get()->mpExtra->mFullscreen = *(bool*)pObj);
//	CASE("Windowtitle", get()->mpExtra->mWindowTitle = *(LPCWSTR*)pObj);
//	CASE("ResX", get()->mpExtra->mBufferWidth = *(float*)pObj);
//	CASE("ResY", get()->mpExtra->mBufferHeight = *(float*)pObj);
//	CASE("Speed", get()->mpExtra->mTickDelay = *(UINT*)pObj);
//	CE1_ASSERT(0 && "unknown Object received");
//	return ERROR_SUCCESS;
//}

//HRESULT Engine::loadExtra(void* p0) {
//	if (!get()->mpConfigParser) {
//		get()->mpConfigParser = new Parser();
//		get()->mpConfigParser->declareVariable("Bool", "Fullscreen", &Engine::readExtra);
//		get()->mpConfigParser->declareVariable("Float", "ResX", &Engine::readExtra);
//		get()->mpConfigParser->declareVariable("Float", "ResY", &Engine::readExtra);
//		get()->mpConfigParser->declareVariable("UINT", "Speed", &Engine::readExtra);
//		get()->mpConfigParser->declareVariable("LPCWSTR", "Windowtitle", &Engine::readExtra);
//
//		get()->mpConfigParser->registerOperator(";", OperatorCode::submit);
//		get()->mpConfigParser->registerOperator(" ", OperatorCode::ignore);
//		get()->mpConfigParser->registerOperator("\n", OperatorCode::ignore);
//	}
//	get()->mpConfigParser->parseFile("Resource/Configuration/Config.txt", &Engine::readExtra, NULL);
//	delete get()->mpConfigParser;
//	return S_OK;
//}
