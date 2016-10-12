//#include "TopInclude.h"
//#include "Engine.h"
//#include "Controller.h"
//#include "EventManager.h"
//#include "directx.h"
//#include "Memory.h"
//
//#include "CStrike.h"
//
//CStrike* gpCStrike;
//
//CStrike::CStrike() {
//	mControlsDefault = Controller::get()->newControls(); 
//	EventManager::get()->registerForEvent(EVENT_RESTORE, &restore);  
//	EventManager::get()->registerForEvent(EVENT_RELEASE, &release);
//};
//
//CStrike* CStrike::get()
//{
//	if (!gpCStrike) {
//		gpCStrike = new CStrike();
//	}
//	return gpCStrike;
//}
//
//HRESULT CStrike::restore(void * p0)
//{
//	Controller::addMapping(get()->mControlsDefault, VK_ESCAPE, EVENT_RELEASE);
//	Controller::addMapping(get()->mControlsDefault, 'A', EVENT_MOVELEFT);
//	Controller::addMapping(get()->mControlsDefault, 'D', EVENT_MOVERIGHT);
//	Controller::addMapping(get()->mControlsDefault, 'S', EVENT_MOVEBACK);
//	Controller::addMapping(get()->mControlsDefault, 'W', EVENT_MOVEFORWARD);
//	Controller::setControls(get()->mControlsDefault);
//	return E_NOTIMPL;
//}
