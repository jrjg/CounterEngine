#include "TopInclude.h"
#include "List.h"
#include "Vector.h"
#include "Timer.h"
#include "Engine.h"
#include "ProcessManager.h"

struct Process
{
	PCB _pCallBack;
	TIME _wait;
	TIME _waited;
	ID _id;
};

struct ProcessManager
{
	List* _pProcessList;
	Vector* _pIDVector;
	ID _idcnt;
};

ProcessManager* ProcessManager_New(void)
{
	ProcessManager* pPM;
	_NEW(ProcessManager, pPM);
	if (!pPM)
	{
		return NULL;
	}
	pPM->_pProcessList = List_New(sizeof(Process*));
	pPM->_pIDVector = Vector_New(sizeof(ID*), 300);
	pPM->_idcnt = 0;
	return pPM;
}

ID ProcessManager_NewProcess(PCB pCallBack, TIME wait)
{
	ProcessManager* pPM = Engine_GetPM();
	CE1_ASSERT(pPM&&pCallBack);
	Process* pProcess;
	_NEW(Process, pProcess);
	pProcess->_pCallBack = pCallBack;
	pPM->_idcnt++;
	pProcess->_id = pPM->_idcnt;
	pProcess->_wait = wait;
	pProcess->_waited = 0;
	ID* pListID;
	_NEW(ID, pListID);
	(*pListID) = List_PushBack(pPM->_pProcessList, pProcess);
	CE1_CALL(Vector_Insert(pPM->_pIDVector, pProcess->_id, (void*)pListID));
	return pProcess->_id;
}

HRESULT ProcessManager_DeleteProcess(ID id)
{
	ProcessManager* pPM = Engine_GetPM();
	if (!pPM) {
		return S_OK;
	}
	CE1_CALL(List_DeleteElement(pPM->_pProcessList, *(ID*)Vector_Get(pPM->_pIDVector, id),true));
	CE1_CALL(Vector_DeleteElement(pPM->_pIDVector, id));
	return S_OK;
}

HRESULT ProcessManager_Delete()
{
	ProcessManager* pPM = Engine_GetPM();
	if (!pPM) {
		return S_OK;
	}
	CE1_CALL(List_FullDelete(pPM->_pProcessList,true));
	CE1_CALL(Vector_FullDelete(pPM->_pIDVector));
	CE1_DEL(pPM);
	return S_OK;
}

HRESULT ProcessManager_RunProcess(Process* pProcess, TIME elapsed)
{
	CE1_ASSERT(pProcess&&elapsed);
	pProcess->_waited = pProcess->_waited + elapsed;
	if (pProcess->_waited >= pProcess->_wait)
	{
		if (pProcess->_wait == 0)
		{
			(*pProcess->_pCallBack)(pProcess->_wait);
		}
		else
		{
			for (; pProcess->_waited >= pProcess->_wait; pProcess->_waited = pProcess->_waited - pProcess->_wait)
			{
				(*pProcess->_pCallBack)(pProcess->_wait);
			}
		}
	}
	return S_OK;
}

HRESULT ProcessManager_Run(TIME elapsed)
{
	ProcessManager* pPM = Engine_GetPM();
	CE1_ASSERT(elapsed&&pPM);
	if (List_Length(pPM->_pProcessList) > 0)
	{
		for (Iterator itr = List_Iterator(pPM->_pProcessList); itr != NULL; itr = List_Next(itr))
		{
			CE1_CALL(ProcessManager_RunProcess((Process*)List_Get(itr), elapsed));
		}
	}
	return S_OK;
}