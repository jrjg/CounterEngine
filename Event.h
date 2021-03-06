#ifndef EVENT_INC
#define EVENT_INC

#include "MemManaged.h"

class Event : public MemManaged
{
private:
	bool mManageContent;
	MemManaged* mpData;
	ID mSlotID;
	ID mID;
public:
	Event(MemManaged* pData, ID slotID, ID id) : mManageContent(true), mpData(pData), mSlotID(slotID), mID(id) {};
	virtual ~Event() { if (mManageContent) { SAFE_RELEASE(mpData); } };
	ID getSlotID() { return mSlotID; };
	ID getID() { return mID; };
	void setManageContent(bool m) { mManageContent = m; };
	MemManaged* getData() { return mpData; };
};

#endif 