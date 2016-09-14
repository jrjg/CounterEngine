#ifndef INCLUDE_TIMER
#define INCLUDE_TIMER

struct Timer;
typedef struct Timer Timer;

TIME Timer_Elapsed(void);
TIME Timer_Time(void);
Timer* Timer_New(void);
HRESULT Timer_Wait(TIME);
HRESULT Timer_Delete(void);

#endif 