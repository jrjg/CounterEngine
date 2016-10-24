#include "CLibrary.h"
#include "TopInclude.h"
#include "Engine.h"
#include "CoreComponent.h"
#include "SimplyManaged.h"
#include "MemManaged.h"

#include "Timer.h"

TIME Timer::getElapsed()
{
	TIME now = getTime();
	TIME elapsed = now - mThen;
	mThen = now;
	return elapsed;
}

TIME Timer::getTime(){ 
	return (clock() * 1000 / CLOCKS_PER_SEC); 
};

HRESULT Timer::wait(TIME time) { 
	if (time > 0) { 
		Sleep((DWORD)time); 
	}; 
	return S_OK; 
};

HRESULT Timer::restore() { 
	srand(time((time_t)NULL)); 
	mThen = getTime(); 
	return S_OK; 
}
HRESULT Timer::run(TIME elapsed) {
	wait(CORETICKSPEED - elapsed); 
	return S_OK;
};
