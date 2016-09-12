#ifndef INCLUDE_TIMER
#define INCLUDE_TIMER

struct Timer;
typedef struct Timer Timer;

TIME Timer_Elapsed(void);
TIME Timer_Time(void);
Timer* Timer_New(void);
BOOL Timer_Wait(TIME);
BOOL Timer_Delete(void);

#endif 