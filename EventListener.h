#ifndef EVENTLISTENER_INC
#define EVENTLISTENER_INC

class EventListener {
private:
	ID mEventID;
	ID mListenerID;
public:
	virtual HRESULT handleEvent(MemManaged* pData) = 0;
	EventListener(ID eventID) : mEventID(eventID) { EventManager::get()->registerForEvent(mEventID, this); };
	virtual ~EventListener() { EventManager::get()->unRegisterForEvent(mEventID, mListenerID); };
};

#endif