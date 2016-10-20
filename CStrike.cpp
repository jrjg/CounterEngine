#include "TopInclude.h"
#include "Engine.h"
#include "Controller.h"
#include "EventManager.h"
#include "Memory.h"
#include "SimplyManaged.h"
#include "Singleton.h"
#include "WindowManager.h"

#include "CStrike.h"

HRESULT CStrike::gameInit()
{
	mDefaultControls = Controller::get()->newControlSet();
	Controller::get()->addMapping(mDefaultControls, VK_ESCAPE, EVENT_RELEASE);
	Controller::get()->addMapping(mDefaultControls, 'A', EVENT_MOVELEFT);
	Controller::get()->addMapping(mDefaultControls, 'D', EVENT_MOVERIGHT);
	Controller::get()->addMapping(mDefaultControls, 'S', EVENT_MOVEBACK);
	Controller::get()->addMapping(mDefaultControls, 'W', EVENT_MOVEFORWARD);
	EventManager::get()->queueEvent(EVENT_SETCONTROLSET, new SimplyManaged<ID>(mDefaultControls));

	EventManager::get()->queueEvent(EVENT_CREATEWINDOW, new WindowCreator(800, 600, L"Counter Engine"));
	SAFE_RELEASE(mpWindowCreatedListener); mpWindowCreatedListener = new WindowCreatedListener(&mDefaultWindow, L"Counter Engine");
	return S_OK;
}

HRESULT CStrike::restore()
{
	SAFE_RELEASE(mpGameInitListener); mpGameInitListener = new GameInitListener();
	return S_OK;
}

HRESULT WindowCreatedListener::handle(MemManaged * pData)
{
	WindowCreated* pWindowCreated = (WindowCreated*)pData;
	if (mName == pWindowCreated->mWNDTitle) {
		*mpID = pWindowCreated->mID;
	}
	return S_OK;
}
