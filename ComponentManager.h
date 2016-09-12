#ifndef CMPM_INC
#define CMPM_INC

typedef HRESULT(*CCF)(void*, void**);
typedef HRESULT(*CDF)(void*);

struct Component {
	char* name;
	void* pData;
};
typedef struct Component Component;

struct ComponentHandler {
	char* name;
	CCF pCreator;
	CDF pDestroyer;
};
typedef struct ComponentHandler ComponentHandler;

struct ComponentManager {
	struct Vector* pComponents;
	struct List* pComponentHandlers;
};
typedef struct ComponentManager ComponentManager;


ComponentManager* ComponentManager_NEW();
HRESULT ComponentManager_DELETE();
HRESULT ComponentManager_RegisterComponentHandler(char* name, CCF pCreator, CDF pDestroyer);
HRESULT ComponentManager_CreateComponent(char* name, void* pCreatorDesc, ID* pID);
HRESULT ComponentManager_DestroyComponent(ID id);
HRESULT ComponentManager_GetComponent(ID id, void** ppData);
HRESULT ComponentManager_FullDestroy();
#endif
