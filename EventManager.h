#ifndef INCLUDE_EVENTMANAGER
#define INCLUDE_EVENTMANAGER

class EventManager : public CoreComponent{
private:
	static EventManager* mpInstance;
	Vector< List<EventListener> >* mpListeners;
	List<Event>* mpEvents;
	ID mEventCounter;
	HRESULT registerEvent(ID id) { return mpListeners->set(id, new List<EventListener>()); };
	EventManager() : CoreComponent(false) {};
	~EventManager() { delete mpListeners; delete mpEvents; };
public:
	ID queueEvent(ID id, MemManaged* pData) { return mpEvents->pushBack(new Event(pData, id, mEventCounter++)); };
	ID registerForEvent(ID id, EventListener* pListener);
	HRESULT unRegisterForEvent(ID eventID, ID listenerID) { return mpListeners->get(eventID)->deleteByID(listenerID); };
	HRESULT removeEvent(ID id) { mpEvents->deleteByID(id); };

	static EventManager* get();

	HRESULT run(TIME elapsed) override;
	HRESULT restore();
};

#endif