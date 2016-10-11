#ifndef EVENTLISTENER_INC
#define EVENTLISTENER_INC

class EventListener {
private:
	ID mEventID;
	EventManager* mpEventManager;
public:
	virtual HRESULT handleEvent(ID eventID, MemManaged* pData) = 0;
	EventListener(ID eventID, EventManager* pEventManager) : mEventID(eventID), mpEventManager(pEventManager) { mpEventManager->registerForEvent(mEventID, this); };
	~EventListener() { mpEventManager->unRegisterForEvent(mEventID,this); };
};

#endif