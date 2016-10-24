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
	Controller* pController = Controller::get();
	EventManager* pEventManager = EventManager::get();
	if (pController) {
		if (SUCCEEDED(pController->newControlSet(&mDefaultControls))) {
			pController->addMapping(mDefaultControls, VK_ESCAPE, EVENT_RELEASE);
			pController->addMapping(mDefaultControls, 'A', EVENT_MOVELEFT);
			pController->addMapping(mDefaultControls, 'D', EVENT_MOVERIGHT);
			pController->addMapping(mDefaultControls, 'S', EVENT_MOVEBACK);
			pController->addMapping(mDefaultControls, 'W', EVENT_MOVEFORWARD);
			if (pEventManager) {
				pEventManager->queueEvent(EVENT_SETCONTROLSET, new SimplyManaged<ID>(mDefaultControls));
			}
		}
	}
	if (pEventManager) {
		pEventManager->queueEvent(EVENT_CREATEWINDOW, new WindowCreator(800, 600, L"Counter Engine"));
	}
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
