#include "CLibrary.h"
#include "TopInclude.h"
#include "List.h"
#include "EventManager.h"
#include "Engine.h"
#include "Memory.h"
#include "ListElement.h"
#include "CoreComponent.h"
#include "EventListener.h"
#include "SimplyManaged.h"
#include "MemManaged.h"

#include "Controller.h"

Controller* gpController;

HRESULT SetControlSetListener::handle(MemManaged* pData){ 
	Controller::get()->setControls(*(ID*)pData); 
	return S_OK;
};

HRESULT ControlSet::addMapping(ID controlsID, KEYCODE keyCode, ID eventID) {
	mpMappings->pushBack(new Mapping(keyCode, eventID)); 
	return S_OK; 
};

ControlSet::ControlSet(ID id) : mID(id) {
	mpMappings = new List<Mapping>(); 
};

HRESULT ControlSet::evalMappings() {
	Mapping* pMapping;
	bool isKeyPressed;
	for (ListElement<Mapping>* pListElem = mpMappings->getFirst(); pListElem != NULL; pListElem = pListElem->getNext()) {
		pMapping = pListElem->getObject();
		isKeyPressed = (GetAsyncKeyState(pMapping->mKeyCode) & 0x8000) ? 1 : 0;
		if (pMapping->mIsKeyPressed != isKeyPressed) {
			EventManager::get()->queueEvent(pMapping->mEventID, new SimplyManaged<bool>(isKeyPressed));
			pMapping->mIsKeyPressed = isKeyPressed;
		}
	}
	return S_OK;
};

HRESULT Controller::setControls(ID id) { 
	mpActiveControls = mpControls->getByID(id); return S_OK; 
}
Controller::~Controller() { mpControls->release(); }

Controller * Controller::get() { 
	if (!gpController) {
		gpController = new Controller();
	} 
	return gpController;
};

ID Controller::addControlSet(ControlSet * pControlSet) { return mpControls->pushBack(pControlSet); };

HRESULT Controller::run(TIME elapsed) { mpActiveControls->evalMappings(); return S_OK; };

HRESULT Controller::restore() {
	if (!mpControls) { mpControls = new List<ControlSet>(); }; mpControls->restore();
	if (mpSetControlSetListener) { mpSetControlSetListener->release(); }; mpSetControlSetListener = new SetControlSetListener();
	return S_OK; 
};