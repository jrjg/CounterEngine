#ifndef INCLUDE_ENGINE
#define INCLUDE_ENGINE

struct String {
	char* pBuffer;
	size_t length;
};
typedef struct String String;

struct WString {
	wchar_t* pBuffer;
	size_t length;
};
typedef struct WString WString;

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
struct Camera* Engine_GetCamera();
struct Skybox* Engine_GetSkybox();

HRESULT Engine_ShutDown(void);
HRESULT Engine_Run(void);
HRESULT Engine_StartUp(HINSTANCE, HINSTANCE, LPSTR, int);
HRESULT Engine_WaitTimer(TIME);
HRESULT Engine_Terminate(void*);
HRESULT Engine_UpdateUpTime(TIME);
TIME Engine_GetUpTime(void);
LPCWSTR Engine_WINDOWTITLE();
float Engine_BUFFERWIDTH();
float Engine_BUFFERHEIGHT();
BOOL Engine_FULLSCREEN();
HRESULT Engine_LoadConfig(void*);
HRESULT Engine_ConfigHandler(void* p0, String* pObjName, void* pObj);

BOOL CHAREQ(char * a, char * b);
BOOL CE1_CMPCHARBYL(char * a, char * b, size_t l);
BOOL CE1_CompareStrings(String* pS1, String* pS2);
BOOL CE1_CompareWStrings(WString* pS1, WString* pS2);

#endif