#include "TopInclude.h"
#include "EventManager.h"
#include "Memory.h"
#include "SimplyManaged.h"
#include "MemManaged.h"

#include "EventListener.h"

EventListener::EventListener(ID eventID) : mEventID(eventID),mEnabled(true) { 
	EventManager* pEventManager = EventManager::get();
	if (pEventManager) {
		pEventManager->registerForEvent(mEventID, this);
	}
}

EventListener::~EventListener() { 
	EventManager* pEventManager = EventManager::get();
	if (pEventManager) {
		pEventManager->unRegisterForEvent(mEventID, mListenerID);
	}
};
