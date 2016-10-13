#ifndef MAIN_INC
#define MAIN_INC

#include "MemManaged.h"

class WinParams :public MemManaged {
protected:
	virtual ~WinParams() {};
public:
	WinParams(HINSTANCE HInstance, HINSTANCE HPrevInstance, LPSTR pCmdLine, int CmdShow) : mHInstance(HInstance), mHPrevInstance(HPrevInstance), mpCmdLine(pCmdLine), mCmdShow(CmdShow) {};
	HINSTANCE mHInstance;
	HINSTANCE mHPrevInstance;
	LPSTR mpCmdLine;
	int mCmdShow;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

#endif