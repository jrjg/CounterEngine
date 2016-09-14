#ifndef INCLUDE_EVENTMANAGER
#define INCLUDE_EVENTMANAGER



struct Event;
struct EventManager;
struct EventListener;
typedef struct EventManager EventManager;
typedef struct Event Event;
typedef struct EventListener EventListener;

typedef HRESULT(*ECB)(void*); //event callback

EventManager* EventManager_New(void);
HRESULT EventManager_Delete();
ID EventManager_QueueEvent(ID,void*);
HRESULT EventManager_RegisterEvent(ID);
HRESULT EventManager_UnRegisterEvent(ID);
ID EventManager_RegisterForEvent(ID, ECB);
HRESULT EventManager_UnRegisterForEvent(ID);
HRESULT EventManager_Run(TIME);
HRESULT EventManager_TriggerEvent(ID, void*);
HRESULT EventManager_RemoveEvent(ID);


#endif