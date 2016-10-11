#ifndef CORECOMPONENT_INC
#define CORECOMPONENT_INC

class CoreListener : public MemManaged, public EventListener {
private:
	CoreComponent* mpCoreComponent;
public:
	HRESULT handleEvent(ID eventID, MemManaged* pData)override;
	CoreListener(CoreComponent* pCoreComponent);
};

class CoreComponent : public MemManaged {
protected:
	Engine* mpEngine;
	String<char>* mpName;
	ProcessOwner* mpCoreProcessOwner;
	CoreListener* mpCoreListener;
public:
	CoreComponent(Engine* pEngine);
	Engine* getEngine() { return mpEngine; };

	virtual HRESULT restore();
	virtual HRESULT release() { delete mpName; delete mpCoreListener; delete mpCoreProcessOwner;  delete this; };
};

#endif