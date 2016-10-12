#include "TopInclude.h"
#include "Engine.h"
#include "Memory.h"
#include "EventManager.h"
#include "ProcessManager.h"
#include "String.h"
#include "EventListener.h"
#include "Process.h"

#include "CoreComponent.h"

HRESULT CoreComponent::restore()
{
	delete mpRestoreListener; mpRestoreListener = new RestoreListener(this);
	delete mpReleaseListener; mpReleaseListener = new ReleaseListener(this);
	delete mpRunHandler; mpRunHandler = new RunHandler(this);
	return S_OK;
};