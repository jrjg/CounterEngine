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
//#include "SceneManager.h"
#include "Camera.h"
#include "Parser.h"
#include "Skybox.h"
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
	Camera* pCamera;
	Skybox* pSkybox;

	LPCWSTR WINDOWTITLE;
	float BUFFERWIDTH;
	float BUFFERHEIGHT;
	bool FULLSCREEN;
	Parser* pConfigParser;
};

LPCWSTR Engine_WINDOWTITLE() {
	CE1_ASSERT(gpEngine);
	return gpEngine->WINDOWTITLE;
}

float Engine_BUFFERWIDTH() {
	CE1_ASSERT(gpEngine);
	return gpEngine->BUFFERWIDTH;
}

float Engine_BUFFERHEIGHT() {
	CE1_ASSERT(gpEngine);
	return gpEngine->BUFFERHEIGHT;
}

BOOL Engine_FULLSCREEN() {
	CE1_ASSERT(gpEngine);
	return gpEngine->FULLSCREEN;
}

cd3d11* Engine_GetCD3D11()
{
	CE1_ASSERT(gpEngine);
	return gpEngine->_pCD3D11;
}

Skybox* Engine_GetSkybox()
{
	CE1_ASSERT(gpEngine);
	return gpEngine->pSkybox;
}

CStrike* Engine_GetCStrike()
{
	CE1_ASSERT(gpEngine);
	return gpEngine->_pCStrike;
}

Camera* Engine_GetCamera()
{
	CE1_ASSERT(gpEngine);
	return gpEngine->pCamera;
}

EventManager* Engine_GetEM()
{
	CE1_ASSERT(gpEngine);
	return gpEngine->_pEM;
}

ProcessManager* Engine_GetPM()
{
	CE1_ASSERT(gpEngine);
	return gpEngine->_pPM;
}

WindowMgr* Engine_GetWindowMgr()
{
	CE1_ASSERT(gpEngine);
	return gpEngine->_pWindowMgr;
}

Timer* Engine_GetTimer()
{
	CE1_ASSERT(gpEngine);
	return gpEngine->_pTimer;
}

Controller* Engine_GetController()
{
	CE1_ASSERT(gpEngine);
	return gpEngine->_pController;
}

ComponentMgr* Engine_GetCmpMgr(void) {
	CE1_ASSERT(gpEngine);
	return gpEngine->_pCmpMgr;
}

ComponentManager* Engine_GetComponentManager(void) {
	CE1_ASSERT(gpEngine);
	return gpEngine->pCM;
}

ResourceManager* Engine_GetResourceManager(void) {
	CE1_ASSERT(gpEngine);
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

#define CASE(NAME,EXEC)if (CE1_CMPSTR(pObjName->pBuffer, NAME, pObjName->length)) {EXEC;return S_OK;}

HRESULT Engine_ConfigHandler(void* p0,String* pObjName,void* pObj) {
	CASE("Fullscreen", gpEngine->FULLSCREEN = *(bool*)pObj);
	CASE("Windowtitle", gpEngine->WINDOWTITLE = *(LPCWSTR*)pObj);
	CASE("ResX", gpEngine->BUFFERWIDTH = *(float*)pObj);
	CASE("ResY", gpEngine->BUFFERHEIGHT = *(float*)pObj);
	CASE("Speed", gpEngine->_tickdelay = *(UINT*)pObj);
	CE1_ASSERT(0&&"unknown Object received");
}

HRESULT Engine_LoadConfig(void* p0) {
	if (!gpEngine->pConfigParser) {
		gpEngine->pConfigParser = Parser_New();
		CE1_CALL(Parser_DeclareVariable(gpEngine->pConfigParser, "Bool", "Fullscreen", &Engine_ConfigHandler));
		CE1_CALL(Parser_DeclareVariable(gpEngine->pConfigParser, "Float", "ResX", &Engine_ConfigHandler));
		CE1_CALL(Parser_DeclareVariable(gpEngine->pConfigParser, "Float", "ResY", &Engine_ConfigHandler));
		CE1_CALL(Parser_DeclareVariable(gpEngine->pConfigParser, "UINT", "Speed", &Engine_ConfigHandler));
		CE1_CALL(Parser_DeclareVariable(gpEngine->pConfigParser, "LPCWSTR", "Windowtitle", &Engine_ConfigHandler));

		CE1_CALL(Parser_RegisterOperator(gpEngine->pConfigParser, ";",OperatorCode::submit));
		CE1_CALL(Parser_RegisterOperator(gpEngine->pConfigParser, " ", OperatorCode::ignore));
		CE1_CALL(Parser_RegisterOperator(gpEngine->pConfigParser, "\n", OperatorCode::ignore));
	}
	CE1_CALL(Parser_ParseFile(gpEngine->pConfigParser, "Resource/Configuration/Config.txt", &Engine_ConfigHandler,NULL));
	CE1_CALL(Parser_Destroy(gpEngine->pConfigParser));
	return S_OK;
}

HRESULT Engine_StartUp(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CE1_ASSERT(hInstance&&lpCmdLine);
	if (gpEngine == NULL)
	{
		/*initializing*/
		 _NEW(Engine, gpEngine);
		 CE1_ASSERT(gpEngine);
		
		gpEngine->_tickdelay = 10;
		gpEngine->_uptime = 0;
		gpEngine->_terminate = FALSE;

		gpEngine->BUFFERWIDTH = 800;
		gpEngine->BUFFERHEIGHT = 600;
		gpEngine->FULLSCREEN = false;
		gpEngine->WINDOWTITLE = L"Counter Engine";

		CE1_CALL(Engine_LoadConfig(NULL));
		
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
		CE1_CALL(Camera_New(&gpEngine->pCamera));
		CE1_CALL(Skybox_New(&gpEngine->pSkybox));
		//gpEngine->_pCMGR = CameraManager_New();
		//gpEngine->_pSMGR = SceneManager_NEW();
		
		

		/*create processes*/
		ProcessManager_NewProcess(&Engine_WaitTimer,0);				//timer
		ProcessManager_NewProcess(&EventManager_Run, 10);			//eventmanager
		ProcessManager_NewProcess(&Engine_UpdateUpTime, 10);		//updating uptime mostly test process
		ProcessManager_NewProcess(&Controller_Run, 20);			//controller
		ProcessManager_NewProcess(&WindowMgr_Run, 20);				//WindowMgr
		ProcessManager_NewProcess(&CStrike_Run, 10);				//WindowMgr
		ProcessManager_NewProcess(&cd3d11_Run, (1000.0f)/(100.0f));					//cd3d11
		

		EventManager_RegisterForEvent(EVENT_END,&Engine_Terminate);

		EventManager_QueueEvent(EVENT_START,NULL);
	}
	return S_OK;
}

WinParams* Engine_GetWinParams(void)
{
	CE1_ASSERT(gpEngine);
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
	CE1_ASSERT(a&&b);
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

BOOL CE1_CMPSTR(char * a, char * b, size_t l)
{
	CE1_ASSERT(a&&b&&l);
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
	CE1_ASSERT(gpEngine);
	while (gpEngine->_terminate == FALSE)
	{
		CE1_CALL(ProcessManager_Run(Timer_Elapsed()));
	}
	return S_OK;
}

HRESULT Engine_Terminate(void* pData)
{
	CE1_ASSERT(gpEngine);
	gpEngine->_terminate = TRUE;
	return S_OK;
}