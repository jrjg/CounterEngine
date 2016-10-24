#ifndef EVENTLISTENER_INC
#define EVENTLISTENER_INC

#include "MemManaged.h"

class EventListener : public MemManaged{
	friend class EventManager;
private:
	ID mEventID;
	ID mListenerID;
	bool mEnabled;
protected:
	virtual HRESULT handle(MemManaged* pData) = 0;
public:
	EventListener(ID eventID);
	HRESULT run(MemManaged* pData) { if (mEnabled) { handle(pData); } return S_OK; };
	void enable() { mEnabled = true; };
	void disable() { mEnabled = false; };
	virtual ~EventListener();
};

#endif