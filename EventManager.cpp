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

HRESULT EventManager::registerEvent(ID id) { 
	return mpListeners->set(id, new List<EventListener>()); 
}

EventManager::EventManager() : CoreComponent(false),Singleton<EventManager>(false){
	mpListeners = new Vector<List<EventListener>>(100); 
	mpEvents = new List<Event>(); 
	mpEventsTemp = new List<Event>();
	mEventCounter = 0;
};
EventManager::~EventManager() { 
	SAFE_RELEASE(mpListeners); 
	SAFE_RELEASE(mpEvents); 
	SAFE_RELEASE(mpEventsTemp);
}

HRESULT EventManager::queueEvent(Event * pEvent)
{
	ID id;
	if (SUCCEEDED(mpEvents->unlock(0))) {
		return mpEvents->pushBack(pEvent, &id);
	}
	else {
		if (SUCCEEDED(mpEventsTemp->unlock(0))) {
			return mpEventsTemp->pushBack(pEvent, &id);
		}
	}
	return ERROR_SUCCESS;
}
HRESULT EventManager::transferTempEvents()
{
	V_RETURN(mpEventsTemp->unlock(0));
	V_RETURN(mpEvents->unlock(0));
	Event* pEvent;
	ID id;
	while (mpEventsTemp->getLength() > 0) {
		pEvent = 0;
		if (SUCCEEDED(mpEventsTemp->popFirst(&pEvent))) {
			if (FAILED(mpEvents->pushBack(pEvent, &id))) {
				delete pEvent;
			}
		}
	}
	return S_OK;
};

HRESULT EventManager::queueEvent(ID id, MemManaged * pData) { 
	Event* pEvent = new Event(pData, id, mEventCounter++);
	if (FAILED(queueEvent(pEvent))) {
		delete pEvent;
		return ERROR_SUCCESS;
	}
	return S_OK;
}

HRESULT EventManager::queueEventND(ID id, MemManaged * pData)
{
	Event* pEvent = new Event(pData, id, mEventCounter++);
	pEvent->setManageContent(false);
	if (FAILED(queueEvent(pEvent))) {
		delete pEvent;
		return ERROR_SUCCESS;
	}
	return S_OK;
};

HRESULT EventManager::registerForEvent(ID id, EventListener* pListener)
{
	List<EventListener>* pListenerList = mpListeners->get(id);
	if (!pListenerList) {
		V_RETURN(registerEvent(id));
		pListenerList = mpListeners->get(id);
	}
	V_RETURN(pListenerList->unlock(0));
	return pListenerList->pushBack(pListener,&pListener->mListenerID);
}

HRESULT EventManager::unRegisterForEvent(ID eventID, ID listenerID) { 
	List<EventListener>* pList = mpListeners->get(eventID);
	if (pList) {
		V_RETURN(pList->unlock(0));
		return pList->deleteByID(listenerID);
	}
	return S_OK;
}

HRESULT EventManager::run(TIME elapsed)
{
	V_RETURN(mpEvents->unlock(0));
	V_RETURN(mpListeners->unlock(0));
	List<EventListener>* pListenerList = nullptr;
	ListElement<EventListener>* pListElem = nullptr;
	Event* pEvent = nullptr;
	UINT length = 0;
	EventListener* pListener;
	ID listenersKey;
	ID eventsKey;
	while (mpEvents->getLength()>0) {
		pEvent = nullptr;
		V_RETURN(mpEvents->popFirst(&pEvent));
		pListenerList = mpListeners->get(pEvent->getSlotID());
		if (pListenerList) {
			eventsKey = 0;
			if (SUCCEEDED(mpEvents->lock(&eventsKey))) {
				listenersKey = 0;
				if (SUCCEEDED(mpListeners->lock(&listenersKey))) {
					pListElem = pListenerList->iterator();
					while (pListElem) {
						pListener = pListElem->getObject();
						pListener->run(pEvent->getData());
						pListElem = (ListElement<EventListener>*)pListElem->getNext();
					}
					V_RETURN(mpListeners->unlock(listenersKey));
				}
				V_RETURN(mpEvents->unlock(eventsKey));
			}
		}
		SAFE_RELEASE(pEvent);
	}
	transferTempEvents();
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
	if (!mpEventsTemp) {
		mpEventsTemp = new List<Event>();
	}
	else {
		mpEventsTemp->restore();
	}
	mEventCounter = 0;
	return S_OK;
}

HRESULT EventManager::Release()
{
	mpEvents->restore();
	mpEventsTemp->restore();
	return S_OK;
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

