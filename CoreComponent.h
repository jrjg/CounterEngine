#ifndef CORECOMPONENT_INC
#define CORECOMPONENT_INC

#include "MemManaged.h"
#include "EventListener.h"
#include "Process.h"
#include "TopInclude.h"

class RestoreListener;
class ReleaseListener;
class RunHandler;

class CoreComponent : public MemManaged {
	friend class MemoryManager;
protected:
	RestoreListener* mpRestoreListener;
	ReleaseListener* mpReleaseListener;
	RunHandler* mpRunHandler;
	CoreComponent(bool restCore) {if (restCore) { restoreCore(); }; };
	CoreComponent() { restoreCore(); };
	virtual ~CoreComponent();
	HRESULT restoreCoreListeners();
	HRESULT restoreCoreProcesses();
	HRESULT restoreCore() { restoreCoreListeners(); restoreCoreProcesses(); return S_OK; };
public:
	virtual HRESULT run(TIME elapsed) = 0;
	virtual HRESULT restore() = 0;
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
	HRESULT handle(MemManaged* pData) override { SAFE_RELEASE(mpCoreComponent); return S_OK; };
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