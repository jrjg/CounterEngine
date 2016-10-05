#ifndef INCLUDE_PROCESSMANAGER
#define INCLUDE_PROCESSMANAGER

typedef HRESULT(*PCB)(TIME);

class Process : public MemManaged
{
private:
	PCB mpCallBack;
	TIME mWait;
	TIME mWaited;
	bool mRunning;
	ID mID;
public:
	Process(PCB pCallBack, TIME wait, ID id) : mpCallBack(mpCallBack), mWait(mWait), mWaited(0), mRunning(true), mID(id) {};
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
	ID newProcess(PCB, TIME);
	HRESULT deleteProcess(ID);
	static HRESULT run(TIME);
	static ProcessManager* get();
	HRESULT pauseProcess(ID processID) { ((Process*)mpProcesses->getByID(processID))->pause(); return S_OK; };
	HRESULT continueProcess(ID processID) { ((Process*)mpProcesses->getByID(processID))->proceed(); return S_OK; };;
};

#endif