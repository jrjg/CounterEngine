#ifndef INCLUDE_PROCESSMANAGER
#define INCLUDE_PROCESSMANAGER

class Process : public MemManaged
{
private:
	ProcessHandler* mpProcessHandler;
	TIME mWait;
	TIME mWaited;
	bool mRunning;
	ID mID;
public:
	Process(ProcessHandler* pProcessHandler, TIME wait, ID id) : mpProcessHandler(pProcessHandler), mWait(mWait), mWaited(0), mRunning(true), mID(id) {};
	bool isRunning() { return mRunning; };
	void pause() { mRunning = false; };
	void proceed() { mRunning = true; };
	ID getID() { return mID; };
	HRESULT run(TIME elapsed);
};

class ProcessManager : public MemManaged
{
private:
	List<Process>* mpProcesses;
	Vector<SimplyManaged<ID>>* mpIDs;
	ID mIDCounter;
	ProcessManager();
	~ProcessManager() { delete mpProcesses; delete mpIDs; };
public:
	ID newProcess(ProcessHandler* pProcessHandler, TIME);
	HRESULT deleteProcess(ID);
	HRESULT pauseProcess(ID processID) { mpProcesses->getByID(processID)->pause(); return S_OK; };
	HRESULT continueProcess(ID processID) { mpProcesses->getByID(processID)->proceed(); return S_OK; };;
};

#endif