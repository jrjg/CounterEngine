//#include "TopInclude.h"
//#include "Engine.h"
//#include "ProcessManager.h"
//#include "EventManager.h"
//#include "windowmgr.h"
//#include "Memory.h"
//
//
//#include "Camera.h"
//
//
//
//HRESULT Camera_Reset(Camera * pCamera)
//{
//	pCamera->pitch = 0.0f;
//	pCamera->yaw = 0.0f;
//	pCamera->moveBackForward = 0.0f;
//	pCamera->moveLeftRight = 0.0f;
//	pCamera->Forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
//	pCamera->Right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
//	pCamera->DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
//	pCamera->DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
//	pCamera->Target = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
//	pCamera->Position = XMVectorSet(0.0f, 1.3f, -4.0f, 0.0f);
//	pCamera->Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
//	pCamera->Projection = XMMatrixPerspectiveFovLH(0.4f*3.14f, Engine_BUFFERWIDTH() / Engine_BUFFERHEIGHT(), 1.0f, 1000.0f);
//	pCamera->CursorLastPos.x = -1.0f;
//	pCamera->CursorLastPos.y = -1.0f;
//	pCamera->mr = false;
//	pCamera->ml = false;
//	pCamera->mf = false;
//	pCamera->mb = false;
//	CE1_CALL(Camera_Run(NULL));
//	return S_OK;
//}
//
//XMMATRIX* Camera_GetCamView(Camera* pCamera) {
//	return &pCamera->View;
//}
//
//XMMATRIX* Camera_GetCamProjection(Camera* pCamera) {
//	return &pCamera->Projection;
//}
//
//XMMATRIX* Camera_GetViewTimesProjection(Camera* pCamera) {
//	return &pCamera->ViewTimesProjection;
//}
//
//HRESULT Camera_CalcViewTimesProjection(Camera* pCamera) {
//	pCamera->ViewTimesProjection = pCamera->View * pCamera->Projection;
//	return S_OK;
//}
//
//HRESULT Camera_CalcView(Camera* pCamera,TIME elapsed) {
//	pCamera->RotationMatrix = XMMatrixRotationRollPitchYaw(pCamera->pitch, pCamera->yaw, 0);
//	pCamera->Target = XMVector3TransformCoord(pCamera->DefaultForward, pCamera->RotationMatrix);
//	pCamera->Target = XMVector3Normalize(pCamera->Target);
//
//	XMMATRIX RotateYTempMatrix;
//	RotateYTempMatrix = XMMatrixRotationY(pCamera->yaw);
//
//	pCamera->Right = XMVector3TransformCoord(pCamera->DefaultRight, RotateYTempMatrix);
//	pCamera->Up = XMVector3TransformCoord(pCamera->Up, RotateYTempMatrix);
//	pCamera->Forward = XMVector3TransformCoord(pCamera->DefaultForward, RotateYTempMatrix);
//
//	pCamera->Position += pCamera->moveLeftRight * (pCamera->Right);
//	pCamera->Position += pCamera->moveBackForward * (pCamera->Forward);
//	
//	pCamera->Target = pCamera->Position + pCamera->Target;
//	pCamera->View = XMMatrixLookAtLH(pCamera->Position, pCamera->Target, pCamera->Up);
//	return S_OK;
//}
//
//
//HRESULT Camera_Run(TIME elapsed)
//{
//	Camera* pCamera = Engine_GetCamera();
//	POINT p;
//
//	GetCursorPos(&p);
//	pCamera->yaw += (p.x - pCamera->CursorLastPos.x) * 0.001f;
//	pCamera->pitch += (p.y - pCamera->CursorLastPos.y) * 0.001f;
//	pCamera->CursorLastPos.x = Engine_BUFFERWIDTH() / 2;
//	pCamera->CursorLastPos.y = Engine_BUFFERHEIGHT() / 2;
//	SetCursorPos(pCamera->CursorLastPos.x, pCamera->CursorLastPos.y);
//
//	CE1_CALL(Camera_CalcView(pCamera, elapsed));
//	CE1_CALL(Camera_CalcViewTimesProjection(pCamera));
//	return S_OK;
//}
//
//HRESULT Camera_MoveLeft(void* pDown) {
//	Camera* pCamera = Engine_GetCamera();
//	if (*(bool*)pDown) {
//		pCamera->moveLeftRight = -0.02f;
//		pCamera->ml = true;
//	}
//	else {
//		pCamera->ml = false;
//		if (!pCamera->mr) {
//			pCamera->moveLeftRight = 0.0f;
//		}
//		else {
//			pCamera->moveLeftRight = 0.02f;
//		}
//	}
//	return S_OK;
//}
//
//HRESULT Camera_MoveRight(void* pDown) {
//	Camera* pCamera = Engine_GetCamera();
//	if (*(bool*)pDown) {
//		pCamera->moveLeftRight = 0.02f;
//		pCamera->mr = true;
//	}
//	else {
//		pCamera->mr = false;
//		if (!pCamera->ml) {
//			pCamera->moveLeftRight = 0.0f;
//		}
//		else {
//			pCamera->moveLeftRight = -0.02f;
//		}
//	}
//	return S_OK;
//}
//
//HRESULT Camera_MoveBack(void* pDown) {
//	Camera* pCamera = Engine_GetCamera();
//	if (*(bool*)pDown) {
//		pCamera->moveBackForward = -0.02f;
//		pCamera->mb = true;
//	}
//	else {
//		pCamera->mb = false;
//		if (!pCamera->mf) {
//			pCamera->moveBackForward = 0.0f;
//		}
//		else {
//			pCamera->moveBackForward = 0.02f;
//		}
//	}
//	return S_OK;
//}
//
//HRESULT Camera_MoveForward(void* pDown) {
//	Camera* pCamera = Engine_GetCamera();
//	if (*(bool*)pDown) {
//		pCamera->moveBackForward = 0.02f;
//		pCamera->mf = true;
//	}
//	else {
//		pCamera->mf = false;
//		if (!pCamera->mb) {
//			pCamera->moveBackForward = 0.0f;
//		}
//		else {
//			pCamera->moveBackForward = -0.02f;
//		}
//	}
//	return S_OK;
//}
//
//HRESULT Camera_New(Camera** ppCamera)
//{
//	_NEW(Camera, *ppCamera);
//	EventManager_RegisterForEvent(EVENT_START, &Camera_Startup);
//	EventManager_RegisterForEvent(EVENT_END, &Camera_ShutdownAndDelete);
//	return S_OK;
//}
//
//HRESULT Camera_Startup(void* p0) {
//	Camera* pCamera = Engine_GetCamera();
//	ProcessManager_NewProcess(&Camera_Run, (1000.0f) / (100.0f));
//
//	EventManager_RegisterForEvent(EVENT_MOVELEFT, &Camera_MoveLeft);
//	EventManager_RegisterForEvent(EVENT_MOVERIGHT, &Camera_MoveRight);
//	EventManager_RegisterForEvent(EVENT_MOVEBACK, &Camera_MoveBack);
//	EventManager_RegisterForEvent(EVENT_MOVEFORWARD, &Camera_MoveForward);
//	CE1_CALL(Camera_Reset(pCamera));
//	return S_OK;
//}
//
//HRESULT Camera_ShutdownAndDelete(void* p0)
//{
//	Camera* pCamera = Engine_GetCamera();
//	if (!pCamera) { return S_OK; }
//	CE1_DEL(pCamera);
//	return S_OK;
//}