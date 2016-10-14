#ifndef INCLUDE_ENGINE
#define INCLUDE_ENGINE

#include "CoreComponent.h"

class Engine : public CoreComponent
{
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

	static Engine* get();
};

#endif