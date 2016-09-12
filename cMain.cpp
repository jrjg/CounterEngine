#include "TopInclude.h"
#include "Engine.h"
#include "cMain.h"



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{ 

	/* declare and initialize error variable*/
	BOOL error = ERROR_FAILURE;



	/* start up the engine*/
	error = error || Engine_StartUp(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	


	/* run the engine*/
	error = error || Engine_Run();



	/* shut down the engine*/
	error = error || Engine_ShutDown();



	/* return error code*/
	return error;

};

