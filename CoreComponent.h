#ifndef CORECOMPONENT_INC
#define CORECOMPONENT_INC

#include "MemManaged.h"
#include "EventListener.h"
#include "Process.h"

class RestoreListener;
class ReleaseListener;
class RunHandler;

class CoreComponent : public MemManaged {
protected:
	RestoreListener* mpRestoreListener;
	ReleaseListener* mpReleaseListener;
	RunHandler* mpRunHandler;
	CoreComponent(bool autoRegister) { if (autoRegister) { restore(); }; };
	CoreComponent() { restore(); };
	virtual ~CoreComponent();
public:
	virtual HRESULT run(TIME elapsed) = 0;
	virtual HRESULT restore();
};

class RestoreListener : public EventListener {
private:
	CoreComponent* mpCoreComponent;
protected:
	virtual ~RestoreListener() {};
public:
	HRESULT handle(MemManaged* pData) override { mpCoreComponent->restore(); return S_OK; };
	RestoreListener(CoreComponent* pCoreComponent) : EventListener(EVENT_RESTORE), mpCoreComponent(pCoreComponent) {};
};

class ReleaseListener : public EventListener {
private:
	CoreComponent* mpCoreComponent;
protected:
	virtual ~ReleaseListener() {};
public:
	HRESULT handle(MemManaged* pData) override { mpCoreComponent->release(); return S_OK; };
	ReleaseListener(CoreComponent* pCoreComponent) : EventListener(EVENT_RELEASE), mpCoreComponent(pCoreComponent) {};
};

class RunHandler : public Process {
private:
	CoreComponent* mpCoreComponent;
protected:
	virtual ~RunHandler() {};
public:
	HRESULT handle(TIME elapsed)override { mpCoreComponent->run(elapsed); return S_OK; };
	RunHandler(CoreComponent* pCoreComponent) : Process(CORETICKSPEED), mpCoreComponent(pCoreComponent) {};
};

#endif