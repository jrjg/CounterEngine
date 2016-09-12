#ifndef INCLUDE_ENGINE
#define INCLUDE_ENGINE


struct Engine;
struct WinParams {
	HINSTANCE hInstance;
	HINSTANCE hPrevInstance;
	LPSTR lpCmdLine;
	int nCmdShow;
};
typedef struct Engine Engine;
typedef struct WinParams WinParams;
extern Engine* gpEngine;

struct ProcessManager* Engine_GetPM(void);
struct EventManager* Engine_GetEM(void);
struct Timer* Engine_GetTimer(void);
struct Controller* Engine_GetController(void);
struct WinParams* Engine_GetWinParams(void);
struct WindowMgr* Engine_GetWindowMgr(void);
struct cd3d11* Engine_GetCD3D11(void);
struct CStrike * Engine_GetCStrike(void);
struct ComponentMgr* Engine_GetCmpMgr(void);
struct CameraManager* Engine_GetCameraManager(void);
struct ResourceManager* Engine_GetResourceManager(void);
struct ComponentManager* Engine_GetComponentManager(void);

BOOL Engine_ShutDown(void);
BOOL Engine_Run(void);
BOOL Engine_StartUp(HINSTANCE, HINSTANCE, LPSTR, int);
BOOL Engine_WaitTimer(TIME);
BOOL Engine_Terminate(void*);
BOOL Engine_UpdateUpTime(TIME);
TIME Engine_GetUpTime(void);

BOOL CHAREQ(char * a, char * b);

#endif