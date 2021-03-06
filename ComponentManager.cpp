//#include "TopInclude.h"
//#include "Resource.h"
//#include "Vector.h"
//#include "List.h"
//#include "Engine.h"
//#include "Parser.h"
//#include "ComponentManager.h"
//
//ComponentManager * ComponentManager_NEW()
//{
//	ComponentManager * pCMPMGR;
//	_NEW(ComponentManager, pCMPMGR);
//	pCMPMGR->pComponents = Vector_New(sizeof(Component*),200);
//	pCMPMGR->pComponentHandlers = List_New(sizeof(ComponentHandler*));
//	return pCMPMGR;
//}
//
//HRESULT ComponentManager_DELETE()
//{
//	ComponentManager* pCM = Engine_GetComponentManager();
//	if (!pCM) {
//		return S_OK;
//	}
//	CE1_CALL(ComponentManager_FullDestroy());
//	CE1_CALL(Vector_Delete(pCM->pComponents));
//	CE1_CALL(List_FullDelete(pCM->pComponentHandlers,true));
//	CE1_DEL(pCM);
//	return S_OK;
//}
//
//HRESULT ComponentManager_FullDestroy()
//{
//	ComponentManager* pCM = Engine_GetComponentManager();
//	if (!pCM) {
//		return S_OK;
//	}
//	for (UINT i = 0; i < Vector_Last(pCM->pComponents); i++)
//	{
//		ComponentManager_DestroyComponent(i); //dont CE1_CALL
//	}
//	return S_OK;
//}
//
//HRESULT ComponentManager_RegisterComponentHandler(char * name, CCF pCreator, CDF pDestroyer)
//{
//	ComponentManager* pCM = Engine_GetComponentManager();
//	CE1_ASSERT(pCM&&name&&pCreator&&pDestroyer);
//	ComponentHandler* pComponentHandler;
//	_NEW(ComponentHandler, pComponentHandler);
//	pComponentHandler->name = name;
//	pComponentHandler->pCreator = pCreator;
//	pComponentHandler->pDestroyer = pDestroyer;
//	List_PushBack(pCM->pComponentHandlers, (void*)pComponentHandler);
//	return S_OK;
//}
//
//HRESULT ComponentManager_CreateComponent(char * name, void * pCreatorDesc, ID * pID)
//{
//	ComponentManager* pCM = Engine_GetComponentManager();
//	CE1_ASSERT(pCM&&name&&pCreatorDesc&&pID);
//
//	ComponentHandler* pComponentHandler;
//	CCF pCCF;
//
//	//get the Componenthandler
//	CE1_LISTEXEC(
//		pCM->pComponentHandlers, pComponentHandler = (ComponentHandler*)List_Get(itr);
//		if (CHAREQ(pComponentHandler->name, name)) {
//		pCCF = pComponentHandler->pCreator;});
//
//	CE1_ASSERT(pCCF && "Component type not supported");
//
//	//create the component
//	Component* pC;
//	_NEW(Component, pC);
//	pC->name = name;
//
//	//create the componentdata
//	CE1_CALL((*pCCF)(pCreatorDesc, &(pC->pData)));
//	CE1_ASSERT(pC->pData && "Component could not be created");
//
//	//store component
//	return Vector_Pushback(pCM->pComponents, pC);
//	return S_OK;
//}
//
//HRESULT ComponentManager_DestroyComponent(ID id)
//{
//	ComponentManager* pCM = Engine_GetComponentManager();
//	CE1_ASSERT(pCM&&id);
//
//	//get the component
//	Component* pC;
//	CE1_CALL(ComponentManager_GetComponent(id, (void**)(&pC) ));
//
//	//get the destroyer
//	ComponentHandler* pComponentHandler;
//	CDF pDestroyer;
//	CE1_LISTEXEC(
//		pCM->pComponentHandlers, pComponentHandler = (ComponentHandler*)List_Get(itr);
//		if (CHAREQ(pComponentHandler->name, pC->name)) {
//		pDestroyer = pComponentHandler->pDestroyer;});
//
//	CE1_ASSERT(pDestroyer && "Component type not supported");
//
//	//delete component data
//	CE1_CALL((*pDestroyer)(pC->pData));
//
//	//delete compoment
//	CE1_DEL(pC->name);
//	CE1_DEL(pC);
//	return S_OK;
//}
//
//HRESULT ComponentManager_GetComponent(ID id, void ** ppData)
//{
//	ComponentManager* pCM = Engine_GetComponentManager();
//	CE1_ASSERT(pCM&&id);
//	Component* pC = (Component*)Vector_Get(pCM->pComponents, id);
//	CE1_ASSERT(pC);
//	(*ppData) = pC->pData;
//	return S_OK;
//}