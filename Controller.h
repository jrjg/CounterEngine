#ifndef INCLUDE_CONTROLLER
#define INCLUDE_CONTROLLER


struct Controls;
struct Controller;
typedef struct Controls Controls;
typedef struct Controller Controller;
Controller* Controller_New(void);
HRESULT Controller_Delete();
HRESULT Controller_SetControls(ID);
ID Controller_NewControls();
HRESULT Controller_AddControl(ID, KEYCODE, ID);
HRESULT Controller_Run(TIME);
HRESULT Controller_EvalKey(void*);

#endif