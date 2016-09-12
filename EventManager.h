#ifndef INCLUDE_EVENTMANAGER
#define INCLUDE_EVENTMANAGER



struct Event;
struct EventManager;
struct EventListener;
typedef struct EventManager EventManager;
typedef struct Event Event;
typedef struct EventListener EventListener;

typedef BOOL(*ECB)(void*); //event callback

EventManager* EventManager_New(void);
BOOL EventManager_Delete();
ID EventManager_QueueEvent(ID,void*);
BOOL EventManager_RegisterEvent(ID);
BOOL EventManager_UnRegisterEvent(ID);
ID EventManager_RegisterForEvent(ID, ECB);
BOOL EventManager_UnRegisterForEvent(ID);
BOOL EventManager_Run(TIME);
BOOL EventManager_TriggerEvent(ID, void*);
BOOL EventManager_RemoveEvent(ID);


#endif