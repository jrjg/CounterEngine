#include "TopInclude.h"
#include "EventManager.h"
#include "ProcessManager.h"
#include "Timer.h"
#include "Controller.h"
#include "windowmgr.h"
#include "CStrike.h"
#include "directx.h"
#include "ComponentManager.h"
#include "Resource.h"
//#include "ComponentMgr.h"
//#include "CameraManager.h"
//#include "SceneManager.h"
#include "Parser.h"
#include "Engine.h"


Engine* gpEngine;

struct Engine
{
	ProcessManager* _pPM;
	EventManager* _pEM;
	Timer* _pTimer;
	Controller* _pController;
	WindowMgr* _pWindowMgr;
	ComponentMgr* _pCmpMgr;
	//CameraManager* _pCMGR;
	//SceneManager* _pSMGR;
	cd3d11* _pCD3D11;
	CStrike* _pCStrike;
	void* _pGraphics;
	TIME _tickdelay;
	TIME _uptime;
	BOOL _terminate;
	WinParams* _pWinParams;
	ComponentManager* pCM;
	ResourceManager* pRM;

	LPWSTR WINDOWTITLE;
	float BUFFERWIDTH;
	float BUFFERHEIGHT;
	bool FULLSCREEN;
	Parser* pConfigParser;
};

LPWSTR Engine_WINDOWTITLE() {
	CEASSERT(gpEngine);
	return gpEngine->WINDOWTITLE;
}

float Engine_BUFFERWIDTH() {
	CEASSERT(gpEngine);
	return gpEngine->BUFFERWIDTH;
}

float Engine_BUFFERHEIGHT() {
	CEASSERT(gpEngine);
	return gpEngine->BUFFERHEIGHT;
}

BOOL Engine_FULLSCREEN() {
	CEASSERT(gpEngine);
	return gpEngine->FULLSCREEN;
}

cd3d11* Engine_GetCD3D11()
{
	CEASSERT(gpEngine);
	return gpEngine->_pCD3D11;
}

CStrike* Engine_GetCStrike()
{
	CEASSERT(gpEngine);
	return gpEngine->_pCStrike;
}

EventManager* Engine_GetEM()
{
	CEASSERT(gpEngine);
	return gpEngine->_pEM;
}

ProcessManager* Engine_GetPM()
{
	CEASSERT(gpEngine);
	return gpEngine->_pPM;
}

WindowMgr* Engine_GetWindowMgr()
{
	CEASSERT(gpEngine);
	return gpEngine->_pWindowMgr;
}

Timer* Engine_GetTimer()
{
	CEASSERT(gpEngine);
	return gpEngine->_pTimer;
}

Controller* Engine_GetController()
{
	CEASSERT(gpEngine);
	return gpEngine->_pController;
}

ComponentMgr* Engine_GetCmpMgr(void) {
	CEASSERT(gpEngine);
	return gpEngine->_pCmpMgr;
}

ComponentManager* Engine_GetComponentManager(void) {
	CEASSERT(gpEngine);
	return gpEngine->pCM;
}

ResourceManager* Engine_GetResourceManager(void) {
	CEASSERT(gpEngine);
	return gpEngine->pRM;
}

//CameraManager * Engine_GetCameraManager(void)
//{
//	return gpEngine->_pCMGR;
//}

HRESULT Engine_ShutDown()
{
	CE1_CALL(ProcessManager_Delete());
	CE1_CALL(EventManager_Delete());
	CE1_CALL(Timer_Delete());
	CE1_CALL(Controller_Delete());
	CE1_CALL(WindowMgr_DELETE());
	CE1_CALL(ResourceManager_DELETE());
	CE1_CALL(ComponentManager_DELETE());
	CE1_CALL(CStrike_DELETE());
	CE1_CALL(cd3d11_DELETE());
	return S_OK;
}

HRESULT Engine_ConfigHandler(void* p0,String* pObjName,void* pObj) {

}

HRESULT Engine_LoadConfig() {
	String* pString;
	if (!gpEngine->pConfigParser) {
		gpEngine->pConfigParser = Parser_New();
		CE1_STR(pString, "Fullscreen");
		CE1_CALL(Parser_registerType(gpEngine->pConfigParser, pString,sizeof(bool),&Engine_ConfigHandler));
		
	}
	CE1_STR(pString, "Config.txt");
	Parser_ParseFromFile(gpEngine->pConfigParser, pString, &Engine_ConfigHandler);
	return S_OK;
}



