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

EventManager::EventManager() : CoreComponent(false),Singleton<EventManager>(false), mEventListKey(0){
	mpListeners = new Vector<List<EventListener>>(100); 
	mpEvents = new List<Event>(); 
	mEventCounter = 0;
};
EventManager::~EventManager() { 
	SAFE_RELEASE(mpListeners); 
	SAFE_RELEASE(mpEvents); 
};

ID EventManager::queueEvent(ID id, MemManaged * pData) { 
	mpEvents->pushBack(new Event(pData, id, mEventCounter++))
	return ; 
}
ID EventManager::queueEventND(ID id, MemManaged * pData)
{
	return ID();
};

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
	UINT length = 0;
	MemManaged* pMemManaged;
	EventListener* pListener;
	ID listenerVectorKey;
	while (true) {
		V_RETURN(mpEvents->unlock(0));
		length = 0;
		V_RETURN(mpEvents->getLength(&length));
		if (length == 0) { return S_OK; }
		pEvent = 0;
		V_RETURN(mpEvents->popFirst(&pEvent));
		mEventListKey = 0;
		V_RETURN(mpEvents->lock(&mEventListKey));

		V_RETURN(mpListeners->unlock(0));
		pListenerList = 0;
		mpListeners->get(pEvent->getSlotID(),&pListenerList);
		listenerVectorKey = 0;
		V_RETURN(mpListeners->lock(&listenerVectorKey));
		::operator->()->unlock(0);
		if (pListenerList) {
			if (SUCCEEDED(pListenerList->unlock(0))) {
				pListenerList->iterator(&pListElem);
				while (pListElem){
					pListElem->getObject(&pListener);
					pListener->run(pEvent->getData());
					pListElem->getNext((void**)&pListElem);
				}
			}
			
		}
		SAFE_RELEASE(pEvent);
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
	mEventListKey = 0;
	return S_OK;
}
void EventManager::Release()
{
	mpEvents->restore();
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

