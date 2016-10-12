#ifndef INCLUDE_PROCESSMANAGER
#define INCLUDE_PROCESSMANAGER

class ProcessManager : public CoreComponent
{
private:
	static ProcessManager* mpInstance;
	List<Process>* mpProcesses;
	ProcessManager() : CoreComponent(false) {};
	~ProcessManager() { delete mpProcesses; };
public:
	ID addProcess(Process* pProcess) { return mpProcesses->pushBack(pProcess); };
	HRESULT removeProcess(ID processID) { return mpProcesses->deleteByID(processID); };
	static ProcessManager* get();
	HRESULT restore();
	HRESULT run(TIME elapsed)override;
};

#endif