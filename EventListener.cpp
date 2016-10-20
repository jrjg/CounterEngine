#include "TopInclude.h"
#include "EventManager.h"
#include "Memory.h"
#include "SimplyManaged.h"
#include "MemManaged.h"

#include "EventListener.h"

EventListener::EventListener(ID eventID) : mEventID(eventID),mEnabled(true) { 
	EventManager::get()->registerForEvent(mEventID, this); 
}

EventListener::~EventListener() { 
	EventManager* pEM = EventManager::get();
	if (pEM) { 
		pEM->unRegisterForEvent(mEventID, mListenerID); 
	}
};
