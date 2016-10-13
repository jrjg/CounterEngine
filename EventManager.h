#ifndef INCLUDE_EVENTMANAGER
#define INCLUDE_EVENTMANAGER

#include "CoreComponent.h"
#include "EventListener.h"
#include "Event.h"
#include "Vector.h"
#include "List.h"

class EventManager : public CoreComponent{
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

	static EventManager* get();

	HRESULT run(TIME elapsed) override;
	HRESULT restore();
};

#endif