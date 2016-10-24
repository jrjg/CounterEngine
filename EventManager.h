#ifndef INCLUDE_EVENTMANAGER
#define INCLUDE_EVENTMANAGER

#include "CoreComponent.h"
#include "EventListener.h"
#include "Event.h"
#include "Vector.h"
#include "List.h"
#include "Singleton.h"

class EventManager : public CoreComponent,public Singleton<EventManager>{
	friend class Singleton<EventManager>;
private:
	Vector< List<EventListener> >* mpListeners;
	List<Event>* mpEvents;
	List<Event>* mpEventsTemp;
	ID mEventCounter;
	HRESULT registerEvent(ID id); 
	EventManager();
	virtual ~EventManager();
	HRESULT queueEvent(Event* pEvent);
	HRESULT transferTempEvents();
public:
	HRESULT queueEvent(ID id, MemManaged* pData);
	HRESULT queueEventND(ID id, MemManaged* pData);
	HRESULT registerForEvent(ID id, EventListener* pListener);
	HRESULT unRegisterForEvent(ID eventID, ID listenerID);

	HRESULT run(TIME elapsed) override;
	HRESULT restore();
	HRESULT Release()override;
	void ManualRelease() { delete this; };

	static EventManager* get();
};

#endif