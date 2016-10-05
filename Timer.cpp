#include "CLibrary.h"
#include "TopInclude.h"
#include "Engine.h"
#include "Timer.h"

struct Timer
{
	TIME _last;
};

TIME Timer_Elapsed()
{
	Timer* pTimer = Engine_GetTimer();
	TIME now = Timer_Time();
	TIME elapsed = now - pTimer->_last;
	pTimer->_last = now;
	return elapsed;
}

TIME Timer_Time()
{
	return (clock() * 1000 / CLOCKS_PER_SEC);
}

HRESULT Timer_Wait(TIME time)
{
	CE1_ASSERT(time);
	Sleep(time);
	return S_OK;
}

HRESULT Timer_Delete()
{
	Timer* pTimer = Engine_GetTimer();
	CE1_DEL(pTimer);
	return S_OK;
}

Timer* Timer_New()
{

	srand(time(NULL));

	Timer* pTimer;
	_NEW(Timer, pTimer);
	CE1_ASSERT(pTimer);
	pTimer->_last = Timer_Time();
	return pTimer;
}