#include "TopInclude.h"
#include "CoreComponent.h"
#include "Vector.h"
#include "List.h"
#include "Memory.h"
#include "ListElement.h"
#include "EventListener.h"
#include "Event.h"
#include "SimplyManaged.h"
#include "MemManaged.h"
#include "EventManager.h"

EventManager* gpEventManager;

HRESULT EventManager::registerEvent(ID id) { return mpListeners->set(id, new List<EventListener>()); }
EventManager::EventManager() : CoreComponent(false) {
	mpListeners = new Vector<List<EventListener>>(100); 
	mpEvents = new List<Event>(); 
	mEventCounter = 0;
};
EventManager::~EventManager() { SAFE_RELEASE(mpListeners); SAFE_RELEASE(mpEvents); };

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
HRESULT EventManager::unRegisterForEvent(ID eventID, ID listenerID) { return mpListeners->get(eventID)->deleteByID(listenerID); }
HRESULT EventManager::removeEvent(ID id) { mpEvents->deleteByID(id); };

EventManager * EventManager::get() {
	if (!gpEventManager) {
		gpEventManager = new EventManager();
		gpEventManager->restoreCore();
	} 
	return gpEventManager;
};

HRESULT EventManager::run(TIME elapsed)
{
	List<EventListener>* pListenerList;
	
	ListElement<EventListener>* pListElem;
	if (mpEvents->getLength() > 0) {
		Event* pEvent = mpEvents->popFirst();
		while (pEvent) {
			pListenerList = mpListeners->get(pEvent->getSlotID());
			if (pListenerList) {
				if (pListenerList->getLength() > 0) {
					for (pListElem = pListenerList->getFirst(); pListElem != NULL; pListElem = pListElem->getNext())
					{
						pListElem->getObject()->run(pEvent->getData());
					}
				}
			}
			SAFE_RELEASE(pEvent);
			pEvent = mpEvents->popFirst();
		}
	}
	return S_OK;
};

HRESULT EventManager::restore()
{
	if (!mpListeners) { mpListeners = new Vector<List<EventListener>>(100); }; mpListeners->restore();
	if (!mpEvents) { mpEvents = new List<Event>(); }; mpEvents->restore();
	mEventCounter = 0;
	return S_OK;
};
