#ifndef EVENTLISTENER_INC
#define EVENTLISTENER_INC

class EventListener : public MemManaged{
private:
	ID mEventID;
	ID mListenerID;
	bool mEnabled;
protected:
	virtual HRESULT handle(MemManaged* pData) = 0;
public:
	EventListener(ID eventID) : mEventID(eventID) { EventManager::get()->registerForEvent(mEventID, this); };
	virtual ~EventListener() { EventManager::get()->unRegisterForEvent(mEventID, mListenerID); };
	HRESULT run(MemManaged* pData) { if (mEnabled) { handle(pData); } };
	void enable() { mEnabled = true; };
	void disable() { mEnabled = false; };
};

#endif