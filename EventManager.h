#ifndef INCLUDE_EVENTMANAGER
#define INCLUDE_EVENTMANAGER

typedef HRESULT(*ECB)(ID,void*); 

class EventListener {
public:
	virtual HRESULT handleEvent(ID eventID, void* pData) = 0;
};

class Event : public MemManaged
{
public:
	Event(void* pData, ID slotID, ID id, bool deleteContent) : mDeleteContent(deleteContent), mpData(pData), mSlotid(slotID), mID(id) {};
	~Event() { if (mDeleteContent) { delete (MemManaged*)mpData; } };
	bool mDeleteContent;
	void* mpData;
	ID mSlotid;
	ID mID;
};

class EventManager : public CoreComponent
{
private:
	Vector<List<EventListener>>* mpListenersForEvents;
	List<Event>* mpEvents;
	ID mEventCounter;
public:
	ID queueEvent(ID id, void* pData, bool deleteContent) { return mpEvents->pushBack(new Event(pData, id, mEventCounter++, deleteContent)); };
	HRESULT registerEvent(ID id) { return mpListenersForEvents->set(id, new List<EventListener>(true)); };
	ID registerForEvent(ID, ECB);
	HRESULT triggerEvent(ID, void*);
	HRESULT removeEvent(ID id) { mpEvents->deleteByID(id); };

	EventManager(Engine* pEngine) :CoreComponent(pEngine){ };
	~EventManager() {  };

	HRESULT run(TIME);
	HRESULT restore() {};
	HRESULT release() { delete mpListenersForEvents; delete mpEvents; };
};

#endif