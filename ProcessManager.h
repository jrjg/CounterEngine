#ifndef INCLUDE_PROCESSMANAGER
#define INCLUDE_PROCESSMANAGER

#include "CoreComponent.h"
#include "List.h"
#include "Process.h"
#include "Singleton.h"

class ProcessManager : public CoreComponent, public Singleton<ProcessManager>
{
	friend class Singleton<ProcessManager>;
private:
	List<Process>* mpProcesses;
	ProcessManager();
protected:
	virtual ~ProcessManager() { SAFE_RELEASE(mpProcesses); };
public:
	ID addProcess(Process* pProcess) { return mpProcesses->pushBack(pProcess); };
	HRESULT removeProcess(ID processID) { return mpProcesses->deleteByID(processID); };
	HRESULT restore();
	HRESULT run(TIME elapsed)override;

	void Release()override { mpProcesses->restore(); /*remove all Processes*/ };
	void ManualRelease(){ delete this; };

	static ProcessManager* get();
};

#endif