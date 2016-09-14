#ifndef INCLUDE_PROCESSMANAGER
#define INCLUDE_PROCESSMANAGER


typedef HRESULT(*PCB)(TIME);		// Process callback
struct Process;
struct ProcessManager;
typedef struct Process Process;
typedef struct ProcessManager ProcessManager;
ID ProcessManager_NewProcess(PCB, TIME);
HRESULT ProcessManager_DeleteProcess(ID);
HRESULT ProcessManager_Delete(void);
ProcessManager* ProcessManager_New(void);
HRESULT ProcessManager_Run(TIME);
HRESULT ProcessManager_RunProcess(Process*, TIME);

#endif