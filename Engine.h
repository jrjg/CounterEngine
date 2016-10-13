#ifndef INCLUDE_ENGINE
#define INCLUDE_ENGINE

#include "CoreComponent.h"

class Engine : public CoreComponent
{
private:
	bool mRunning;
	Engine() {};
protected:
	virtual ~Engine() { };
public:
	HRESULT run(TIME elapsed) override;
	HRESULT restore();
	void release()override { mRunning = false; };
	void manualRelease() { delete this; };

	static Engine* get();
};

#endif