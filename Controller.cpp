#include "CLibrary.h"
#include "TopInclude.h"
#include "Vector.h"
#include "EventManager.h"
#include "Engine.h"
#include "Memory.h"
#include "List.h"
#include "CoreComponent.h"

#include "Controller.h"

HRESULT Controller_EvalKeyDown(void* pData) {
	Controller* pController = Engine_GetController();
	CE1_ASSERT(pController&&pData);
	ID* pEventid = (ID*)Vector_Get(pController->_pActiveControls->_pControlVector, *(WPARAM*)pData);
	CE1_NEW(bool,pDown);
	*pDown = true;
	if (pEventid)
	{
		EventManager_QueueEvent((*pEventid), pDown);
	}
	return S_OK;
}

HRESULT Controller_EvalKeyUp(void* pData) {
	Controller* pController = Engine_GetController();
	CE1_ASSERT(pController&&pData);
	ID* pEventid = (ID*)Vector_Get(pController->_pActiveControls->_pControlVector, *(WPARAM*)pData);
	CE1_NEW(bool, pDown);
	*pDown = false;
	if (pEventid)
	{
		EventManager_QueueEvent((*pEventid), pDown);
	}
	return S_OK;
}