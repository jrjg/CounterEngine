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

	EventManager_RegisterForEvent(EVENT_KEYDOWN, &Controller_EvalKey);

	return pCStrike;
}

BOOL CStrike_Run(TIME elapsed) {
	cd3d11* pcd3d11 = Engine_GetCD3D11();

	pcd3d11->rot += .0005f*elapsed;
	if (pcd3d11->rot > 6.26f)
		pcd3d11->rot = 0.0f;
	pcd3d11->World = XMMatrixTranslation(0.0f,0.0f, 0.0f) * XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), pcd3d11->rot);
	return ERROR_FAILURE;
}

BOOL CStrike_DELETE(void) {
	CStrike* pCStrike = Engine_GetCStrike();
	_DEL(pCStrike);
	return ERROR_FAILURE;
}

BOOL CStrike_CreateControls(CStrike* pCStrike)
{
	pCStrike->_ctrlsGame = Controller_NewControls();
	Controller_AddControl(pCStrike->_ctrlsGame, 27, EVENT_END);
	Controller_AddControl(pCStrike->_ctrlsGame, 97, EVENT_MOVELEFT);
	Controller_AddControl(pCStrike->_ctrlsGame, 100, EVENT_MOVERIGHT);
	Controller_AddControl(pCStrike->_ctrlsGame, 115, EVENT_MOVEDOWN);
	Controller_AddControl(pCStrike->_ctrlsGame, 119, EVENT_ROTATECW);
	Controller_AddControl(pCStrike->_ctrlsGame, 32, EVENT_MOVEFIX);
	return ERROR_FAILURE;
}