#include "TopInclude.h"
#include "directx.h"
#include "Engine.h"
#include "Vector.h"
#include "Camera.h"
#include "EventManager.h"
#include "ProcessManager.h"

#include "Skybox.h"

HRESULT Skybox_New(Skybox** ppSkybox)
{
	_NEW(Skybox,*ppSkybox);
	Skybox* pSkybox = *ppSkybox;
	pSkybox->alive = true;
	EventManager_RegisterForEvent(EVENT_START, &Skybox_Startup);
	EventManager_RegisterForEvent(EVENT_END, &Skybox_Delete);
	return S_OK;
}

//HRESULT Skybox_CreateConstBuffer(Skybox* pSkybox) {
//	D3D11_BUFFER_DESC cbbd;
//	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
//	cbbd.Usage = D3D11_USAGE_DEFAULT;
//	cbbd.ByteWidth = sizeof(cbPerObject);
//	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	cbbd.CPUAccessFlags = 0;
//	cbbd.MiscFlags = 0;
//	CE1_CALL(Engine_GetCD3D11()->pDevice->CreateBuffer(&cbbd, NULL, &pSkybox->cbPerObjectBuffer));
//}

HRESULT Skybox_Render(Skybox* pSkybox) {

	Engine_GetCD3D11()->pImmediateContext->IASetIndexBuffer(pSkybox->sphereIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	Engine_GetCD3D11()->pImmediateContext->IASetVertexBuffers(0, 1, &pSkybox->sphereVertBuffer, &pSkybox->stride, &pSkybox->offset);
	
	XMMATRIX WVP = pSkybox->sphereWorld * (*Camera_GetViewTimesProjection(Engine_GetCamera()));
	cbPerObject cbPerObj;
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	cbPerObj.World = XMMatrixTranspose(pSkybox->sphereWorld);
	Engine_GetCD3D11()->pImmediateContext->UpdateSubresource(Engine_GetCD3D11()->cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	Engine_GetCD3D11()->pImmediateContext->VSSetConstantBuffers(0, 1, &Engine_GetCD3D11()->cbPerObjectBuffer);
	Engine_GetCD3D11()->pImmediateContext->PSSetShaderResources(0, 1, &pSkybox->smrv);
	//Engine_GetCD3D11()->pImmediateContext->PSSetSamplers(0, 1, &CubesTexSamplerState);

	Engine_GetCD3D11()->pImmediateContext->VSSetShader(pSkybox->SKYMAP_VS, 0, 0);
	Engine_GetCD3D11()->pImmediateContext->PSSetShader(pSkybox->SKYMAP_PS, 0, 0);
	Engine_GetCD3D11()->pImmediateContext->OMSetDepthStencilState(Engine_GetCD3D11()->DSLessEqual, 0);
	Engine_GetCD3D11()->pImmediateContext->RSSetState(Engine_GetCD3D11()->RSCullNone);
	Engine_GetCD3D11()->pImmediateContext->DrawIndexed(pSkybox->NumSphereFaces * 3, 0, 0);

	Engine_GetCD3D11()->pImmediateContext->OMSetDepthStencilState(NULL, 0);
	return S_OK;
}

HRESULT Skybox_Delete(void* p0)
{
	Skybox* pSkybox = Engine_GetSkybox();
	if (!pSkybox) { return S_OK; }
	pSkybox->alive = false;
	CE1_DEL(pSkybox->pVertices);
	CE1_DEL(pSkybox->pIndices);
	SAFE_RELEASE(pSkybox->sphereIndexBuffer);
	SAFE_RELEASE(pSkybox->sphereVertBuffer);
	SAFE_RELEASE(pSkybox->SKYMAP_VS);
	SAFE_RELEASE(pSkybox->SKYMAP_PS);
	SAFE_RELEASE(pSkybox->SKYMAP_VS_Buffer);
	SAFE_RELEASE(pSkybox->SKYMAP_PS_Buffer);
	SAFE_RELEASE(pSkybox->smrv);
	CE1_DEL(pSkybox);
	return S_OK;
}

HRESULT Skybox_CreateShaders(Skybox* pSkybox)
{
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	ID3DBlob* pErrorBlob;
	CE1_CALL(D3DCompileFromFile(L"Resource/Shader/Skymap.hlsl", NULL, NULL, "SKYMAP_VS", "vs_5_0", NULL, 0, &pSkybox->SKYMAP_VS_Buffer, &pErrorBlob));
	CE1_CALL(Engine_GetCD3D11()->pDevice->CreateVertexShader(pSkybox->SKYMAP_VS_Buffer->GetBufferPointer(), pSkybox->SKYMAP_VS_Buffer->GetBufferSize(), NULL, &pSkybox->SKYMAP_VS));
	SAFE_RELEASE(pErrorBlob);
	CE1_CALL(D3DCompileFromFile(L"Resource/Shader/Skymap.hlsl", NULL, NULL, "SKYMAP_PS", "ps_5_0", NULL, 0, &pSkybox->SKYMAP_PS_Buffer, &pErrorBlob));
	CE1_CALL(Engine_GetCD3D11()->pDevice->CreatePixelShader(pSkybox->SKYMAP_PS_Buffer->GetBufferPointer(), pSkybox->SKYMAP_PS_Buffer->GetBufferSize(), NULL, &pSkybox->SKYMAP_PS));
	SAFE_RELEASE(pErrorBlob);
	return S_OK;
}

HRESULT Skybox_Run(TIME elapsed)
{
	Skybox* pSkybox = Engine_GetSkybox();
	pSkybox->sphereWorld = XMMatrixIdentity();
	pSkybox->sphereWorld = XMMatrixScaling(5.0f, 5.0f, 5.0f) * XMMatrixTranslation(XMVectorGetX(Engine_GetCamera()->Position), XMVectorGetY(Engine_GetCamera()->Position), XMVectorGetZ(Engine_GetCamera()->Position));
	return S_OK;
}

HRESULT Skybox_CreateTexture(Skybox* pSkybox) {
	CE1_CALL(CreateDDSTextureFromFile(
		Engine_GetCD3D11()->pDevice, 
		L"Resource/Texture/skymap.dds",
		NULL,
		&pSkybox->smrv
	));
	return S_OK;
}

HRESULT Skybox_CreateSphere(Skybox* pSkybox, int LatLines, int LongLines)
{
	pSkybox->stride = sizeof(Float3);
	pSkybox->offset = 0;
	pSkybox->NumSphereVertices = ((LatLines - 2) * LongLines) + 2;
	pSkybox->NumSphereFaces = ((LatLines - 3)*(LongLines) * 2) + (LongLines * 2);
	float sphereYaw = 0.0f;
	float spherePitch = 0.0f;
	pSkybox->pVertices = (Float3*)malloc(pSkybox->NumSphereVertices * sizeof(Float3));
	XMVECTOR currVertPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	pSkybox->pVertices[0].x = 0.0f;
	pSkybox->pVertices[0].y = 0.0f;
	pSkybox->pVertices[0].z = 1.0f;
	for (DWORD i = 0; i < LatLines - 2; ++i) {
		spherePitch = (i + 1) * (3.14 / (LatLines - 1));
		pSkybox->Rotationx = XMMatrixRotationX(spherePitch);
		for (DWORD j = 0; j < LongLines; ++j) {
			sphereYaw = j * (6.28 / (LongLines));
			pSkybox->Rotationy = XMMatrixRotationZ(sphereYaw);
			currVertPos = XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (pSkybox->Rotationx * pSkybox->Rotationy));
			currVertPos = XMVector3Normalize(currVertPos);
			pSkybox->pVertices[i*LongLines + j + 1].x = XMVectorGetX(currVertPos);
			pSkybox->pVertices[i*LongLines + j + 1].y = XMVectorGetY(currVertPos);
			pSkybox->pVertices[i*LongLines + j + 1].z = XMVectorGetZ(currVertPos);
		}
	}
	pSkybox->pVertices[pSkybox->NumSphereVertices - 1].x = 0.0f;
	pSkybox->pVertices[pSkybox->NumSphereVertices - 1].y = 0.0f;
	pSkybox->pVertices[pSkybox->NumSphereVertices - 1].z = -1.0f;
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Float3) * pSkybox->NumSphereVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = &pSkybox->pVertices[0];
	CE1_CALL(Engine_GetCD3D11()->pDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &pSkybox->sphereVertBuffer));
	pSkybox->pIndices = (DWORD*)malloc(pSkybox->NumSphereFaces * 3 * sizeof(DWORD));
	int k = 0;
	for (DWORD l = 0; l < LongLines - 1; ++l) {
		pSkybox->pIndices[k] = 0;
		pSkybox->pIndices[k + 1] = l + 1;
		pSkybox->pIndices[k + 2] = l + 2;
		k += 3;
	}
	pSkybox->pIndices[k] = 0;
	pSkybox->pIndices[k + 1] = LongLines;
	pSkybox->pIndices[k + 2] = 1;
	k += 3;
	for (DWORD i = 0; i < LatLines - 3; ++i) {
		for (DWORD j = 0; j < LongLines - 1; ++j) {
			pSkybox->pIndices[k] = i*LongLines + j + 1;
			pSkybox->pIndices[k + 1] = i*LongLines + j + 2;
			pSkybox->pIndices[k + 2] = (i + 1)*LongLines + j + 1;
			pSkybox->pIndices[k + 3] = (i + 1)*LongLines + j + 1;
			pSkybox->pIndices[k + 4] = i*LongLines + j + 2;
			pSkybox->pIndices[k + 5] = (i + 1)*LongLines + j + 2;
			k += 6; // next quad
		}
		pSkybox->pIndices[k] = (i*LongLines) + LongLines;
		pSkybox->pIndices[k + 1] = (i*LongLines) + 1;
		pSkybox->pIndices[k + 2] = ((i + 1)*LongLines) + LongLines;
		pSkybox->pIndices[k + 3] = ((i + 1)*LongLines) + LongLines;
		pSkybox->pIndices[k + 4] = (i*LongLines) + 1;
		pSkybox->pIndices[k + 5] = ((i + 1)*LongLines) + 1;
		k += 6;
	}
	for (DWORD l = 0; l < LongLines - 1; ++l) {
		pSkybox->pIndices[k] = pSkybox->NumSphereVertices - 1;
		pSkybox->pIndices[k + 1] = (pSkybox->NumSphereVertices - 1) - (l + 1);
		pSkybox->pIndices[k + 2] = (pSkybox->NumSphereVertices - 1) - (l + 2);
		k += 3;
	}
	pSkybox->pIndices[k] = pSkybox->NumSphereVertices - 1;
	pSkybox->pIndices[k + 1] = (pSkybox->NumSphereVertices - 1) - LongLines;
	pSkybox->pIndices[k + 2] = pSkybox->NumSphereVertices - 2;
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * pSkybox->NumSphereFaces * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &pSkybox->pIndices[0];
	CE1_CALL(Engine_GetCD3D11()->pDevice->CreateBuffer(&indexBufferDesc, &iinitData, &pSkybox->sphereIndexBuffer));
	return S_OK;
}

HRESULT Skybox_Startup(void* p0) {
	Skybox* pSkybox = Engine_GetSkybox();
	CE1_ASSERT(pSkybox);
	CE1_CALL(Skybox_CreateSphere(pSkybox, 10, 10));
	CE1_CALL(Skybox_CreateShaders(pSkybox));
	CE1_CALL(Skybox_CreateTexture(pSkybox));
	CE1_CALL(Skybox_CreateRasterizerAndDepthStencilState(pSkybox));
	ProcessManager_NewProcess(&Skybox_Run, (1000.0f) / (100.0f));
	return S_OK;
}