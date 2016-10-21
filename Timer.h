#ifndef INCLUDE_TIMER
#define INCLUDE_TIMER

#include "CoreComponent.h"
#include "Singleton.h"

class Timer : public CoreComponent, public Singleton<Timer>{
	friend class Singleton<Timer>;
private:
	TIME mThen;
	Timer() { restore(); };
protected:
	virtual ~Timer() {};
public:
	TIME getElapsed();
	TIME getTime() { return (clock() * 1000 / CLOCKS_PER_SEC); };
	HRESULT wait(TIME time) { if (time > 0) { Sleep((DWORD)time); }; return S_OK; };

	HRESULT restore() { srand(time((time_t)NULL)); mThen = getTime(); return S_OK; };
	HRESULT run(TIME elapsed)override { 
		wait(CORETICKSPEED - elapsed); return S_OK; 
	};
};

#endif 