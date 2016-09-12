#ifndef INCLUDE_PROCESSMANAGER
#define INCLUDE_PROCESSMANAGER


typedef BOOL(*PCB)(TIME);		// Process callback
struct Process;
struct ProcessManager;
typedef struct Process Process;
typedef struct ProcessManager ProcessManager;
ID ProcessManager_NewProcess(PCB, TIME);
BOOL ProcessManager_DeleteProcess(ID);
BOOL ProcessManager_Delete(void);
ProcessManager* ProcessManager_New(void);
BOOL ProcessManager_Run(TIME);
BOOL ProcessManager_RunProcess(Process*, TIME);

#endif