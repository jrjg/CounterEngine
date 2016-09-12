#ifndef INCLUDE_CONTROLLER
#define INCLUDE_CONTROLLER


struct Controls;
struct Controller;
typedef struct Controls Controls;
typedef struct Controller Controller;
Controller* Controller_New(void);
BOOL Controller_Delete();
BOOL Controller_SetControls(ID);
ID Controller_NewControls();
BOOL Controller_AddControl(ID, KEYCODE, ID);
BOOL Controller_Run(TIME);
BOOL Controller_EvalKey(void*);

#endif