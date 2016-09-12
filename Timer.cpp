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

BOOL Timer_Wait(TIME time)
{
	Sleep(time);
	return ERROR_FAILURE;
}

BOOL Timer_Delete()
{
	Timer* pTimer = Engine_GetTimer();
	_DEL(pTimer);
	return ERROR_FAILURE;
}

Timer* Timer_New()
{
	Timer* pTimer;
	_NEW(Timer, pTimer);
	if (!pTimer)
	{
		return NULL;
	}
	pTimer->_last = Timer_Time();
	return pTimer;
}