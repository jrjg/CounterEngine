#include "TopInclude.h"
#include "Vector.h"
#include "List.h"
#include "Engine.h"
#include "EventManager.h"
#include "ProcessManager.h"


struct Event
{
	void* pData;
	ID slotid;
	ID id;
};

struct EventListener
{
	ID _id;
	ECB _pCallBack;
};

struct EventManager
{
	Vector* _pEventVector;
	Vector* _pListenerToEventSlotVector;
	List* _pEventList;
	ID _eventcounter;
	ID _listenercounter;
};

EventManager* EventManager_New(void)
{
	EventManager* pEM;
	_NEW(EventManager, pEM);
	CEASSERT(pEM);
	pEM->_pEventVector = Vector_New(sizeof(List*),100);
	pEM->_pListenerToEventSlotVector = Vector_New(sizeof(ID*),400);
	pEM->_pEventList = List_New(sizeof(Event*));
	pEM->_eventcounter = 0;
	pEM->_listenercounter = 0;
	return pEM;
}

HRESULT EventManager_Delete()
{
	EventManager* pEM = Engine_GetEM();
	if (!pEM) {
		return S_OK;
	}
	List* pListenerList;
	for (unsigned int i = 0; i < Vector_Last(pEM->_pEventVector); i++)
	{
		pListenerList = (List*)Vector_Get(pEM->_pEventVector, i);
		SAFECALL(List_FullDelete(pListenerList,true));
	}
	SAFECALL(Vector_Delete(pEM->_pEventVector)); 
	SAFECALL(Vector_FullDelete(pEM->_pListenerToEventSlotVector));
	_DEL(pEM);
	return S_OK;
}

ID EventManager_QueueEvent(ID id, void* pData)
{
	EventManager* pEM = Engine_GetEM();
	CEASSERT(pEM);
	Event* pEvent;
	_NEW(Event, pEvent);
	if (!pEvent)
	{
		return 0;
	}
	pEvent->slotid = id;
	pEM->_eventcounter++;
	pEvent->id = pEM->_eventcounter;
	pEvent->pData = pData;
	ID id2 = List_PushBack(pEM->_pEventList,pEvent);
	if (id2==0)
	{
		_DEL(pEvent);
		return 0;
	}
	return pEvent->id;
}

HRESULT EventManager_RegisterEvent(ID id)
{
	EventManager* pEM = Engine_GetEM();
	CEASSERT(pEM);
	List* pList = List_New(sizeof(EventListener*));
	SAFECALL(Vector_Insert(pEM->_pEventVector, id, pList));
	return S_OK;
}


ID EventManager_RegisterForEvent(ID id, ECB pCallBack)
{
	EventManager* pEM = Engine_GetEM();
	CEASSERT(pEM && pCallBack);
	EventListener* pListener;
	_NEW(EventListener, pListener);
	pEM->_listenercounter++;
	pListener->_id = pEM->_listenercounter;
	pListener->_pCallBack = pCallBack;

	List* pList = (List*)Vector_Get(pEM->_pEventVector, id);
	if (!pList) {
		SAFECALL(EventManager_RegisterEvent(id));
		pList = (List*)Vector_Get(pEM->_pEventVector, id);
	}

	ID listenerid = List_PushBack(pList, pListener);
	if(!listenerid)
	{
		_DEL(pListener);
		return 0;
	}
	ID* pEventSlotID;
	_NEW(ID, pEventSlotID);
	(*pEventSlotID) = id;
	SAFECALL(Vector_Insert(pEM->_pListenerToEventSlotVector,listenerid,pEventSlotID));
	return listenerid;
}

HRESULT EventManager_UnRegisterEvent(ID eventslotid)
{
	EventManager* pEM = Engine_GetEM();
	CEASSERT(pEM );
	SAFECALL(List_DeleteAllElements((List*)Vector_Get(pEM->_pEventVector, eventslotid),true));
	SAFECALL(Vector_DeleteElement(pEM->_pEventVector, eventslotid));
	return S_OK;
}

HRESULT EventManager_UnRegisterForEvent(ID listenerid)
{
	EventManager* pEM = Engine_GetEM();
	CEASSERT(pEM );
	ID eventslotid = *(ID*)Vector_Get(pEM->_pListenerToEventSlotVector,listenerid);
	SAFECALL(Vector_DeleteElement(pEM->_pListenerToEventSlotVector,listenerid));
	SAFECALL(List_DeleteElement((List*)Vector_Get(pEM->_pEventVector, eventslotid), listenerid,true));
	return S_OK;
}

HRESULT EventManager_TriggerEvent(ID slotid, void* pData)
{
	EventManager* pEM = Engine_GetEM();
	CEASSERT(pEM );
	List* pList = (List*)Vector_Get(pEM->_pEventVector,slotid);
	EventListener* pListener;
	if (!pList) {
		return S_OK;
	}
	for (Iterator itr = List_Iterator(pList); itr != NULL; itr = List_Next(itr))
	{
		pListener = (EventListener*)List_Get(itr);
		(*pListener->_pCallBack)(pData);
	}
	return S_OK;
}

HRESULT EventManager_RemoveEvent(ID eventid)
{
	EventManager* pEM = Engine_GetEM();
	CEASSERT(pEM);
	SAFECALL(List_DeleteElement(pEM->_pEventList, eventid,true));
	return S_OK;
}

HRESULT EventManager_Run(TIME elapsed)
{
	EventManager* pEM = Engine_GetEM();
	CEASSERT(pEM && elapsed);
	Event* pEvent = (Event*)List_Pop(pEM->_pEventList);
	while (pEvent)
	{
		SAFECALL(EventManager_TriggerEvent(pEvent->slotid, pEvent->pData));
		_DEL(pEvent->pData);
		_DEL(pEvent);
		pEvent = (Event*)List_Pop(pEM->_pEventList);
	}
	return S_OK;
}