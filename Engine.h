#ifndef INCLUDE_ENGINE
#define INCLUDE_ENGINE

#include "CoreComponent.h"
#include "Singleton.h"

class Engine : public CoreComponent,public Singleton<Engine>
{
	friend class Singleton<Engine>;
private:
	bool mRunning;
	Engine() { restore(); };
protected:
	virtual ~Engine() { };
public:
	HRESULT run(TIME elapsed) override { return S_OK; };
	HRESULT restore();
	void Release()override { mRunning = false; };
	bool isRunning() { return mRunning; };
	void manualRelease() { delete this; };
};

#endif