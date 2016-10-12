#ifndef CORECOMPONENT_INC
#define CORECOMPONENT_INC

class RestoreListener : public EventListener {
private:
	CoreComponent* mpCoreComponent;
public:
	HRESULT handle(MemManaged* pData) override { mpCoreComponent->restore(); return S_OK; };
	RestoreListener(CoreComponent* pCoreComponent) : EventListener(EVENT_RESTORE), mpCoreComponent(pCoreComponent) {};
};

class ReleaseListener : public EventListener {
private:
	CoreComponent* mpCoreComponent;
public:
	HRESULT handle(MemManaged* pData) override { mpCoreComponent->release(); return S_OK; };
	ReleaseListener(CoreComponent* pCoreComponent) : EventListener(EVENT_RELEASE), mpCoreComponent(pCoreComponent) {};
};

class RunHandler : public Process {
private:
	CoreComponent* mpCoreComponent;
public:
	HRESULT handle(TIME elapsed)override { mpCoreComponent->run(elapsed); };
	RunHandler(CoreComponent* pCoreComponent) : Process(CORETICKSPEED), mpCoreComponent(pCoreComponent) {};
};

class CoreComponent : public MemManaged {
protected:
	RestoreListener* mpRestoreListener;
	ReleaseListener* mpReleaseListener;
	RunHandler* mpRunHandler;
	String<char>* mpName;
	CoreComponent(bool autoRegister) { if (autoRegister) { restore(); } };
	CoreComponent() { restore(); };
	virtual ~CoreComponent() { delete mpName; delete mpRunHandler; delete mpRestoreListener; delete mpReleaseListener; }
public:
	virtual HRESULT run(TIME elapsed) = 0;
	virtual HRESULT restore() = 0;
	virtual HRESULT release() { delete this; };
};

#endif