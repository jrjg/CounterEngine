#include "CLibrary.h"
#include "TopInclude.h"
#include "List.h"
#include "EventManager.h"
#include "Engine.h"
#include "Memory.h"
#include "ListElement.h"
#include "CoreComponent.h"
#include "EventListener.h"

#include "Controller.h"

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
}

HRESULT Controller::restore() { 
	if (!mpControls) { mpControls = new List<ControlSet>(); }; mpControls->restore();
	delete mpSetControlSetListener; mpSetControlSetListener = new SetControlSetListener();
	return S_OK; 
};