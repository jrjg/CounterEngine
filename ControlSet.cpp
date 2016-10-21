#include "TopInclude.h"
#include "MemManaged.h"
#include "EventManager.h"
#include "SimplyManaged.h"

#include "ControlSet.h"

HRESULT ControlSet::addMapping(KEYCODE keyCode, ID eventID) {
	mpMappings->pushBack(new Mapping(keyCode, eventID));
	return S_OK;
};

HRESULT ControlSet::restore()
{
	if (!mpMappings) { mpMappings = new List<Mapping>(); }; mpMappings->restore();
	return S_OK;
};

HRESULT ControlSet::evalMappings() {
	Mapping* pMapping;
	bool isKeyPressed;
	for (ListElement<Mapping>* pListElem = (ListElement<Mapping>*)mpMappings->iterator(); pListElem != NULL; pListElem = (ListElement<Mapping>*)pListElem->getNext()) {
		pMapping = pListElem->getObject();
		isKeyPressed = (GetAsyncKeyState(pMapping->mKeyCode) & 0x8000) ? 1 : 0;
		if (pMapping->mIsKeyPressed != isKeyPressed) {
			EventManager::get()->queueEvent(pMapping->mEventID, new SimplyManaged<bool>(isKeyPressed));
			pMapping->mIsKeyPressed = isKeyPressed;
		}
	}
	return S_OK;
};