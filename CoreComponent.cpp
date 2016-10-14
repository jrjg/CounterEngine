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

CoreComponent::~CoreComponent() { 
	SAFE_RELEASE(mpRunHandler);
	SAFE_RELEASE(mpRestoreListener);
	SAFE_RELEASE(mpReleaseListener);
}
HRESULT CoreComponent::restoreCoreListeners()
{
	SAFE_RELEASE(mpRestoreListener); mpRestoreListener = new RestoreListener(this);
	SAFE_RELEASE(mpReleaseListener); mpReleaseListener = new ReleaseListener(this);
	return S_OK;
}
HRESULT CoreComponent::restoreCoreProcesses()
{
	SAFE_RELEASE(mpRunHandler); mpRunHandler = new RunHandler(this);
	return S_OK;
};