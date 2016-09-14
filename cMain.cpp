#include "TopInclude.h"
#include "Engine.h"
#include "cMain.h"



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{ 
	CEASSERT(hInstance  && lpCmdLine && "Windows broken");

	/* start up the engine*/
	SAFECALL(Engine_StartUp(hInstance, hPrevInstance, lpCmdLine, nCmdShow));
	
	/* run the engine*/
	SAFECALL(Engine_Run());

	/* shut down the engine*/
	SAFECALL(Engine_ShutDown());

	/* return error code*/
	return S_OK;
};

