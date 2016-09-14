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
	CEASSERT(pRM);
	SAFECALL(ResourceManager_FullUnLoad());
	SAFECALL(Vector_Delete(pRM->pResources));
	SAFECALL(List_FullDelete(pRM->pResourceHandlers,true));
	_DEL(pRM);
	return S_OK;
}

ID ResourceManager_LoadResource(char* name)
{
	ResourceManager* pRM = Engine_GetResourceManager();
	CEASSERT(pRM&&name);
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
	CEASSERT(from&&to);
	size_t len = strlen(from);
	for (UINT n = 0; n<2; n++) { to[n] = from[len - n]; }
	return S_OK;
}

HRESULT ResourceManager_UnLoadResource(ID id)
{
	ResourceManager* pRM = Engine_GetResourceManager();
	CEASSERT(id&&pRM);

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
	CEASSERT(pRM);
	for (UINT i = 0; i < Vector_Last(pRM->pResources); i++)
	{
		ResourceManager_UnLoadResource(i); //dont safecall
	}
	return S_OK;
}

HRESULT ResourceManager_GetResource(ID id, void ** ppRes)
{
	ResourceManager* pRM = Engine_GetResourceManager();
	CEASSERT(pRM&&id&&*ppRes);
	Resource* pRes = (Resource*)Vector_Get(pRM->pResources, id);
	CEASSERT(pRes);
	(*ppRes) = pRes->ResourceExtra;
	return S_OK;
}

HRESULT ResourceManager_RegisterFormatHandler(char* name, FH pFH, xFH pXFH)
{
	ResourceManager* pRM = Engine_GetResourceManager();
	CEASSERT(pRM&&name&&pFH&&pXFH);
	FormatHandler* pFormatHandler;
	_NEW(FormatHandler,pFormatHandler);
	pFormatHandler->pFormatHandler = pFH;
	pFormatHandler->pXFormatHandler = pXFH;
	pFormatHandler->formatname = name;
	List_PushBack(pRM->pResourceHandlers,(void*)pFormatHandler);
	return S_OK;
}