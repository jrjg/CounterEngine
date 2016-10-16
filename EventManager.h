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
	ID mEventCounter;
	HRESULT registerEvent(ID id); 
	EventManager();
protected:
	virtual ~EventManager();
public:
	ID queueEvent(ID id, MemManaged* pData);
	ID registerForEvent(ID id, EventListener* pListener);
	HRESULT unRegisterForEvent(ID eventID, ID listenerID);
	HRESULT removeEvent(ID id);

	HRESULT run(TIME elapsed) override;
	HRESULT restore();
	void Release()override;
	void ManualRelease() { delete this; };

	static EventManager* get();
};

#endif