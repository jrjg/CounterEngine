#ifndef INCLUDE_CONTROLLER
#define INCLUDE_CONTROLLER

#include "EventListener.h"
#include "MemManaged.h"
#include "CoreComponent.h"
#include "List.h"

class SetControlSetListener : public EventListener{
protected:
	virtual ~SetControlSetListener() {};
public:
	HRESULT handle(MemManaged* pData) override;
	SetControlSetListener() : EventListener(EVENT_SETCONTROLSET) {};
};

class Mapping : public MemManaged {
protected:
	virtual ~Mapping() {};
public:
	Mapping(KEYCODE mKeyCode, ID mEventID) : mKeyCode(mKeyCode), mIsKeyPressed(false), mEventID(mEventID) {};
	KEYCODE mKeyCode;
	bool mIsKeyPressed;
	ID mEventID;
};

class ControlSet : public MemManaged {
private:
	List<Mapping>* mpMappings;
	ID mID;
protected:
	virtual ~ControlSet() {};
public:
	List<Mapping>* getMappings() { return mpMappings; };
	ID getID() { return mID; };
	HRESULT addMapping(ID controlsID, KEYCODE keyCode, ID eventID);
	HRESULT evalMappings();
	ControlSet(ID id);
};

class Controller : public CoreComponent
{
	friend class SetControlSetListener;
private:
	List<ControlSet>* mpControls;
	ControlSet* mpActiveControls;
	SetControlSetListener* mpSetControlSetListener;

	HRESULT setControls(ID id);
	Controller() {};
protected:
	virtual ~Controller();
public:
	static Controller* get();
	ID addControlSet(ControlSet* pControlSet);

	HRESULT run(TIME elapsed)override;
	HRESULT restore();
};

#endif