#include "TopInclude.h"
#include "MemManaged.h"
#include "EventManager.h"
#include "SimplyManaged.h"

#include "ControlSet.h"

HRESULT ControlSet::addMapping(KEYCODE keyCode, ID eventID) {
	V_RETURN(mpMappings->unlock(0));
	ID id;
	Mapping* pMapping = new Mapping(keyCode, eventID);
	if (FAILED(mpMappings->pushBack(pMapping, &id))) {
		delete pMapping;
		return ERROR_SUCCESS;
	}
	return S_OK;
};

HRESULT ControlSet::restore()
{
	if (!mpMappings) { 
		mpMappings = new List<Mapping>(); 
	}
	else {
		mpMappings->restore();
	}
	return S_OK;
};

HRESULT ControlSet::evalMappings() {
	Mapping* pMapping;
	bool isKeyPressed;
	ID mappingsKey;
	V_RETURN(mpMappings->lock(&mappingsKey));
	ListElement<Mapping>* pListElem = (ListElement<Mapping>*)mpMappings->iterator();
	EventManager* pEventManager = EventManager::get();
	while (pListElem) {
		pMapping = pListElem->getObject();
		if (pMapping) {
			isKeyPressed = (GetAsyncKeyState(pMapping->mKeyCode) & 0x8000) ? 1 : 0;
			if (pMapping->mIsKeyPressed != isKeyPressed) {
				if (pEventManager) {
					SimplyManaged<bool>* pBool = new SimplyManaged<bool>(isKeyPressed);
					if (FAILED(pEventManager->queueEvent(pMapping->mEventID, pBool))) {
						delete pBool;
					}
				}
				pMapping->mIsKeyPressed = isKeyPressed;
			}
			pListElem = (ListElement<Mapping>*)pListElem->getNext();
		}
	}
	V_RETURN(mpMappings->unlock(mappingsKey));
	return S_OK;
};