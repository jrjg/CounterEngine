#ifndef CONTROLSET_INC
#define CONTROLSET_INC

#include "MemManaged.h"
#include "List.h"

class Mapping : public MemManaged {
	friend class ControlSet;
protected:
	virtual ~Mapping() {};
	Mapping(KEYCODE mKeyCode, ID mEventID) : mKeyCode(mKeyCode), mIsKeyPressed(false), mEventID(mEventID) {};
	KEYCODE mKeyCode;
	bool mIsKeyPressed;
	ID mEventID;
};

class ControlSet : public MemManaged {
	friend class Controller;
private:
	List<Mapping>* mpMappings;
	ID mID;
protected:
	virtual ~ControlSet() { SAFE_RELEASE(mpMappings); };
	void setID(ID id) {mID = id;};
	ID getID() { return mID; };
	HRESULT addMapping(KEYCODE keyCode, ID eventID);
	HRESULT evalMappings();
	ControlSet() { restore(); };
	HRESULT restore();
};

#endif