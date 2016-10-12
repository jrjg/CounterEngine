#include "CLibrary.h"
#include "TopInclude.h"
#include "Engine.h"
#include "CoreComponent.h"

#include "Timer.h"

TIME Timer::getElapsed()
{
	TIME now = getTime();
	TIME elapsed = now - mThen;
	mThen = now;
	return elapsed;
}
