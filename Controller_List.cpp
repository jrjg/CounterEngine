#include "CLibrary.h"
#include "TopInclude.h"
#include "List.h"
#include "EventManager.h"
#include "Engine.h"
#include "Memory.h"
#include "ListElement.h"
#include "Controller.h"

Controller* gpController;

Controller* Controller::get()
{
	if (!gpController) {
		gpController = new Controller();
	}
	return gpController;
}

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

HRESULT Controller::run(TIME time)
{
	Mapping* pMapping;
	bool isKeyPressed;
	for (ListElement<Mapping>* pListElem = get()->mpActiveControls->getMappings()->getFirst(); pListElem != NULL; pListElem = pListElem->getNext()) {
		pMapping = (Mapping*)pListElem->getObject();
		isKeyPressed = KEY_DOWN(pMapping->mKeyCode);
		if (pMapping->mIsKeyPressed != isKeyPressed) {
			EventManager_QueueEvent(pMapping->mEventID, new BoolManaged());
			pMapping->mIsKeyPressed = isKeyPressed;
		}
	}
	return S_OK;
}

ID Controller::newControls() 
{
	Controls* pNewControls = new Controls(get()->mIDCounter++);
	get()->mpControls->pushBack(pNewControls);
	return pNewControls->getID();
}