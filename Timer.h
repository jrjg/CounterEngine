#ifndef INCLUDE_TIMER
#define INCLUDE_TIMER

#include "CoreComponent.h"

class Timer : public CoreComponent {
private:
	TIME mThen;
	Timer() {};
protected:
	virtual ~Timer() {};
public:
	TIME getElapsed();
	TIME getTime() { return (clock() * 1000 / CLOCKS_PER_SEC); };
	HRESULT wait(TIME time) { Sleep(time); return S_OK; };

	HRESULT restore() { srand(time(NULL)); mThen = getTime(); return S_OK; };
	HRESULT run(TIME elapsed)override { wait(CORETICKSPEED - elapsed); return S_OK; };

	static Timer* get();
};

#endif 