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
	Controller* pController = Controller::get();
	if (pController) {
		return pController->setControls(*((SimplyManaged<ID>*)pData)->getObject());
	}
	return ERROR_SUCCESS;
};

HRESULT Controller::setControls(ID id) { 
	mpActiveControls = mpControls->getByID(id); 
	if (!mpActiveControls) {
		return ERROR_SUCCESS;
	}
	return S_OK; 
}

Controller::~Controller() {
	SAFE_RELEASE(mpSetControlSetListener);
	SAFE_RELEASE(mpControls);
}

HRESULT Controller::newControlSet(ID* pID) { 
	V_RETURN(mpControls->unlock(0));
	HRESULT hr = S_OK;
	ControlSet* pControlSet = new ControlSet();
	hr = mpControls->pushBack(pControlSet, pID);
	if (FAILED(hr)) {
		delete pControlSet;
	}
	return hr;
};

HRESULT Controller::addMapping(ID controlsID, KEYCODE keyCode, ID eventID)
{
	ControlSet* pControlSet = mpControls->getByID(controlsID);
	if (!pControlSet) {
		return ERROR_SUCCESS;
	}
	return pControlSet->addMapping(keyCode, eventID);
};

HRESULT Controller::run(TIME elapsed) { 
	if (mpActiveControls) {
		return mpActiveControls->evalMappings();
	}; 
	return S_OK;
}

HRESULT Controller::restore() {
	if (!mpControls) { 
		mpControls = new List<ControlSet>(); 
	}else {
		mpControls->restore();
	}
	if (mpSetControlSetListener) { SAFE_RELEASE(mpSetControlSetListener); }; mpSetControlSetListener = new SetControlSetListener();
	mpActiveControls = NULL;
	return S_OK;
};