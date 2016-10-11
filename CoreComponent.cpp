#include "TopInclude.h"
#include "Engine.h"
#include "Memory.h"
#include "EventManager.h"
#include "ProcessManager.h"
#include "String.h"
#include "EventListener.h"

#include "CoreComponent.h"

CoreComponent::CoreComponent(Engine* pEngine) : mpEngine(pEngine) {
	
	restore();
}

HRESULT CoreComponent::restore()
{
	if (!mpCoreListener) { mpCoreListener = new CoreListener(this); };
	if (!processHandlerID) { processHandlerID = mpEngine->getProcessManager()->newProcess((ProcessOwner*)this, CORETICKSPEED); };
	return S_OK;
}

HRESULT CoreListener::handleEvent(ID eventID, MemManaged * pData)
{
	switch (eventID) {
		case EVENT_RESTORE:
			mpCoreComponent->restore();
			break;
		case EVENT_RELEASE:
			mpCoreComponent->release();
			break;
	}
	return S_OK;
}

CoreListener::CoreListener(CoreComponent * pCoreComponent) : mpCoreComponent(pCoreComponent) {
	mpCoreComponent->getEngine()->getEventManager()->registerForEvent(EVENT_RESTORE, (EventListener*)this);
	mpCoreComponent->getEngine()->getEventManager()->registerForEvent(EVENT_RELEASE, (EventListener*)this);
};