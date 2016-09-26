#include "CLibrary.h"
#include "TopInclude.h"
#include "List.h"
#include "EventManager.h"
#include "Engine.h"
#include "Controller.h"

struct Key {
	KEYCODE keycode;
	bool down;
	ID eventid;
};

struct Controls
{
	List* pKeys;
	ID id;
};

struct Controller
{
	List* pControls;
	Controls* pActiveControls;
	ID idcounter;
};

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

HRESULT Controller_Run(TIME time)
{
	Controller* pController = Engine_GetController();
	CE1_ASSERT(pController&&time);

	Key* pKey;
	bool down;
	bool* pDown;
	CE1_LISTEXEC(
		pController->pActiveControls->pKeys,
		pKey = (Key*)List_Get(itr);
		down = KEY_DOWN(pKey->keycode);
		_NEW(bool, pDown);
		*pDown = down;
		if(pKey->down != down) {
			EventManager_QueueEvent(pKey->eventid, pDown);
			pKey->down = down;
		}
	);

	return S_OK;
}

HRESULT Controller_Delete()
{
	Controller* pController = Engine_GetController();
	if (!pController) { return S_OK; }
	
	Controls* pControls;
	Key* pKey;
	if (List_Length(pController->pControls) > 0) {
		CE1_LISTEXEC(
			pController->pControls,
			pControls = (Controls*)List_Get(itr);
			if (List_Length(pControls->pKeys) > 0) {
				CE1_CALL(List_FullDelete(pControls->pKeys,true));
			}
		);
	}
	CE1_CALL(List_FullDelete(pController->pControls,true));
	CE1_DEL(pController);
	return S_OK;
}

HRESULT Controller_SetControls(ID id)
{
	Controller* pController = Engine_GetController();
	CE1_ASSERT(pController);
	CE1_CALL(List_Get(pController->pControls,id, (void**)(&pController->pActiveControls)));
	return S_OK;
}

Controller* Controller_New()
{
	Controller* pController;
	_NEW(Controller, pController);
	pController->pControls = List_New(sizeof(Controls*));
	pController->idcounter = 0;
	return pController;
}

HRESULT Controller_AddControl(ID controlsid, KEYCODE keycode, ID eventid)
{
	Controller* pController = Engine_GetController();
	CE1_ASSERT(pController);
	Controls* pControls;
	CE1_CALL(List_Get(pController->pControls,controlsid,(void**)&pControls));
	CE1_NEW(Key,pKey);
	pKey->eventid = eventid;
	pKey->down = false;
	pKey->keycode = keycode;
	CE1_CALL(List_PushBack(pControls->pKeys,pKey));
	return S_OK;
}

ID Controller_NewControls() 
{
	Controller* pController = Engine_GetController();
	CE1_ASSERT(pController);
	pController->idcounter++;
	Controls* pControls;
	_NEW(Controls, pControls);
	pControls->id = pController->idcounter;
	pControls->pKeys = List_New(sizeof(Key*));
	CE1_CALL(List_PushBack(pController->pControls, pControls));
	return pControls->id;
}