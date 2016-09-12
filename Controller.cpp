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

BOOL Controller_Run(TIME time)
{
	/*Controller* pController = Engine_GetController();
	
	if (_kbhit())
	{
		ID* pEventid = (ID*)Vector_Get(pController->_pActiveControls->_pControlVector,_getch());
		if (pEventid)
		{
			EventManager_QueueEvent((*pEventid), NULL);
		}
	}*/
	return ERROR_FAILURE;
}

BOOL Controller_EvalKey(void* pData) {
	Controller* pController = Engine_GetController();
	ID* pEventid = (ID*)Vector_Get(pController->_pActiveControls->_pControlVector, *(WPARAM*)pData);
	if (pEventid)
	{
		EventManager_QueueEvent((*pEventid), NULL);
	}
	return ERROR_FAILURE;
}

BOOL Controller_Delete()
{
	Controller* pController = Engine_GetController();
	BOOL error = ERROR_FAILURE;
	if (Vector_Elements(pController->_pControlsVector)>0)
	{
		Vector* pControlVector = NULL;
		Controls* pControls = NULL;
		for (unsigned int i = 0; i<Vector_Last(pController->_pControlsVector);i++)
		{
			pControls = (Controls*)Vector_Get(pController->_pControlsVector,i);
			if (pControls){
				pControlVector = pControls->_pControlVector;
				error = error || Vector_FullDelete(pControlVector);
			}
		}
	}
	Vector_FullDelete(pController->_pControlsVector);
	_DEL(pController);
	return error;
}

BOOL Controller_SetControls(ID controlsid)
{
	Controller* pController = Engine_GetController();
	Controls* pControls = (Controls*)Vector_Get(pController->_pControlsVector,controlsid);
	if (!pControls)
	{
		return ERROR_SUCCESS;
	}
	pController->_pActiveControls = pControls;
	return ERROR_FAILURE;
}

Controller* Controller_New()
{
	Controller* pController;
	_NEW(Controller, pController);
	pController->_pControlsVector = Vector_New(sizeof(void*),20);
	pController->_idcounter = 0;
	return pController;
}

BOOL Controller_AddControl(ID controlsid, KEYCODE key, ID eventid)
{
	Controller* pController = Engine_GetController();
	BOOL error = ERROR_FAILURE;
	Controls* pControls = (Controls*)Vector_Get(pController->_pControlsVector,controlsid);
	ID* pEventID;
	_NEW(ID, pEventID);
	(*pEventID) = eventid;
	error = Vector_Insert(pControls->_pControlVector,key,pEventID);
	return error;
}

ID Controller_NewControls() //memory allocation missing
{
	Controller* pController = Engine_GetController();
	BOOL error = ERROR_FAILURE;
	pController->_idcounter++;
	Controls* pControls;
	_NEW(Controls, pControls);
	pControls->_id = pController->_idcounter;
	pControls->_pControlVector = Vector_New(sizeof(void*),255);
	error = Vector_Insert(pController->_pControlsVector, pControls->_id, pControls);
	return pControls->_id;
}