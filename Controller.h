#ifndef INCLUDE_CONTROLLER
#define INCLUDE_CONTROLLER

class SetControlSetListener : public EventListener{
public:
	HRESULT handle(MemManaged* pData) override { Controller::get()->setControls(*(ID*)pData); };
	SetControlSetListener() : EventListener(EVENT_SETCONTROLSET) {};
};

class Mapping : public MemManaged {
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
public:
	List<Mapping>* getMappings() { return mpMappings; };
	ID getID() { return mID; };
	HRESULT addMapping(ID controlsID, KEYCODE keyCode, ID eventID) { mpMappings->pushBack(new Mapping(keyCode, eventID)); return S_OK; };
	HRESULT evalMappings();
	ControlSet(ID id) : mID(id) { mpMappings = new List<Mapping>(); };
	~ControlSet() { delete mpMappings; };
};

class Controller : public CoreComponent
{
	friend class SetControlSetListener;
private:
	static Controller* mpInstance;
	List<ControlSet>* mpControls;
	ControlSet* mpActiveControls;
	SetControlSetListener* mpSetControlSetListener;

	HRESULT setControls(ID id) { mpActiveControls = mpControls->getByID(id); return S_OK; };
	Controller() {};
	~Controller() { delete mpControls; };
public:
	static Controller* get() { if (!mpInstance) { mpInstance = new Controller(); } return mpInstance; };
	ID addControlSet(ControlSet* pControlSet) { return mpControls->pushBack(pControlSet); };

	HRESULT run(TIME elapsed)override { mpActiveControls->evalMappings(); };
	HRESULT restore();
};

#endif