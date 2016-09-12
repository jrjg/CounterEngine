#include "TopInclude.h"
#include "Vector.h"
#include "List.h"
#include "Engine.h"
#include "Resource.h"



ResourceManager * ResourceManager_NEW()
{
	ResourceManager *pRM;
	_NEW(ResourceManager, pRM);
	pRM->pResourceHandlers = List_New(sizeof(FormatHandler*));
	pRM->pResources = Vector_New(sizeof(Resource*), 300);
	return pRM;
}

HRESULT ResourceManager_DELETE()
{
	ResourceManager* pRM = Engine_GetResourceManager();
	SAFECALL(ResourceManager_FullUnLoad());
	Vector_Delete(pRM->pResources);
	List_FullDelete(pRM->pResourceHandlers);
	_DEL(pRM);
	return S_OK;
}

ID ResourceManager_LoadResource(char* name)
{
	ResourceManager* pRM = Engine_GetResourceManager();
	FormatHandler* pFormatHandler;
	FH pFH;

	//get the formatname
	char formatname[3];
	SAFECALL(ResourceManager_GetFormatName(name, formatname));

	//get the formathandler
	ExecOnList(
		pRM->pResourceHandlers, pFormatHandler = (FormatHandler*)List_Get(itr);  
		if (CHAREQ(pFormatHandler->formatname,formatname)) {
			pFH = pFormatHandler->pFormatHandler;
		});
	CEASSERT(pFH && "Format not supported");

	//create the resource
	Resource* pRes;
	_NEW(Resource, pRes);
	pRes->name = name;

	//create the resourceextra
	SAFECALL((*pFH)(name,&(pRes->ResourceExtra)));
	CEASSERT(pRes->ResourceExtra && "Resource could not be loaded");

	//store resource
	return Vector_Pushback(pRM->pResources, pRes);
}

HRESULT ResourceManager_GetFormatName(char* from, char* to) {
	size_t len = strlen(from);
	for (UINT n = 0; n<2; n++) { to[n] = from[len - n]; }
	return S_OK;
}

HRESULT ResourceManager_UnLoadResource(ID id)
{
	ResourceManager* pRM = Engine_GetResourceManager();

	//get the resource
	Resource* pRes;
	SAFECALL(ResourceManager_GetResource(id, ((void**)(&pRes)) ));

	//get the formatname
	char formatname[3];
	SAFECALL(ResourceManager_GetFormatName(pRes->name, formatname));

	//get the xformathandler
	FormatHandler* pFormatHandler;
	xFH pXFH;
	ExecOnList(
		pRM->pResourceHandlers, pFormatHandler = (FormatHandler*)List_Get(itr);
		if (CHAREQ(pFormatHandler->formatname, formatname)) {
		pXFH = pFormatHandler->pXFormatHandler;
	});
	CEASSERT(pXFH && "Format not supported");

	//delete resourceextra
	SAFECALL((*pXFH)(pRes->ResourceExtra));

	//delete resource
	_DEL(pRes->name);
	_DEL(pRes);
	return S_OK;
}

HRESULT ResourceManager_FullUnLoad()
{
	ResourceManager* pRM = Engine_GetResourceManager();
	for (UINT i = 0; i < Vector_Last(pRM->pResources); i++)
	{
		ResourceManager_UnLoadResource(i); //dont safecall
	}
	return S_OK;
}

HRESULT ResourceManager_GetResource(ID id, void ** ppRes)
{
	ResourceManager* pRM = Engine_GetResourceManager();
	Resource* pRes = (Resource*)Vector_Get(pRM->pResources, id);
	if (!pRes) {
		return ERROR_SUCCESS;
	}
	(*ppRes) = pRes->ResourceExtra;
	return S_OK;
}

HRESULT ResourceManager_RegisterFormatHandler(char* name, FH pFH, xFH pXFH)
{
	ResourceManager* pRM = Engine_GetResourceManager();
	FormatHandler* pFormatHandler;
	_NEW(FormatHandler,pFormatHandler);
	pFormatHandler->pFormatHandler = pFH;
	pFormatHandler->pXFormatHandler = pXFH;
	pFormatHandler->formatname = name;
	List_PushBack(pRM->pResourceHandlers,(void*)pFormatHandler);
	return S_OK;
}