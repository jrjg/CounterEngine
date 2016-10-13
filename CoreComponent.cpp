#include "TopInclude.h"
#include "Engine.h"
#include "Memory.h"
#include "EventManager.h"
#include "ProcessManager.h"
#include "String.h"
#include "EventListener.h"
#include "Process.h"
#include "SimplyManaged.h"
#include "MemManaged.h"

#include "CoreComponent.h"

CoreComponent::~CoreComponent() { mpRunHandler->release(); mpRestoreListener->release(); mpReleaseListener->release(); };

HRESULT CoreComponent::restore()
{
	if (mpRestoreListener) { mpRestoreListener->release(); }; mpRestoreListener = new RestoreListener(this);
	if (mpReleaseListener) { mpReleaseListener->release(); }; mpReleaseListener = new ReleaseListener(this);
	if (mpRunHandler) { mpRunHandler->release(); }; mpRunHandler = new RunHandler(this);
	return S_OK;
};