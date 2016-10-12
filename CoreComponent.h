#ifndef CORECOMPONENT_INC
#define CORECOMPONENT_INC

class RestoreListener : public EventListener {
private:
	CoreComponent* mpCoreComponent;
public:
	HRESULT handleEvent(MemManaged* pData) override { mpCoreComponent->restore(); return S_OK; };
	RestoreListener(CoreComponent* pCoreComponent) : EventListener(EVENT_RESTORE), mpCoreComponent(pCoreComponent) {};
};

class ReleaseListener : public EventListener {
private:
	CoreComponent* mpCoreComponent;
public:
	HRESULT handleEvent(MemManaged* pData) override { mpCoreComponent->release(); return S_OK; };
	ReleaseListener(CoreComponent* pCoreComponent) : EventListener(EVENT_RELEASE), mpCoreComponent(pCoreComponent) {};
};

class RunHandler : public ProcessHandler {
private:
	CoreComponent* mpCoreComponent;
public:
	HRESULT handleProcess(TIME elapsed) { mpCoreComponent->run(elapsed); };
	RunHandler(CoreComponent* pCoreComponent) : ProcessHandler(CORETICKSPEED), mpCoreComponent(pCoreComponent) {};
};

class CoreComponent : public MemManaged {
private:
	static void* mpInstance;
	RestoreListener* mpRestoreListener;
	ReleaseListener* mpReleaseListener;
	RunHandler* mpRunHandler;
	CoreComponent() { restore(); };
	virtual ~CoreComponent(){ delete mpName; delete mpRestoreListener; delete mpReleaseListener; }
protected:
	String<char>* mpName;
public:
	virtual void* get() = 0;
	virtual HRESULT run(TIME elapsed) = 0;
	virtual HRESULT restore();
	virtual HRESULT release() { delete this; };
};

#endif