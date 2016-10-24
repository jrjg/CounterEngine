#ifndef INCLUDE_TIMER
#define INCLUDE_TIMER

#include "CoreComponent.h"
#include "Singleton.h"

class Timer : public CoreComponent, public Singleton<Timer>{
	friend class Singleton<Timer>;
private:
	TIME mThen;
	Timer() { restore(); };
protected:
	virtual ~Timer() {};
public:
	TIME getElapsed();
	TIME getTime();
	HRESULT wait(TIME time);
	HRESULT restore();
	HRESULT run(TIME elapsed)override;
};

#endif 