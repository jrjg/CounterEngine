#ifndef INCLUDE_CONTROLLER
#define INCLUDE_CONTROLLER

#include "EventListener.h"
#include "MemManaged.h"
#include "CoreComponent.h"
#include "List.h"
#include "ControlSet.h"

class SetControlSetListener : public EventListener{
protected:
	virtual ~SetControlSetListener() {};
public:
	HRESULT handle(MemManaged* pData) override;
	SetControlSetListener() : EventListener(EVENT_SETCONTROLSET) {};
};

class Controller : public CoreComponent
{
	friend class SetControlSetListener;
private:
	List<ControlSet>* mpControls;
	ControlSet* mpActiveControls;
	SetControlSetListener* mpSetControlSetListener;

	HRESULT setControls(ID id);
	Controller() { restore(); };
protected:
	virtual ~Controller();
public:
	static Controller* get();
	ID newControlSet();
	HRESULT addMapping(ID controlsID, KEYCODE keyCode, ID eventID);

	HRESULT run(TIME elapsed)override;
	HRESULT restore();
};

#endif