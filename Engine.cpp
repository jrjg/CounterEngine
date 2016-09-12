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
};

cd3d11* Engine_GetCD3D11()
{
	return gpEngine->_pCD3D11;
}

CStrike* Engine_GetCStrike()
{
	return gpEngine->_pCStrike;
}

EventManager* Engine_GetEM()
{
	return gpEngine->_pEM;
}

ProcessManager* Engine_GetPM()
{
	return gpEngine->_pPM;
}

WindowMgr* Engine_GetWindowMgr()
{
	return gpEngine->_pWindowMgr;
}

Timer* Engine_GetTimer()
{
	return gpEngine->_pTimer;
}

Controller* Engine_GetController()
{
	return gpEngine->_pController;
}

ComponentMgr* Engine_GetCmpMgr(void) {
	return gpEngine->_pCmpMgr;
}

ComponentManager* Engine_GetComponentManager(void) {
	return gpEngine->pCM;
}

ResourceManager* Engine_GetResourceManager(void) {
	return gpEngine->pRM;
}

//CameraManager * Engine_GetCameraManager(void)
//{
//	return gpEngine->_pCMGR;
//}

BOOL Engine_ShutDown()
{
	BOOL error = ERROR_FAILURE;
	ProcessManager_Delete();
	EventManager_Delete();
	Timer_Delete();
	Controller_Delete();
	WindowMgr_DELETE();
	//CameraManager_Delete();
	//SceneManager_DELETE();
	ResourceManager_DELETE();
	ComponentManager_DELETE();
	CStrike_DELETE();
	cd3d11_DELETE();
	return error;
}

BOOL Engine_StartUp(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	BOOL error = ERROR_FAILURE;
	if (gpEngine == NULL)
	{


		/*initializing*/
		 _NEW(Engine, gpEngine);
		if (!gpEngine) 
		{ 
			error = error || ERROR_SUCCESS;
			return error;
		}
		gpEngine->_tickdelay = 10;
		gpEngine->_uptime = 0;
		gpEngine->_terminate = FALSE;
		
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
		gpEngine->_pWindowMgr = WindowMgr_NEW();
		gpEngine->_pCStrike = CStrike_NEW();
		gpEngine->_pCD3D11 = cd3d11_NEW();
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

		EventManager_QueueEvent(EVENT_START,NULL);
	}
	return error;
}

WinParams* Engine_GetWinParams(void)
{
	return gpEngine->_pWinParams;
}

BOOL Engine_UpdateUpTime(TIME elapsed)
{
	gpEngine->_uptime = gpEngine->_uptime + elapsed;
	return ERROR_FAILURE;
}

TIME Engine_GetUpTime(void)
{
	return gpEngine->_uptime;
}

BOOL CHAREQ(char * a, char * b)
{
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

BOOL Engine_WaitTimer(TIME elapsed)
{
	TIME wait = gpEngine->_tickdelay - elapsed;
	if (wait < 0)
	{
		return ERROR_SUCCESS;
	}
	Timer_Wait(wait);
	return ERROR_FAILURE;
}

BOOL Engine_Run()
{
	BOOL error = ERROR_FAILURE;
	while (gpEngine->_terminate == FALSE)
	{
		ProcessManager_Run(Timer_Elapsed());
	}
	return error;
}

BOOL Engine_Terminate(void* pData)
{
	gpEngine->_terminate = TRUE;
	return ERROR_FAILURE;
}