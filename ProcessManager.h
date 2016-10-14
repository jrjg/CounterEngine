#ifndef INCLUDE_PROCESSMANAGER
#define INCLUDE_PROCESSMANAGER

#include "CoreComponent.h"
#include "List.h"
#include "Process.h"

class ProcessManager : public CoreComponent
{
private:
	List<Process>* mpProcesses;
	ProcessManager();
protected:
	virtual ~ProcessManager() { SAFE_RELEASE(mpProcesses); };
public:
	ID addProcess(Process* pProcess) { return mpProcesses->pushBack(pProcess); };
	HRESULT removeProcess(ID processID) { return mpProcesses->deleteByID(processID); };
	static ProcessManager* get();
	HRESULT restore();
	HRESULT run(TIME elapsed)override;
};

#endif