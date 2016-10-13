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
};
