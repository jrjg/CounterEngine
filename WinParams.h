#ifndef WINPARAMS_INC
#define WINPARAMS_INC

#include "TopInclude.h"
#include "MemManaged.h"

class WinParams :public MemManaged {
public:
	virtual ~WinParams() {};
	WinParams(HINSTANCE HInstance, HINSTANCE HPrevInstance, LPSTR pCmdLine, int CmdShow) : mHInstance(HInstance), mHPrevInstance(HPrevInstance), mpCmdLine(pCmdLine), mCmdShow(CmdShow) {};
	HINSTANCE mHInstance;
	HINSTANCE mHPrevInstance;
	LPSTR mpCmdLine;
	int mCmdShow;
};

#endif