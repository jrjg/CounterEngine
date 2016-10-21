#include "TopInclude.h"
#include "CoreComponent.h"
#include "Vector.h"
#include "List.h"
#include "Memory.h"
#include "EventListener.h"
#include "Event.h"
#include "SimplyManaged.h"
#include "MemManaged.h"
#include "EventManager.h"

HRESULT EventManager::registerEvent(ID id) { return mpListeners->set(id, new List<EventListener>()); }

EventManager::EventManager() : CoreComponent(false),Singleton<EventManager>(false){
	mpListeners = new Vector<List<EventListener>>(100); 
	mpEvents = new List<Event>(); 
	mEventCounter = 0;
};
EventManager::~EventManager() { 
	SAFE_RELEASE(mpListeners); 
	SAFE_RELEASE(mpEvents); 
};

ID EventManager::queueEvent(ID id, MemManaged * pData) { return mpEvents->pushBack(new Event(pData, id, mEventCounter++)); };

ID EventManager::registerForEvent(ID id, EventListener* pListener)
{
	List<EventListener>* pListenerList = mpListeners->get(id);
	if (!pListenerList) {
		registerEvent(id);
		pListenerList = mpListeners->get(id);
	}
	return pListenerList->pushBack(pListener);
}

HRESULT EventManager::unRegisterForEvent(ID eventID, ID listenerID) { 
	if (mpListeners) {
		return mpListeners->get(eventID)->deleteByID(listenerID);
	}
	return S_OK;
}
	
HRESULT EventManager::removeEvent(ID id) { 
	if (mpEvents) {
		return mpEvents->deleteByID(id);
	};
	return S_OK;
}

HRESULT EventManager::run(TIME elapsed)
{
	List<EventListener>* pListenerList = 0;
	ListElement<EventListener>* pListElem = 0;
	Event* pEvent = 0; 

	while (mpEvents->getLength() > 0) {
		pEvent = mpEvents->popFirst(); 
		pListenerList = mpListeners->get(pEvent->getSlotID());
		if (pListenerList) {
			if (pListenerList->getLength() > 0) {
				for (pListElem = (ListElement<EventListener>*)pListenerList->iterator(); pListElem != NULL; pListElem = (ListElement<EventListener>*)pListElem->getNext())
				{
					pListElem->getObject()->run(pEvent->getData());
					if (!pListElem) {
						return S_OK;
					}
				}
			}
		}
		SAFE_RELEASE(pEvent);
		if (!mpEvents) {
			return S_OK;
		}
	}
	return S_OK;
};

HRESULT EventManager::restore()
{
	if (!mpListeners) { 
		mpListeners = new Vector<List<EventListener>>(100); 
	}
	else {
		mpListeners->restore();
	}
	if (!mpEvents) { 
		mpEvents = new List<Event>(); 
	}
	else {
		mpEvents->restore();
	}
	mEventCounter = 0;
	return S_OK;
}
void EventManager::Release()
{
	//mpEvents->restore(); //delete all remaining Events
	SAFE_RELEASE(mpEvents);
}

EventManager * EventManager::get()
{
	if (!mpInstance) {
		if (mAllowInstancing) {
			mpInstance = new EventManager();
			mpInstance->restoreCore();
			mpInstance->mpAllowInstancingListener = new AllowInstancingListener<EventManager>(mpInstance);
		}
		else {
			CE1_ASSERT(0 && "Not allowed to instance Singleton");
		}
	}
	return mpInstance;
}

