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


ID EventManager::registerForEvent(ID id, EventListener* pListener)
{
	List<EventListener>* pListenerList = mpListeners->get(id);
	if (!pListenerList) {
		registerEvent(id);
		pListenerList = mpListeners->get(id);
	}
	return pListenerList->pushBack(pListener);
};

EventManager * EventManager::get() {
	if (!mpInstance) {
		mpInstance = new EventManager(); 
		mpInstance->CoreComponent::restore();
	} 
	return mpInstance;
};

HRESULT EventManager::run(TIME elapsed)
{
	List<EventListener>* pListenerList;
	Event* pEvent = mpEvents->pop();
	ListElement<EventListener>* pListElem;
	while (pEvent)
	{
		pListenerList = mpListeners->get(pEvent->getSlotID());
		if (pListenerList) {
			if (pListenerList->getLength() > 0) {
				for (pListElem = pListenerList->getFirst(); pListElem != NULL; pListElem = pListElem->getNext())
				{
					pListElem->getObject()->run(pEvent->getData());
				}
			}
		}
		delete pEvent;
		pEvent = mpEvents->pop();
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
