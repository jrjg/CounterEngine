#ifndef INCLUDE_EVENTMANAGER
#define INCLUDE_EVENTMANAGER

class Event : public MemManaged
{
private:
	bool mManageContent;
	MemManaged* mpData;
	ID mSlotID;
	ID mID;
public:
	Event(MemManaged* pData, ID slotID, ID id, bool manageContent) : mManageContent(manageContent), mpData(pData), mSlotID(slotID), mID(id) {};
	~Event() { if (mManageContent) { delete mpData; } };
	ID getSlotID() { return mSlotID; };
	ID getID() { return mID; };
	void setManageContent(bool m) { mManageContent = m; };
	MemManaged* getData() { return mpData; };
};

class EventManager : public CoreComponent
{
private:
	Vector<List<EventListener>>* mpListenersForEvents;
	List<Event>* mpEvents;
	ID mEventCounter;
	HRESULT registerEvent(ID id) { return mpListenersForEvents->set(id, new List<EventListener>(true)); };
public:
	ID queueEvent(ID id, MemManaged* pData, bool deleteContent) { return mpEvents->pushBack(new Event(pData, id, mEventCounter++, deleteContent)); };
	ID registerForEvent(ID id, EventListener* pListener);
	HRESULT unRegisterForEvent(ID eventID, ID listenerID) { return mpListenersForEvents->get(eventID)->deleteByID(listenerID); };
	HRESULT removeEvent(ID id) { mpEvents->deleteByID(id); };

	HRESULT handleProcess(TIME elapsed) override;
	HRESULT restore()override;

	EventManager(Engine* pEngine) :CoreComponent(pEngine) { restore(); };
	~EventManager() { delete mpListenersForEvents; delete mpEvents; };
};

#endif