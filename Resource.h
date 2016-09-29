#ifndef RESOURCE_INC
#define RESOURCE_INC
typedef HRESULT(*FH)(char*, void**);
typedef HRESULT(*xFH)(void*);

struct Resource {
	char* name;
	void* ResourceExtra;
};
typedef struct Resource Resource;

struct FormatHandler {
	char* formatname;
	FH pFormatHandler;
	xFH pXFormatHandler;
};
typedef struct FormatHandler FormatHandler;

struct ResourceManager {
	struct Vector* pResources;
	struct List* pResourceHandlers;
};
typedef struct ResourceManager ResourceManager;



ResourceManager* ResourceManager_NEW();
HRESULT ResourceManager_DELETE();
HRESULT ResourceManager_LoadResource(char* name, ID* pID);
HRESULT ResourceManager_LoadResource(String* pName, ID* pID);
HRESULT ResourceManager_LoadResource(WString* pName, ID* pID);
HRESULT ResourceManager_UnLoadResource(ID id);
HRESULT ResourceManager_FullUnLoad();
HRESULT ResourceManager_GetResource(ID id, void** ppRes);
HRESULT ResourceManager_RegisterFormatHandler(char* name, FH pFH, xFH pXFH);
HRESULT ResourceManager_GetFormatName(char* from, char* to);
#endif