#include "TopInclude.h"
#include "Engine.h"
#include "Memory.h"
#include "EventManager.h"
#include "ProcessManager.h"
#include "String.h"

#include "CoreComponent.h"

HRESULT CoreComponent::handleEvent(ID eventID, void * pData)
{
	switch (eventID) {
	case EVENT_RESTORE:
		restore();
		break;
	case EVENT_RELEASE:
		release();
		break;
	}
	return S_OK;
}

CoreComponent::CoreComponent(Engine* pEngine) : mpEngine(pEngine) {
	mpEngine->getEventManager()->registerForEvent(EVENT_RESTORE, (EventListener*)this); 
	mpEngine->getEventManager()->registerForEvent(EVENT_RELEASE, (EventListener*)this);
	mpEngine->getProcessManager()->newProcess((ProcessOwner*)this,CORETICKSPEED);
}