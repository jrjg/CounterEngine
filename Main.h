#ifndef MAIN_INC
#define MAIN_INC

class WinParams :public MemManaged {
public:
	WinParams(HINSTANCE HInstance, HINSTANCE HPrevInstance, LPSTR pCmdLine, int CmdShow) : mHInstance(HInstance), mHPrevInstance(HPrevInstance), mpCmdLine(pCmdLine), mCmdShow(CmdShow) {};
	HINSTANCE mHInstance;
	HINSTANCE mHPrevInstance;
	LPSTR mpCmdLine;
	int mCmdShow;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

#endif