#include "TopInclude.h"
#include "Vector.h"
#include "List.h"
#include "Engine.h"
#include "ProcessManager.h"
#include "Memory.h"
#include "ListElement.h"

#include "EventManager.h"

EventManager* gpEventManager;

EventManager::EventManager() {
	mpEventVector = new Vector<List<EventListener>>(true, 100);
	mpListenerToEventSlotVector = new Vector<SimplyManaged<ID>>(true, 100);
	mpEventList = new List<Event>(true);
	mEventCounter = 0;
	mListenerCounter = 0;
	registerForEvent(EVENT_RESTORE,&restore);
	registerForEvent(EVENT_RELEASE, &release);
}

HRESULT EventManager::registerForEvent(ID id, ECB pCallBack)
{
	List<EventListener>* pListenerList = (List<EventListener>*)mpEventVector->get(id);
	if (!pListenerList) {
		if (FAILED(registerEvent(id))) {
			CE1_ASSERT(0&&"Could not register for Event");
		}
		pListenerList = (List<EventListener>*)mpEventVector->get(id);
	}
	mpListenerToEventSlotVector->insert(pListenerList->pushBack(new EventListener(mListenerCounter++, pCallBack)), new SimplyManaged<ID>(id));
	return S_OK;
}

HRESULT EventManager::triggerEvent(ID slotid, void* pData)
{
	List<EventListener>* pListenerList = mpEventVector->get(slotid);
	if (!pListenerList) { return S_OK; }
	for (ListElement<EventListener>* pListElem = pListenerList->getFirst(); pListElem != NULL; pListElem = pListElem->getNext())
	{
		(*((EventListener*)pListElem->getObject())->mpCallBack)(pData);
	}
	return S_OK;
}

EventManager * EventManager::get()
{
	if (!gpEventManager) {
		gpEventManager = new EventManager();
	}
	return gpEventManager;
}

HRESULT EventManager::run(TIME elapsed)
{
	Event* pEvent = (Event*)(get()->mpEventList->pop());
	while (pEvent)
	{
		get()->triggerEvent(pEvent->mSlotid, pEvent->mpData);
		delete pEvent;
		pEvent = (Event*)(get()->mpEventList->pop());
	}
	return S_OK;
}