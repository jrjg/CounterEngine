#include "CLibrary.h"
#include "TopInclude.h"
#include "List.h"
#include "EventManager.h"
#include "Engine.h"
#include "Memory.h"
#include "CoreComponent.h"
#include "EventListener.h"
#include "SimplyManaged.h"
#include "MemManaged.h"
#include "ControlSet.h"
#include "Singleton.h"

#include "Controller.h"

HRESULT SetControlSetListener::handle(MemManaged* pData){ 
	Controller::get()->setControls( *((SimplyManaged<ID>*)pData)->getObject() ); 
	return S_OK;
};

HRESULT Controller::setControls(ID id) { 
	mpActiveControls = mpControls->getByID(id); return S_OK; 
}

Controller::~Controller() {
	SAFE_RELEASE(mpSetControlSetListener);
	SAFE_RELEASE(mpControls);
}

ID Controller::newControlSet() { 
	return mpControls->pushBack(new ControlSet()); 
}

HRESULT Controller::addMapping(ID controlsID, KEYCODE keyCode, ID eventID)
{
	return mpControls->getByID(controlsID)->addMapping(keyCode, eventID);
};

HRESULT Controller::run(TIME elapsed) { 
	if (mpActiveControls) {
		mpActiveControls->evalMappings();
	}; 
	return S_OK;
}

HRESULT Controller::restore() {
	if (!mpControls) { mpControls = new List<ControlSet>(); }; mpControls->restore();
	if (mpSetControlSetListener) { SAFE_RELEASE(mpSetControlSetListener); }; mpSetControlSetListener = new SetControlSetListener();
	mpActiveControls = NULL;
	return S_OK;
};