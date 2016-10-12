#ifndef INCLUDE_TIMER
#define INCLUDE_TIMER

class Timer : public CoreComponent {
private:
	TIME mThen;
	static Timer* mpInstance;
	Timer() {};
	~Timer() {};
public:
	TIME getElapsed();
	TIME getTime() { return (clock() * 1000 / CLOCKS_PER_SEC); };
	HRESULT wait(TIME time) { Sleep(time); };

	HRESULT restore()override { srand(time(NULL)); mThen = getTime(); };
	HRESULT run(TIME elapsed)override { wait(CORETICKSPEED - elapsed); };

	static Timer* get() { if (!mpInstance) { mpInstance = new Timer(); } };
};

#endif 