HRESULT Engine_StartUp(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CEASSERT(hInstance&&lpCmdLine);
	if (gpEngine == NULL)
	{
		/*initializing*/
		 _NEW(Engine, gpEngine);
		 CEASSERT(gpEngine);
		
		gpEngine->_tickdelay = 10;
		gpEngine->_uptime = 0;
		gpEngine->_terminate = FALSE;

		gpEngine->BUFFERWIDTH = 800;
		gpEngine->BUFFERHEIGHT = 600;
		gpEngine->FULLSCREEN = false;
		gpEngine->WINDOWTITLE = L"Counter Engine";
		
		_NEW(WinParams, gpEngine->_pWinParams);
		if (gpEngine->_pWinParams) {
			gpEngine->_pWinParams->hInstance = hInstance;
			gpEngine->_pWinParams->hPrevInstance = hPrevInstance;
			gpEngine->_pWinParams->lpCmdLine = lpCmdLine;
			gpEngine->_pWinParams->nCmdShow = nCmdShow;
		}

		/*adding core systems*/
		gpEngine->pCM = ComponentManager_NEW();
		gpEngine->pRM = ResourceManager_NEW();
		gpEngine->_pPM = ProcessManager_New();
		gpEngine->_pEM = EventManager_New();
		gpEngine->_pTimer = Timer_New();
		gpEngine->_pController = Controller_New();
		CE1_CALL(WindowMgr_NEW(&gpEngine->_pWindowMgr));
		gpEngine->_pCStrike = CStrike_NEW();
		CE1_CALL(cd3d11_NEW(&gpEngine->_pCD3D11));
		//gpEngine->_pCMGR = CameraManager_New();
		//gpEngine->_pSMGR = SceneManager_NEW();
		
		

		/*create processes*/
		ProcessManager_NewProcess(&Engine_WaitTimer,0);				//timer
		ProcessManager_NewProcess(&EventManager_Run, 10);			//eventmanager
		ProcessManager_NewProcess(&Engine_UpdateUpTime, 10);		//updating uptime mostly test process
		ProcessManager_NewProcess(&Controller_Run, 100);			//controller
		ProcessManager_NewProcess(&WindowMgr_Run, 20);				//WindowMgr
		ProcessManager_NewProcess(&CStrike_Run, 10);				//WindowMgr
		ProcessManager_NewProcess(&cd3d11_Run, (1000.0f)/(100.0f));					//cd3d11
		

		EventManager_RegisterForEvent(EVENT_END,&Engine_Terminate);
		//EventManager_RegisterForEvent(EVENT_START, &Engine_LoadConfig);

		EventManager_QueueEvent(EVENT_START,NULL);
	}
	return S_OK;
}

WinParams* Engine_GetWinParams(void)
{
	CEASSERT(gpEngine);
	return gpEngine->_pWinParams;
}

HRESULT Engine_UpdateUpTime(TIME elapsed)
{
	gpEngine->_uptime = gpEngine->_uptime + elapsed;
	return S_OK;
}

TIME Engine_GetUpTime(void)
{
	return gpEngine->_uptime;
}

BOOL CHAREQ(char * a, char * b)
{
	CEASSERT(a&&b);
	size_t lena = strlen(a);
	size_t lenb = strlen(b);
	if (lena != lenb) {
		return false;
	}
	for (UINT n = 0; n < lena; n++) {
		if (a[n] != b[n]) { return false; }
	}
	return true;
}

BOOL CHAREQS(char * a, char * b, size_t l)
{
	CEASSERT(a&&b&&l);
	for (size_t n = 0; n < l; n++) {
		if (a[n] != b[n]) { return false; }
	}
	return true;
}

HRESULT Engine_WaitTimer(TIME elapsed)
{
	TIME wait = gpEngine->_tickdelay - elapsed;
	if (wait < 0) {
		return S_OK;
	}
	CE1_CALL(Timer_Wait(wait));
	return S_OK;
}

HRESULT Engine_Run()
{
	CEASSERT(gpEngine);
	while (gpEngine->_terminate == FALSE)
	{
		CE1_CALL(ProcessManager_Run(Timer_Elapsed()));
	}
	return S_OK;
}

HRESULT Engine_Terminate(void* pData)
{
	CEASSERT(gpEngine);
	gpEngine->_terminate = TRUE;
	return S_OK;
}