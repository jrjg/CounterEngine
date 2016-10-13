#include "TopInclude.h"
#include "EventManager.h"
#include "Memory.h"
#include "SimplyManaged.h"
#include "MemManaged.h"

#include "EventListener.h"

EventListener::EventListener(ID eventID) : mEventID(eventID) { EventManager::get()->registerForEvent(mEventID, this); }
EventListener::~EventListener() { EventManager::get()->unRegisterForEvent(mEventID, mListenerID); };
