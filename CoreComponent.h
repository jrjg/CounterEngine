#ifndef CORECOMPONENT_INC
#define CORECOMPONENT_INC

class CoreComponent : public MemManaged, public EventListener, public ProcessOwner{
protected:
	Engine* mpEngine;
	String<char>* mpName;
	ID processHandlerID;
public:
	HRESULT handleEvent(ID eventID, void* pData) override;
	CoreComponent(Engine* pEngine);

	virtual HRESULT restore() = 0;
	virtual HRESULT release() { delete this; };
};

#endif