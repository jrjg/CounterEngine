#include "TopInclude.h"
#include "EventManager.h"
#include "ProcessManager.h"
#include "Parser.h"
#include "Memory.h"
#include "String.h"
#include "CoreComponent.h"
#include "Timer.h"
#include "Controller.h"
#include "SimplyManaged.h"
#include "MemManaged.h"
#include "CStrike.h"

#include "Engine.h"

HRESULT Engine::restore()
{
	mRunning = true;
	EventManager::get();
	ProcessManager::get();
	Timer::get();
	Controller::get();
	CStrike::get();
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
