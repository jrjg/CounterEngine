#include "TopInclude.h"
#include "Vector.h"
#include "List.h"
#include "Engine.h"
#include "ProcessManager.h"
#include "Memory.h"
#include "ListElement.h"
#include "CoreComponent.h"
#include "String.h"
#include "EventListener.h"

#include "EventManager.h"

EventManager* gpEventManager;

EventManager::EventManager(Engine* pEngine) : CoreComponent(pEngine){
	mpListenersForEvents = new Vector<List<EventListener>>(100);
	mpEvents = new List<Event>(true);
	mEventCounter = 0;
}

ID EventManager::registerForEvent(ID id, EventListener* pListener)
{
	List<EventListener>* pListenerList = mpListenersForEvents->get(id);
	if (!pListenerList) {
		registerEvent(id);
		pListenerList = (List<EventListener>*)mpListenersForEvents->get(id);
	}
	return pListenerList->pushBack(pListener);
}

HRESULT EventManager::handleProcess(TIME elapsed)
{
	List<EventListener>* pListenerList;
	Event* pEvent = mpEvents->pop();
	ListElement<EventListener>* pListElem;
	while (pEvent)
	{
		pListenerList = mpListenersForEvents->get(pEvent->getSlotID());
		if (pListenerList) {
			if (pListenerList->getLength() > 0) {
				for (pListElem = pListenerList->getFirst(); pListElem != NULL; pListElem = pListElem->getNext())
				{
					pListElem->getObject()->handleEvent(pEvent->getSlotID(),pEvent->getData());
				}
			}
		}
		delete pEvent;
		pEvent = mpEvents->pop();
	}
	return S_OK;
}

HRESULT EventManager::restore()
{
	if (!mpListenersForEvents) { mpListenersForEvents = new Vector<List<EventListener>>(100); };
	mpListenersForEvents->restore();
	if (!mpEvents) { mpEvents = new List<Event>(true); };
	mpEvents->restore();
	mEventCounter = 0;
	return S_OK;
}
