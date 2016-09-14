#include "CLibrary.h"
#include "TopInclude.h"
#include "Vector.h"
#include "EventManager.h"
#include "Engine.h"
#include "Controller.h"


struct Controls
{
	Vector* _pControlVector;
	ID _id;
};

struct Controller
{
	Vector* _pControlsVector;
	Controls* _pActiveControls;
	ID _idcounter;
};

HRESULT Controller_Run(TIME time)
{
	Controller* pController = Engine_GetController();
	CEASSERT(pController&&time);
	/*if (_kbhit())
	{
		ID* pEventid = (ID*)Vector_Get(pController->_pActiveControls->_pControlVector,_getch());
		if (pEventid)
		{
			EventManager_QueueEvent((*pEventid), NULL);
		}
	}*/
	return S_OK;
}

HRESULT Controller_EvalKey(void* pData) {
	Controller* pController = Engine_GetController();
	CEASSERT(pController&&pData);
	ID* pEventid = (ID*)Vector_Get(pController->_pActiveControls->_pControlVector, *(WPARAM*)pData);
	if (pEventid)
	{
		EventManager_QueueEvent((*pEventid), NULL);
	}
	return S_OK;
}

HRESULT Controller_Delete()
{
	Controller* pController = Engine_GetController();
	CEASSERT(pController);
	if (Vector_Elements(pController->_pControlsVector)>0)
	{
		Vector* pControlVector = NULL;
		Controls* pControls = NULL;
		for (unsigned int i = 0; i<Vector_Last(pController->_pControlsVector);i++)
		{
			pControls = (Controls*)Vector_Get(pController->_pControlsVector,i);
			if (pControls){
				pControlVector = pControls->_pControlVector;
				SAFECALL(Vector_FullDelete(pControlVector));
			}
		}
	}
	SAFECALL(Vector_FullDelete(pController->_pControlsVector));
	_DEL(pController);
	return S_OK;
}

HRESULT Controller_SetControls(ID controlsid)
{
	Controller* pController = Engine_GetController();
	CEASSERT(pController&&controlsid);
	Controls* pControls = (Controls*)Vector_Get(pController->_pControlsVector,controlsid);
	CEASSERT(pControls);
	pController->_pActiveControls = pControls;
	return S_OK;
}

Controller* Controller_New()
{
	Controller* pController;
	_NEW(Controller, pController);
	pController->_pControlsVector = Vector_New(sizeof(void*),20);
	pController->_idcounter = 0;
	return pController;
}

HRESULT Controller_AddControl(ID controlsid, KEYCODE key, ID eventid)
{
	Controller* pController = Engine_GetController();
	CEASSERT(pController&&controlsid&&key&&eventid);
	Controls* pControls = (Controls*)Vector_Get(pController->_pControlsVector,controlsid);
	ID* pEventID;
	_NEW(ID, pEventID);
	(*pEventID) = eventid;
	SAFECALL(Vector_Insert(pControls->_pControlVector,key,pEventID));
	return S_OK;
}

ID Controller_NewControls() //memory allocation missing
{
	Controller* pController = Engine_GetController();
	CEASSERT(pController);
	pController->_idcounter++;
	Controls* pControls;
	_NEW(Controls, pControls);
	pControls->_id = pController->_idcounter;
	pControls->_pControlVector = Vector_New(sizeof(void*),255);
	SAFECALL(Vector_Insert(pController->_pControlsVector, pControls->_id, pControls));
	return pControls->_id;
}