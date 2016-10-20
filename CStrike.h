#ifndef CStrike
#define Cstrike

#include "CoreComponent.h"
#include "EventListener.h"

class WindowCreatedListener : public EventListener {
private:
	ID* mpID;
	LPCWSTR mName;
public:
	HRESULT handle(MemManaged* pData);
	virtual ~WindowCreatedListener() {};
	WindowCreatedListener(ID* pID, LPCWSTR name) :EventListener(EVENT_WINDOWCREATED), mName(name), mpID(pID) {};
};

class CStrike : public CoreComponent,public Singleton<CStrike>{
	friend class Singleton<CStrike>;
	friend class GameInitListener;
private:
	static CStrike* mpCStrike;
	ID mDefaultControls;
	ID mDefaultWindow;
	WindowCreatedListener* mpWindowCreatedListener;
	GameInitListener* mpGameInitListener;
	HRESULT gameInit();
protected:
	CStrike() { restore(); };
	~CStrike() { SAFE_RELEASE(mpWindowCreatedListener); };
public:
	HRESULT restore();
	HRESULT run(TIME elapsed) { return S_OK; };
};

class GameInitListener : public EventListener {
public:
	HRESULT handle(MemManaged* pData) { return CStrike::get()->gameInit(); };
	virtual ~GameInitListener() {};
	GameInitListener() :EventListener(EVENT_GAMEINIT) {};
};

#endif
