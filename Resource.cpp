//#include "TopInclude.h"
//#include "Vector.h"
//#include "List.h"
//#include "Engine.h"
//#include "Resource.h"
//
//
//
//ResourceManager * ResourceManager_NEW()
//{
//	ResourceManager *pRM;
//	_NEW(ResourceManager, pRM);
//	pRM->pResourceHandlers = List_New(sizeof(FormatHandler*));
//	pRM->pResources = Vector_New(sizeof(Resource*), 300);
//	return pRM;
//}
//
//HRESULT ResourceManager_DELETE()
//{
//	ResourceManager* pRM = Engine_GetResourceManager();
//	CE1_ASSERT(pRM);
//	CE1_CALL(ResourceManager_FullUnLoad());
//	CE1_CALL(Vector_Delete(pRM->pResources));
//	CE1_CALL(List_FullDelete(pRM->pResourceHandlers,true));
//	CE1_DEL(pRM);
//	return S_OK;
//}
//
//HRESULT ResourceManager_LoadResource(char* name,ID* pID)
//{
//	ResourceManager* pRM = Engine_GetResourceManager();
//	CE1_ASSERT(pRM&&name);
//	FormatHandler* pFormatHandler;
//	FH pFH;
//
//	//get the formatname
//	char formatname[3];
//	CE1_CALL(ResourceManager_GetFormatName(name, formatname));
//
//	//get the formathandler
//	CE1_LISTEXEC(
//		pRM->pResourceHandlers, pFormatHandler = (FormatHandler*)List_Get(itr);  
//		if (CHAREQ(pFormatHandler->formatname,formatname)) {
//			pFH = pFormatHandler->pFormatHandler;
//		});
//	CE1_ASSERT(pFH && "Format not supported");
//
//	//create the resource
//	Resource* pRes;
//	_NEW(Resource, pRes);
//	pRes->name = name;
//
//	//create the resourceextra
//	CE1_CALL((*pFH)(name,&(pRes->ResourceExtra)));
//	CE1_ASSERT(pRes->ResourceExtra && "Resource could not be loaded");
//
//	//store resource
//	*pID = Vector_Pushback(pRM->pResources, pRes);
//	return S_OK;
//}
//
//HRESULT ResourceManager_LoadResource(String * pName, ID * pID)
//{
//	CE1_CALL(ResourceManager_LoadResource(name->pBuffer, pID));
//	return S_OK;
//}
//
//HRESULT ResourceManager_LoadResource(WString * pName, ID * pID)
//{
//	char* pCharName = (char*)malloc(sizeof(char)*pName->length);
//	wcstombs(pCharName, pName->pBuffer, pName->length);
//	CE1_CALL(ResourceManager_LoadResource(pCharName,pID));
//	return E_NOTIMPL;
//}
//
//HRESULT ResourceManager_GetFormatName(char* from, char* to) {
//	CE1_ASSERT(from&&to);
//	size_t len = strlen(from);
//	for (UINT n = 0; n<2; n++) { to[n] = from[len - n]; }
//	return S_OK;
//}
//
//HRESULT ResourceManager_UnLoadResource(ID id)
//{
//	ResourceManager* pRM = Engine_GetResourceManager();
//	CE1_ASSERT(id&&pRM);
//
//	//get the resource
//	Resource* pRes;
//	CE1_CALL(ResourceManager_GetResource(id, ((void**)(&pRes)) ));
//
//	//get the formatname
//	char formatname[3];
//	CE1_CALL(ResourceManager_GetFormatName(pRes->name, formatname));
//
//	//get the xformathandler
//	FormatHandler* pFormatHandler;
//	xFH pXFH;
//	CE1_LISTEXEC(
//		pRM->pResourceHandlers, pFormatHandler = (FormatHandler*)List_Get(itr);
//		if (CHAREQ(pFormatHandler->formatname, formatname)) {
//		pXFH = pFormatHandler->pXFormatHandler;
//	});
//	CE1_ASSERT(pXFH && "Format not supported");
//
//	//delete resourceextra
//	CE1_CALL((*pXFH)(pRes->ResourceExtra));
//
//	//delete resource
//	CE1_DEL(pRes->name);
//	CE1_DEL(pRes);
//	return S_OK;
//}
//
//HRESULT ResourceManager_FullUnLoad()
//{
//	ResourceManager* pRM = Engine_GetResourceManager();
//	CE1_ASSERT(pRM);
//	for (UINT i = 0; i < Vector_Last(pRM->pResources); i++)
//	{
//		ResourceManager_UnLoadResource(i); //dont CE1_CALL
//	}
//	return S_OK;
//}
//
//HRESULT ResourceManager_GetResource(ID id, void ** ppRes)
//{
//	ResourceManager* pRM = Engine_GetResourceManager();
//	CE1_ASSERT(pRM&&id&&*ppRes);
//	Resource* pRes = (Resource*)Vector_Get(pRM->pResources, id);
//	CE1_ASSERT(pRes);
//	(*ppRes) = pRes->ResourceExtra;
//	return S_OK;
//}
//
//HRESULT ResourceManager_RegisterFormatHandler(char* name, FH pFH, xFH pXFH)
//{
//	ResourceManager* pRM = Engine_GetResourceManager();
//	CE1_ASSERT(pRM&&name&&pFH&&pXFH);
//	FormatHandler* pFormatHandler;
//	_NEW(FormatHandler,pFormatHandler);
//	pFormatHandler->pFormatHandler = pFH;
//	pFormatHandler->pXFormatHandler = pXFH;
//	pFormatHandler->formatname = name;
//	List_PushBack(pRM->pResourceHandlers,(void*)pFormatHandler);
//	return S_OK;
//}