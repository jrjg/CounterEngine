#ifndef INCLUDE_EVENTMANAGER
#define INCLUDE_EVENTMANAGER

typedef HRESULT(*ECB)(void*); 

class Event : public MemManaged
{
public:
	Event(void* pData, ID slotID, ID id) : mpData(pData), mSlotid(slotID), mID(id) {};
	~Event() { delete (MemManaged*)mpData; };
	void* mpData;
	ID mSlotid;
	ID mID;
};

class EventListener : public MemManaged
{
public:
	EventListener(ID id, ECB pCallBack) :mID(id),mpCallBack(pCallBack) {};
	ID mID;
	ECB mpCallBack;
};

class EventManager : public MemManaged
{
private:
	Vector* mpEventVector;
	Vector* mpListenerToEventSlotVector;
	List<Event>* mpEventList;
	ID mEventCounter;
	ID mListenerCounter;
	EventManager();
	~EventManager() {delete mpEventVector;delete mpListenerToEventSlotVector;delete mpEventList;};
public:
	ID queueEvent(ID id, void* pData) { return mpEventList->pushBack(new Event(pData, id, mEventCounter++)); };
	HRESULT registerEvent(ID id) { mpEventVector->insert(id, new List<EventListener>(true)); };
	ID registerForEvent(ID, ECB);
	HRESULT triggerEvent(ID, void*);
	HRESULT removeEvent(ID id) { mpEventList->deleteByID(id); };

	static EventManager* get();
	static HRESULT run(TIME);
	static HRESULT restore(void* p0) {};
	static HRESULT release(void* p0) { delete get(); };
};

#endif