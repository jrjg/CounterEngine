#ifndef CONTROLSET_INC
#define CONTROLSET_INC

#include "MemManaged.h"
#include "List.h"

class Mapping : public MemManaged {
	friend class ControlSet;
protected:
	KEYCODE mKeyCode;
	bool mIsKeyPressed;
	ID mEventID;
public:
	virtual ~Mapping() {};
	Mapping(KEYCODE mKeyCode, ID mEventID) : mKeyCode(mKeyCode), mIsKeyPressed(false), mEventID(mEventID) {};
};

class ControlSet : public MemManaged {
	friend class Controller;
private:
	List<Mapping>* mpMappings;
	ID mID;
protected:
	void setID(ID id) {mID = id;};
	ID getID() { return mID; };
	HRESULT addMapping(KEYCODE keyCode, ID eventID);
	HRESULT evalMappings();
	HRESULT restore();
public:
	virtual ~ControlSet() { SAFE_RELEASE(mpMappings); };
	ControlSet() { restore(); };
};

#endif