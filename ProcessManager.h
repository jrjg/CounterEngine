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
	ProcessManager() : CoreComponent(false) { restore(); };
protected:
	virtual ~ProcessManager() { SAFE_RELEASE(mpProcesses); };
public:
	HRESULT addProcess(Process* pProcess, ID* pID);
	HRESULT removeProcess(ID processID);
	HRESULT restore();
	HRESULT run(TIME elapsed)override;

	HRESULT Release()override { return S_OK; };
	void ManualRelease(){ delete this; };

	static ProcessManager* get();
};

#endif