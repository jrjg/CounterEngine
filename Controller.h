#ifndef INCLUDE_CONTROLLER
#define INCLUDE_CONTROLLER

class Mapping : public MemManaged{
public:
	Mapping(KEYCODE mKeyCode, ID mEventID) : mKeyCode(mKeyCode), mIsKeyPressed(false), mEventID(mEventID) {};
	KEYCODE mKeyCode;
	bool mIsKeyPressed;
	ID mEventID;
};

class Controls : public MemManaged {
private:
	List<Mapping>* mpMappings;
	ID mID;
public:
	Controls(ID id) : mID(id) { mpMappings = new List<Mapping>(true); };
	HRESULT evalKeyDown(void* pData);
	HRESULT evalKeyUp(void* pData);
	List<Mapping>* getMappings() { return mpMappings; }
	ID getID() { return mID; };
};

class Controller : public MemManaged
{
private:
	List<Controls>* mpControls;
	Controls* mpActiveControls;
	ID mIDCounter;
	Controller() {EventManager::registerForEvent(EVENT_RESTORE, &Controller::restore);EventManager::registerForEvent(EVENT_RELEASE, &Controller::release);};
	~Controller() { delete mpControls; };
public:
	static HRESULT release(void* p0) { delete get(); };
	static HRESULT restore(void* p0) {if (!get()->mpControls) get()->mpControls = new List<Controls>(true);get()->mIDCounter = 0;return S_OK;};
	static Controller* get();

	static HRESULT run(TIME);
	static HRESULT setControls(ID id) { get()->mpActiveControls = (Controls*)get()->mpControls->getByID(id); return S_OK; };
	static HRESULT addMapping(ID controlsID, KEYCODE keyCode, ID eventID) { ((Controls*)get()->mpControls->getByID(controlsID))->getMappings()->pushBack(new Mapping(keyCode, eventID)); return S_OK; };
	static ID newControls();
};

#endif