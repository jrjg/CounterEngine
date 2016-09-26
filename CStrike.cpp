#include "TopInclude.h"
#include "Engine.h"
#include "Controller.h"
#include "EventManager.h"
#include "directx.h"
#include "CStrike.h"

CStrike* CStrike_NEW() {
	CStrike* pCStrike;
	_NEW(CStrike, pCStrike);
	srand(time(NULL));
	CStrike_CreateControls(pCStrike);
	Controller_SetControls(pCStrike->_ctrlsGame);

	//EventManager_RegisterForEvent(EVENT_KEYDOWN, &Controller_EvalKeyDown);
	//EventManager_RegisterForEvent(EVENT_KEYUP, &Controller_EvalKeyUp);
	return pCStrike;
}

HRESULT CStrike_Run(TIME elapsed) {
	cd3d11* pcd3d11 = Engine_GetCD3D11();
	CE1_ASSERT(pcd3d11&&elapsed);
	/*pcd3d11->rot += .0005f*elapsed;
	if (pcd3d11->rot > 6.26f)
		pcd3d11->rot = 0.0f;*/
	//pcd3d11->World = XMMatrixTranslation(0.0f,0.0f, 0.0f) * XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), pcd3d11->rot);
	return S_OK;
}

HRESULT CStrike_DELETE(void) {
	CStrike* pCStrike = Engine_GetCStrike();
	CE1_ASSERT(pCStrike);
	CE1_DEL(pCStrike);
	return S_OK;
}

HRESULT CStrike_CreateControls(CStrike* pCStrike)
{
	CE1_ASSERT(pCStrike);
	pCStrike->_ctrlsGame = Controller_NewControls();
	CE1_CALL(Controller_AddControl(pCStrike->_ctrlsGame, VK_ESCAPE, EVENT_END));
	CE1_CALL(Controller_AddControl(pCStrike->_ctrlsGame, 'A', EVENT_MOVELEFT));
	CE1_CALL(Controller_AddControl(pCStrike->_ctrlsGame, 'D', EVENT_MOVERIGHT));
	CE1_CALL(Controller_AddControl(pCStrike->_ctrlsGame, 'S', EVENT_MOVEBACK));
	CE1_CALL(Controller_AddControl(pCStrike->_ctrlsGame, 'W', EVENT_MOVEFORWARD));
	return S_OK;
}