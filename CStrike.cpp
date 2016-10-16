#include "TopInclude.h"
#include "Engine.h"
#include "Controller.h"
#include "EventManager.h"
#include "Memory.h"
#include "SimplyManaged.h"
#include "Singleton.h"

#include "CStrike.h"

HRESULT CStrike::restore()
{
	mDefaultControls = Controller::get()->newControlSet();
	Controller::get()->addMapping(mDefaultControls, VK_ESCAPE, EVENT_RELEASE);
	Controller::get()->addMapping(mDefaultControls, 'A', EVENT_MOVELEFT);
	Controller::get()->addMapping(mDefaultControls, 'D', EVENT_MOVERIGHT);
	Controller::get()->addMapping(mDefaultControls, 'S', EVENT_MOVEBACK);
	Controller::get()->addMapping(mDefaultControls, 'W', EVENT_MOVEFORWARD);
	EventManager::get()->queueEvent(EVENT_SETCONTROLSET, new SimplyManaged<ID>(mDefaultControls));
	return S_OK;
}
