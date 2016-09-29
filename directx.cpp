#include "TopInclude.h"
#include "Engine.h"
#include "windowmgr.h"
#include "Vector.h"
#include "List.h"
#include "Resource.h"
#include "ComponentManager.h"
#include "Parser.h"
#include "Camera.h"
#include "Skybox.h"
//#include "SceneManager.h"
//#include "LightManager.h"
#include "directx.h"

//HRESULT XCreateBlendstateTransparency(cd3d11* pD3D) {
//	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
//	ZeroMemory(&rtbd, sizeof(rtbd));
//	rtbd.BlendEnable = true;
//	rtbd.SrcBlend = D3D11_BLEND_INV_SRC_ALPHA;
//	rtbd.DestBlend = D3D11_BLEND_SRC_ALPHA;
//	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
//	rtbd.SrcBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
//	rtbd.DestBlendAlpha = D3D11_BLEND_SRC_ALPHA;
//	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
//	rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;
//
//	D3D11_BLEND_DESC blendDesc;
//	ZeroMemory(&blendDesc, sizeof(blendDesc));
//	blendDesc.AlphaToCoverageEnable = false;
//	blendDesc.RenderTarget[0] = rtbd;
//
//	Engine_GetCD3D11()->pDevice->CreateBlendState(&blendDesc, &Engine_GetCD3D11()->pBlendStateTransparency);
//	return S_OK;
//}


HRESULT XCreateRasterizerCullNone(cd3d11* pD3D) {
	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.FrontCounterClockwise = false;
	cmdesc.CullMode = D3D11_CULL_NONE;
	CE1_CALL(pD3D->pDevice->CreateRasterizerState(&cmdesc, &pD3D->RSCullNone));
	return S_OK;
}
HRESULT XCreateDepthStencilState(cd3d11* pD3D) {
	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	CE1_CALL(pD3D->pDevice->CreateDepthStencilState(&dssDesc, &pD3D->DSLessEqual));
	return S_OK;
}

HRESULT XCreateConstBuffer(cd3d11* pd3d11) {
	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;
	CE1_CALL(Engine_GetCD3D11()->pDevice->CreateBuffer(&cbbd, NULL, &pd3d11->cbPerObjectBuffer));
}

 HRESULT cd3d11_CreateConstantBuffer(cd3d11* pd3d11,UINT size, bool dynamic, bool CPUupdates, D3D11_SUBRESOURCE_DATA* pData, ID3D11Buffer** ppBuffer) {
	CE1_ASSERT(pd3d11&&size);
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = size;
	desc.MiscFlags = 0;
	//desc.StructureByteStride = 0;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	if (dynamic && CPUupdates) {
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}else if (dynamic && !CPUupdates) {
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;
	}else {
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.CPUAccessFlags = 0;
	}

	CE1_CALL(pd3d11->pDevice->CreateBuffer(&desc, pData, ppBuffer));

	return S_OK;
}

HRESULT cd3d11_CompileShader(LPCWSTR pFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut) {
	CE1_ASSERT(pFileName&&szEntryPoint&&szShaderModel);
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	ID3DBlob* pErrorBlob;
	CE1_CALL(D3DCompileFromFile(pFileName, NULL, NULL, szEntryPoint, szShaderModel, dwShaderFlags, 0, ppBlobOut, &pErrorBlob));
	SAFE_RELEASE(pErrorBlob);
	return S_OK;
}

 HRESULT cd3d11_createVertexShader(cd3d11* pd3d11, LPCSTR szEntryPoint, LPCWSTR name, LPCSTR szShaderModel, cd3d11_VertexShader** ppcd3d11VertexShader) {
	 CE1_ASSERT(pd3d11&&szEntryPoint&&name&&szShaderModel);
	_NEW(cd3d11_VertexShader, *ppcd3d11VertexShader);
	cd3d11_VertexShader* pcd3d11VertexShader = *ppcd3d11VertexShader;
	ID3DBlob* pVertexShaderBuffer;
	CE1_CALL(cd3d11_CompileShader(name, szEntryPoint, szShaderModel, &pVertexShaderBuffer));
	CE1_CALL(pd3d11->pDevice->CreateVertexShader(pVertexShaderBuffer->GetBufferPointer(), pVertexShaderBuffer->GetBufferSize(), NULL, &(pcd3d11VertexShader->pVertexShader)));
	CE1_CALL(pd3d11->pDevice->CreateInputLayout(D3D11VertexLayout_UnlitTextured, ARRAYSIZE(D3D11VertexLayout_UnlitTextured), pVertexShaderBuffer->GetBufferPointer(), pVertexShaderBuffer->GetBufferSize(), &(pcd3d11VertexShader->pVertexLayout11)));

	CE1_CALL(cd3d11_CreateConstantBuffer(pd3d11,sizeof(ConstantBuffer_Matrices), true, true, NULL,&pcd3d11VertexShader->pcbVSMatrices));
	CE1_CALL(cd3d11_CreateConstantBuffer(pd3d11, sizeof(ConstantBuffer_Lighting) , true, true, NULL,&pcd3d11VertexShader->pcbVSLighting));
	CE1_CALL(cd3d11_CreateConstantBuffer(pd3d11, sizeof(ConstantBuffer_Material), true, true, NULL,&pcd3d11VertexShader->pcbVSMaterial));

	SAFE_RELEASE(pVertexShaderBuffer);
	return S_OK;
}

 HRESULT cd3d11_createPixelShader(cd3d11* pd3d11, LPCSTR szEntryPoint, LPCWSTR name, LPCSTR szShaderModel, cd3d11_PixelShader** ppcd3d11PixelShader) {
	 CE1_ASSERT(pd3d11&&szEntryPoint&&name&&szShaderModel);
	_NEW(cd3d11_PixelShader, *ppcd3d11PixelShader);
	cd3d11_PixelShader* pcd3d11PixelShader = *ppcd3d11PixelShader;
	ID3DBlob* pPixelShaderBuffer;
	CE1_CALL(cd3d11_CompileShader(name, szEntryPoint, szShaderModel, &pPixelShaderBuffer));
	CE1_CALL(pd3d11->pDevice->CreatePixelShader(pPixelShaderBuffer->GetBufferPointer(), pPixelShaderBuffer->GetBufferSize(),NULL, &(pcd3d11PixelShader->pPixelShader)));
	CE1_CALL(cd3d11_CreateConstantBuffer(pd3d11, sizeof(ConstantBuffer_Material), true, true, NULL,&pcd3d11PixelShader->pcbPSMaterial));
	SAFE_RELEASE(pPixelShaderBuffer);
	return S_OK;
}

//HRESULT cd3d11_initCamera(cd3d11* pd3d11) {
//	CE1_ASSERT(pd3d11);
//	pd3d11->camView = XMMatrixLookAtLH(XMVectorSet(0.0f, 3.0f, -4.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
//	pd3d11->camProjection = XMMatrixPerspectiveFovLH(0.4f*3.14f, Engine_BUFFERWIDTH() / Engine_BUFFERHEIGHT(), 1.0f, 1000.0f);
//	return S_OK;
//}

//BOOL cd3d11_setupRenderVertexShader(Scene* pScene, SceneNode* pSceneNode) {
HRESULT cd3d11_setupRenderVertexShader() {
	cd3d11* pd3d11 = Engine_GetCD3D11();
	CE1_ASSERT(pd3d11);
	cd3d11_VertexShader* pcd3d11VertexShader = (cd3d11_VertexShader*)Vector_Get(pd3d11->pShaders, pd3d11->vsID);
	pd3d11->pImmediateContext->VSSetShader(pcd3d11VertexShader->pVertexShader , NULL, 0);
	pd3d11->pImmediateContext->IASetInputLayout(pcd3d11VertexShader->pVertexLayout11);
	
	//Matrices
	/*XMMATRIX* pWorldViewProjection = CameraManager_GetWorldViewProjection(SceneManager_GetCamera(pScene));
	XMMATRIX* pWorld = SceneManager_GetTopMatrix(pScene);*/
	D3D11_MAPPED_SUBRESOURCE MappedRessource;
	CE1_CALL(pd3d11->pImmediateContext->Map(pcd3d11VertexShader->pcbVSMatrices,0,D3D11_MAP_WRITE_DISCARD, 0, &MappedRessource));
	ConstantBuffer_Matrices* pVSMatrices = (ConstantBuffer_Matrices*)MappedRessource.pData;
	//(*pWorldViewProjection) = XMMatrixTranspose(pVSMatrices->WorldViewProj);
	//(*pWorld) = XMMatrixTranspose(pVSMatrices->World);
	XMStoreFloat4x4(&(pVSMatrices->WorldViewProj), XMMatrixTranspose(pd3d11->WorldViewProjection));
	XMStoreFloat4x4(&(pVSMatrices->World), XMMatrixTranspose(pd3d11->World));

	pd3d11->pImmediateContext->Unmap(pcd3d11VertexShader->pcbVSMatrices, 0);
	
	//Lighting
	CE1_CALL(pd3d11->pImmediateContext->Map(pcd3d11VertexShader->pcbVSLighting, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedRessource));
	ConstantBuffer_Lighting* pLighting = (ConstantBuffer_Lighting*)MappedRessource.pData;
	/*if (pcd3d11VertexShader->enable_lights) {
		CE1_CALL(LightMgr_CalcLighting(pLighting, pSceneNode));
	}else {*/
		pLighting->NumLights = 0;
		pLighting->LightAmbient = {1.0f, 1.0f, 1.0f, 1.0f};
	/*}*/
	pd3d11->pImmediateContext->Unmap(pcd3d11VertexShader->pcbVSLighting,0);

	//Material
	CE1_CALL(pd3d11->pImmediateContext->Map(pcd3d11VertexShader->pcbVSMaterial, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedRessource));
	ConstantBuffer_Material* pMaterial = (ConstantBuffer_Material*)MappedRessource.pData;
	//D3DCOLORVALUE color = SceneManager_NodeGetMaterial(pSceneNode)->Diffuse;
	//pMaterial->DiffuseObjectColor = DirectX::XMVectorSet(color.r, color.g, color.b, color.a);
	//provisorisch
	pMaterial->DiffuseObjectColor = { 0.5f, 0.5f, 0.5f, 1.0f };

	//D3DCOLORVALUE color;
	//if (pcd3d11VertexShader->enable_lights) {
	//	color = SceneManager_NodeGetMaterial(pSceneNode)->Ambient;
	//}else {
		//color.r = 1.0f;
		//color.g = color.r;
		//color.b = color.r;
		//color.a = color.r;
	//}
	//pMaterial->AmbientObjectColor = DirectX::XMVectorSet(color.r, color.g, color.b, color.a);
	//provisorisch
	pMaterial->AmbientObjectColor = { 1.0f, 1.0f, 1.0f, 1.0f };


	pMaterial->hasTexture = true;

	//nicht sicher ob unmap
	pd3d11->pImmediateContext->Unmap(pcd3d11VertexShader->pcbVSMaterial, 0);

	pd3d11->pImmediateContext->VSSetConstantBuffers(0, 1, &(pcd3d11VertexShader->pcbVSMatrices));
	pd3d11->pImmediateContext->VSSetConstantBuffers(1, 1, &(pcd3d11VertexShader->pcbVSLighting));
	pd3d11->pImmediateContext->VSSetConstantBuffers(2, 1, &(pcd3d11VertexShader->pcbVSMaterial));
	return S_OK;
}

//BOOL cd3d11_setupRenderPixelShader(Scene * pScene, SceneNode * pSceneNode)
HRESULT cd3d11_setupRenderPixelShader()
{
	cd3d11* pd3d11 = Engine_GetCD3D11();
	CE1_ASSERT(pd3d11);
	cd3d11_PixelShader* pcd3d11PixelShader = (cd3d11_PixelShader*)Vector_Get(pd3d11->pShaders, pd3d11->psID);
	pd3d11->pImmediateContext->PSSetShader(pcd3d11PixelShader->pPixelShader, NULL, 0);
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	CE1_CALL(pd3d11->pImmediateContext->Map(pcd3d11PixelShader->pcbPSMaterial, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource));
	ConstantBuffer_Material* pMaterial = (ConstantBuffer_Material*)MappedResource.pData;

	//D3DCOLORVALUE color = SceneManager_NodeGetMaterial(pSceneNode)->Diffuse;
	//pMaterial->DiffuseObjectColor = DirectX::XMVectorSet(color.r, color.g, color.b, color.a);
	//provisorisch farbe setzen
	pMaterial->DiffuseObjectColor = { 1.0f, 0.0f, 0.0f, 1.0f };

	//entscheiden ob texture oder nicht
	pMaterial->hasTexture = true;

	pd3d11->pImmediateContext->Unmap(pcd3d11PixelShader->pcbPSMaterial, 0);
	pd3d11->pImmediateContext->PSSetConstantBuffers(0, 1, &(pcd3d11PixelShader->pcbPSMaterial));

	//setzen der textur
	return S_OK;
}


HRESULT cd3d11_createShaders(cd3d11* pd3d11) {
	CE1_ASSERT(pd3d11);
	cd3d11_VertexShader* pcd3d11VertexShader;
	CE1_CALL(cd3d11_createVertexShader(pd3d11, "VSMain", L"Resource/Shader/VS.hlsl", "vs_5_0",&pcd3d11VertexShader));
	pd3d11->vsID = Vector_Pushback(pd3d11->pShaders, pcd3d11VertexShader);
	
	cd3d11_PixelShader* pcd3d11PixelShader;
	CE1_CALL(cd3d11_createPixelShader(pd3d11, "PSMain" ,L"Resource/Shader/PS.hlsl", "ps_5_0", &pcd3d11PixelShader));
	pd3d11->psID = Vector_Pushback(pd3d11->pShaders, pcd3d11PixelShader);
	return S_OK;
}


HRESULT cd3d11_NEW(cd3d11** ppd3d11) {
	_NEW(cd3d11, *ppd3d11);
	cd3d11* pd3d11 = *ppd3d11;
	CE1_CALL(cd3d11_getSwapChain(pd3d11));
	//pd3d11->pClsColor = (FLOAT*)malloc(sizeof(FLOAT) * 4);
	pd3d11->pClsColor[0] = 0.8f; pd3d11->pClsColor[1] = 0.8f; pd3d11->pClsColor[2] = 0.8f; pd3d11->pClsColor[3] = 1.0f;
	pd3d11->pShaders = Vector_New(sizeof(void*), 15);

	pd3d11->World = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	ShowCursor(false);

	CE1_CALL(cd3d11_createShaders(pd3d11));

	CE1_CALL(cd3d11_CreateDemoObject(pd3d11));

	CE1_CALL(cd3d11_CreateAndSetViewport(pd3d11));

	CE1_CALL(cd3d11_registerHandlers(pd3d11));

	CE1_CALL(XCreateConstBuffer(pd3d11));

	CE1_CALL(XCreateRasterizerCullNone(pd3d11));

	CE1_CALL(XCreateDepthStencilState(pd3d11));

	//CE1_CALL(XCreateBlendstateTranparency(pd3d11));

	return S_OK;
}

HRESULT cd3d11_CreateAndSetViewport(cd3d11* pd3d11) {
	CE1_ASSERT(pd3d11);
	pd3d11->pViewport;
	_NEW(D3D11_VIEWPORT, pd3d11->pViewport);
	pd3d11->pViewport->TopLeftX = 0.0f;
	pd3d11->pViewport->TopLeftY = 0.0f;
	pd3d11->pViewport->Width = Engine_BUFFERWIDTH();
	pd3d11->pViewport->Height = Engine_BUFFERHEIGHT();
	pd3d11->pViewport->MinDepth = 0.0f;
	pd3d11->pViewport->MaxDepth = 1.0f;
	pd3d11->pImmediateContext->RSSetViewports(1, pd3d11->pViewport);
	return S_OK;
}

HRESULT cd3d11_registerHandlers(cd3d11 * pd3d11)
{
	CE1_ASSERT(pd3d11);

	char* name;
	size_t len = 0;


	//resources
	len = sizeof(char) * 4;

	name = (char*)malloc(len); name = "jpg";
	CE1_CALL(ResourceManager_RegisterFormatHandler(name, &Resource_CreateTextureFromJPG, &Resource_DestroyTextureFromJPG));
	

	//components
	len = sizeof(char) * 20;

	name = (char*)malloc(len); name = "Render";
	ComponentManager_RegisterComponentHandler(name,&Component_CreateRenderComponent,&Component_DestroyRenderComponent);


	return S_OK;
}

HRESULT cd3d11_Run(TIME elapsed) {
	cd3d11* pd3d11 = Engine_GetCD3D11();
	CE1_ASSERT(pd3d11&&elapsed);

	pd3d11->pImmediateContext->OMSetRenderTargets(1, &(pd3d11->pView), pd3d11->pDepthView);

	XMMATRIX CamViewProjection = *Camera_GetViewTimesProjection(Engine_GetCamera());
	pd3d11->WorldViewProjection = pd3d11->World * CamViewProjection;
	//pd3d11->WorldViewProjection = pd3d11->World * pd3d11->camView * pd3d11->camProjection;
	//cbPerObj.WVP = XMMatrixTranspose(pd3d11->WorldViewProjection);

	CE1_CALL(cd3d11_clearTargets());
	CE1_CALL(cd3d11_setupRenderVertexShader());
	CE1_CALL(cd3d11_setupRenderPixelShader());

	UINT offset = 0;
	pd3d11->pImmediateContext->IASetVertexBuffers(0, 1, &(pd3d11->pDemoObject->pVertexBuffer), &(pd3d11->pDemoObject->stride), &offset);
	pd3d11->pImmediateContext->IASetIndexBuffer(pd3d11->pDemoObject->pIndexBuffer, DXGI_FORMAT_R16_UINT, offset);
	pd3d11->pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	CE1_CALL(cd3d11_SetTexture(&(pd3d11->pDemoObject->pResView), &(pd3d11->pDemoObject->pSamplerState)));

	pd3d11->pImmediateContext->RSSetState(pd3d11->pRasterizerState);

	pd3d11->pImmediateContext->DrawIndexed(pd3d11->pDemoObject->numPolys*3,0,0);

	Skybox* pSkybox = Engine_GetSkybox();
	if (pSkybox->alive) { CE1_CALL(Skybox_Render(pSkybox)); }

	CE1_CALL(pd3d11->pSwapChain->Present(0, 0));
	
	
	return S_OK;
}

HRESULT cd3d11_SetTexture(ID3D11ShaderResourceView* const *ppDiffuseRV, ID3D11SamplerState * const *ppSamplers)
{
	cd3d11* pd3d11 = Engine_GetCD3D11();
	CE1_ASSERT(pd3d11);
	pd3d11->pImmediateContext->PSSetShaderResources(0, 1, ppDiffuseRV);
	pd3d11->pImmediateContext->PSSetSamplers(0, 1, ppSamplers);
	return S_OK;
}

HRESULT cd3d11_CreateDemoObject(cd3d11* pd3d11) {
	CE1_ASSERT(pd3d11);
	pd3d11->pDemoObject;
	_NEW(VertexObject, pd3d11->pDemoObject);
	int squares = 10;
	int m_numVerts = (squares + 1)*(squares + 1); 
	
	//vertexbuffer
	D3D11Vertex_UnlitTextured* pVerts = (D3D11Vertex_UnlitTextured*)malloc(m_numVerts*sizeof(D3D11Vertex_UnlitTextured));
	for (int j = 0; j<(squares + 1); j++)
	{
		for (int i = 0; i<(squares + 1); i++)
		{
			int index = i + (j * (squares + 1));
			D3D11Vertex_UnlitTextured *vert = &pVerts[index];
			float x = (float)i - (squares / 2.0f);
			float y = (float)j - (squares / 2.0f);
			vert->Pos = XMFLOAT3(x, 0.f, y);
			vert->Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			vert->Uv.x = x;
			vert->Uv.y = y;
		}
	}

	//indexbuffer
	UINT numPolys = squares * squares * 2;
	WORD *pIndices = (WORD*)malloc(sizeof(WORD)*numPolys * 3);

	WORD *current = pIndices;
	for (int j = 0; j<squares; j++)
	{
		for (int i = 0; i<squares; i++)
		{
			*(current) = WORD(i + (j*(squares + 1)));
			*(current + 1) = WORD(i + ((j + 1)*(squares + 1)));
			*(current + 2) = WORD((i + 1) + (j*(squares + 1)));
			*(current + 3) = WORD((i + 1) + (j*(squares + 1)));
			*(current + 4) = WORD(i + ((j + 1)*(squares + 1)));
			*(current + 5) = WORD((i + 1) + ((j + 1)*(squares + 1)));
			current += 6;
		}
	}

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(D3D11Vertex_UnlitTextured) * m_numVerts;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = pVerts;
	CE1_CALL(pd3d11->pDevice->CreateBuffer(&bd, &InitData, &(pd3d11->pDemoObject->pVertexBuffer)));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * numPolys * 3;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = pIndices;
	CE1_CALL(pd3d11->pDevice->CreateBuffer(&bd, &InitData, &(pd3d11->pDemoObject->pIndexBuffer)));

	CE1_DEL(pVerts);
	CE1_DEL(pIndices);

	//numPolys
	pd3d11->pDemoObject->numPolys = numPolys;

	//stride
	pd3d11->pDemoObject->stride = sizeof(D3D11Vertex_UnlitTextured);

	//texture
	CE1_CALL(CreateWICTextureFromFile(pd3d11->pDevice, L"Resource/Texture/metalpanel.jpg", &(pd3d11->pDemoObject->pTex), &(pd3d11->pDemoObject->pResView)));
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));

	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	CE1_CALL(pd3d11->pDevice->CreateSamplerState(&sampDesc, &(pd3d11->pDemoObject->pSamplerState)));

	return S_OK;
}

HRESULT Resource_CreateTextureFromJPG(char* name, void** ppData) {
	cd3d11* pd3d11 = Engine_GetCD3D11();
	CE1_ASSERT(pd3d11&&name);
	TextureResource* pTexRes = (TextureResource*)(*ppData);
	_NEW(TextureResource, pTexRes);
	CE1_CALL(CreateWICTextureFromFile(pd3d11->pDevice, LPCWSTR(name), &(pTexRes->pTex), &(pTexRes->pResView)));
	return S_OK;
}

HRESULT Resource_DestroyTextureFromJPG(void* pData) {
	cd3d11* pd3d11 = Engine_GetCD3D11();
	CE1_ASSERT(pd3d11&&pData);
	SAFE_RELEASE(((TextureResource*)pData)->pResView);
	SAFE_RELEASE(((TextureResource*)pData)->pTex);
	CE1_DEL(pData);
	return S_OK;
}

HRESULT Component_CreateRenderComponent(void * pCreatorDesc, void ** ppData)
{
	CE1_ASSERT(pCreatorDesc);
	RenderComponent* pRC = (RenderComponent*)(*ppData);
	RenderComponentDesc *pDesc = (RenderComponentDesc*)pCreatorDesc; 
	ResourceManager_LoadResource(pDesc->texturename, &pRC->TexResID);
	return S_OK;
}

HRESULT Component_DestroyRenderComponent(void * pData)
{
	CE1_ASSERT(pData);
	RenderComponent* pRC = (RenderComponent*)pData;
	ResourceManager_UnLoadResource(pRC->TexResID);
	return S_OK;
}

HRESULT cd3d11_clearTargets() {
	cd3d11* pd3d11 = Engine_GetCD3D11();
	CE1_ASSERT(pd3d11);
	if (pd3d11->pView && pd3d11->pDepthView)
	{
		pd3d11->pImmediateContext->ClearRenderTargetView(pd3d11->pView, pd3d11->pClsColor);
		pd3d11->pImmediateContext->ClearDepthStencilView(pd3d11->pDepthView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
	return S_OK;
}

HRESULT cd3d11_DELETE(void) {
	cd3d11* pd3d11 = Engine_GetCD3D11();
	CE1_ASSERT(pd3d11);
	pd3d11->pImmediateContext->ClearState();

	cd3d11_VertexShader* pcd3d11VertexShader = (cd3d11_VertexShader*)Vector_Get(pd3d11->pShaders, pd3d11->vsID);
	if (pcd3d11VertexShader) {
		SAFE_RELEASE(pcd3d11VertexShader->pcbVSLighting);
		SAFE_RELEASE(pcd3d11VertexShader->pcbVSMaterial);
		SAFE_RELEASE(pcd3d11VertexShader->pcbVSMatrices);
		SAFE_RELEASE(pcd3d11VertexShader->pVertexLayout11);
		SAFE_RELEASE(pcd3d11VertexShader->pVertexShader);
	}

	cd3d11_PixelShader* pcd3d11PixelShader = (cd3d11_PixelShader*)Vector_Get(pd3d11->pShaders, pd3d11->psID);
	if (pcd3d11PixelShader) {
		SAFE_RELEASE(pcd3d11PixelShader->pcbPSMaterial);
		SAFE_RELEASE(pcd3d11PixelShader->pPixelShader);
	}

	pd3d11->pSwapChain->SetFullscreenState(false, NULL);
	Vector_Delete(pd3d11->pShaders);
	SAFE_RELEASE(pd3d11->DSLessEqual);
	SAFE_RELEASE(pd3d11->RSCullNone);
	SAFE_RELEASE(pd3d11->pRasterizerState);
	SAFE_RELEASE(pd3d11->pView);
	SAFE_RELEASE(pd3d11->pDepthView);
	SAFE_RELEASE(pd3d11->pDepthStencilBuffer);
	SAFE_RELEASE(pd3d11->pSwapChainBuffer);
	SAFE_RELEASE(pd3d11->pSwapChain);
	SAFE_RELEASE(pd3d11->pImmediateContext);
	SAFE_RELEASE(pd3d11->pDebug); 
	SAFE_RELEASE(pd3d11->pDevice);

	SAFE_RELEASE(pd3d11->cbPerObjectBuffer);

	SAFE_RELEASE(pd3d11->pDemoObject->pIndexBuffer);
	SAFE_RELEASE(pd3d11->pDemoObject->pVertexBuffer);
	SAFE_RELEASE(pd3d11->pDemoObject->pSamplerState);
	SAFE_RELEASE(pd3d11->pDemoObject->pResView);
	SAFE_RELEASE(pd3d11->pDemoObject->pTex);
	CE1_DEL(pd3d11->pDemoObject);
	CE1_DEL(pd3d11->pViewport);
	
	CE1_DEL(pd3d11);
	return S_OK;
}

HRESULT cd3d11_getSwapChain(cd3d11* pd3d11) {
	CE1_ASSERT(pd3d11);
	D3D_FEATURE_LEVEL* pFeatureLevels;
	_NEW(D3D_FEATURE_LEVEL, pFeatureLevels);
	(*pFeatureLevels) = D3D_FEATURE_LEVEL_11_0;
	DXGI_SWAP_CHAIN_DESC* pSCD;
	_NEW(DXGI_SWAP_CHAIN_DESC, pSCD);

	pSCD->BufferDesc.Width = Engine_BUFFERWIDTH();
	pSCD->BufferDesc.Height = Engine_BUFFERHEIGHT();
	pSCD->BufferDesc.RefreshRate.Numerator =120;
	pSCD->BufferDesc.RefreshRate.Denominator = 1;
	pSCD->BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//pSCD->BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
	//pSCD->BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
	pSCD->SampleDesc.Count = 1;
	pSCD->SampleDesc.Quality = 0;
	pSCD->BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	pSCD->BufferCount = 2;
	pSCD->OutputWindow = (*(Engine_GetWindowMgr()->phwnd));
	(Engine_FULLSCREEN()) ? pSCD->Windowed = FALSE : pSCD->Windowed = TRUE;
	//pSCD->Windowed = TRUE;
	pSCD->SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	pSCD->Flags = NULL;
	pd3d11->pSwapChain = 0;
	pd3d11->pDevice = 0;
	//pd3d11->FeatureLevel = 0;
	pd3d11->pImmediateContext = 0;
	CE1_CALL(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D10_CREATE_DEVICE_DEBUG,
		pFeatureLevels,
		1,
		D3D11_SDK_VERSION,
		pSCD,
		&(pd3d11->pSwapChain),
		&(pd3d11->pDevice),
		&(pd3d11->FeatureLevel),
		&(pd3d11->pImmediateContext)
	));
	
	pd3d11->pSwapChainBuffer = 0;
	CE1_CALL(pd3d11->pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&pd3d11->pSwapChainBuffer));
	
	pd3d11->pDebug = 0;
	CE1_CALL(pd3d11->pDevice->QueryInterface( __uuidof( ID3D11Debug ), (void**)&pd3d11->pDebug));
	
	pd3d11->pView = 0;
	CE1_CALL(pd3d11->pDevice->CreateRenderTargetView(pd3d11->pSwapChainBuffer, 0, &pd3d11->pView));
	
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc,sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = Engine_BUFFERWIDTH();
	desc.Height = Engine_BUFFERHEIGHT();

	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_D32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	pd3d11->pDepthStencilBuffer = 0;
	CE1_CALL(pd3d11->pDevice->CreateTexture2D( &desc, 0, &pd3d11->pDepthStencilBuffer));

	pd3d11->pDepthView = 0;
	D3D11_DEPTH_STENCIL_VIEW_DESC desc2;
	ZeroMemory(&desc2, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	desc2.Format = DXGI_FORMAT_D32_FLOAT;
	desc2.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	desc2.Flags = NULL;
	CE1_CALL(pd3d11->pDevice->CreateDepthStencilView(pd3d11->pDepthStencilBuffer, &desc2, &pd3d11->pDepthView));

	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_NONE;
	cmdesc.FrontCounterClockwise = true;
	CE1_CALL(pd3d11->pDevice->CreateRasterizerState(&cmdesc, &pd3d11->pRasterizerState));

	return S_OK;
}


//ID3D11Buffer* cd3d11_CreateStructuredBuffer(UINT count, UINT structsize, bool CPUWritable, bool GPUWritable, D3D11_SUBRESOURCE_DATA* pData) {
//	cd3d11* pd3d11 = Engine_GetCD3D11();
//	D3D11_BUFFER_DESC desc;
//	desc.ByteWidth = count * structsize;
//	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
//	desc.StructureByteStride = structsize;
//
//	if (!CPUWritable && !GPUWritable) {
//		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//		desc.Usage = D3D11_USAGE_IMMUTABLE;
//		desc.CPUAccessFlags = 0;
//	}
//	else if (CPUWritable && !GPUWritable) {
//		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//		desc.Usage = D3D11_USAGE_DYNAMIC;
//		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	}
//	else if (!CPUWritable && GPUWritable) {
//		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
//		desc.Usage = D3D11_USAGE_DEFAULT;
//		desc.CPUAccessFlags = 0;
//	}
//	else if (CPUWritable && !GPUWritable) {
//		CE1_CALL(ERROR_SUCCESS);
//	}
//
//	ID3D11Buffer* pBuffer = 0;
//	CE1_CALL(pd3d11->pDevice->CreateBuffer(&desc, pData, &pBuffer));
//
//	return (pBuffer);
//}
//
//ID3D11ShaderResourceView* cd3d11_CreateBufferShaderRessourceView(ID3D11Resource* pResource) {
//	cd3d11* pd3d11 = Engine_GetCD3D11();
//	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
//	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//	desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
//	desc.Buffer.ElementOffset = 0;
//	desc.Buffer.ElementWidth = 100;
//
//	ID3D11ShaderResourceView* pView = 0;
//	CE1_CALL(pd3d11->pDevice->CreateShaderResourceView(pResource, &desc, &pView));
//
//	return (pView);
//}
//
//ID3D11UnorderedAccessView* cd3d11_CreateBufferUnorderedAcessView(ID3D11Resource* pResource) {
//	cd3d11* pd3d11 = Engine_GetCD3D11();
//	D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
//	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//	desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
//	desc.Buffer.FirstElement = 0;
//	desc.Buffer.NumElements = 100;
//	desc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_COUNTER;
//
//	ID3D11UnorderedAccessView* pView = 0;
//	CE1_CALL(pd3d11->pDevice->CreateUnorderedAccessView(pResource, &desc, &pView));
//
//	return (pView);
//}
//
//ID3D11Buffer* cd3d11_CreateAppendConsumeBuffer(UINT size, UINT structsize, D3D11_SUBRESOURCE_DATA* pData) {
//	cd3d11* pd3d11 = Engine_GetCD3D11();
//	D3D11_BUFFER_DESC desc;
//	desc.ByteWidth = size * structsize;
//	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
//	desc.StructureByteStride = structsize;
//	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
//	desc.Usage = D3D11_USAGE_DEFAULT;
//	desc.CPUAccessFlags = 0;
//
//	ID3D11Buffer* pBuffer = 0;
//	CE1_CALL(pd3d11->pDevice->CreateBuffer(&desc, pData, &pBuffer));
//
//	return (pBuffer);
//}
//
//ID3D11Buffer* cd3d11_CreateRawBuffer(UINT size, bool GPUWritable, D3D11_SUBRESOURCE_DATA* pData) {
//	cd3d11* pd3d11 = Engine_GetCD3D11();
//	D3D11_BUFFER_DESC desc;
//	desc.ByteWidth = size;
//	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
//	desc.StructureByteStride = 0;
//	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
//	desc.Usage = D3D11_USAGE_DEFAULT;
//	desc.CPUAccessFlags = 0;
//
//	ID3D11Buffer* pBuffer = 0;
//	CE1_CALL(pd3d11->pDevice->CreateBuffer(&desc, pData, &pBuffer));
//
//	return (pBuffer);
//}
//
//ID3D11Buffer* cd3d11_CreateIndirectArgsBuffer(UINT size, D3D11_SUBRESOURCE_DATA* pData) {
//	cd3d11* pd3d11 = Engine_GetCD3D11();
//	D3D11_BUFFER_DESC desc;
//	desc.ByteWidth = size;
//	desc.MiscFlags = D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
//	desc.StructureByteStride = 0;
//	desc.BindFlags = 0;
//	desc.Usage = D3D11_USAGE_DEFAULT;
//	desc.CPUAccessFlags = 0;
//
//	ID3D11Buffer* pBuffer = 0;
//	CE1_CALL(pd3d11->pDevice->CreateBuffer(&desc, pData, &pBuffer));
//
//	return (pBuffer);
//}
//
//ID3D11ShaderResourceView* cd3d11_CreateShaderResourceView(ID3D11Resource* pResource, D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc) {
//	cd3d11* pd3d11 = Engine_GetCD3D11();
//	ID3D11ShaderResourceView* pView = 0;
//	CE1_CALL(pd3d11->pDevice->CreateShaderResourceView(pResource, pDesc, &pView));
//	return (pView);
//}
//
//ID3D11Buffer* cd3d11_CreateVertexBuffer(UINT size,
//	bool dynamic,
//	bool streamout,
//	D3D11_SUBRESOURCE_DATA* pData) {
//	cd3d11* pd3d11 = Engine_GetCD3D11();
//	D3D11_BUFFER_DESC desc;
//	desc.ByteWidth = size;
//	desc.MiscFlags = 0;
//	desc.StructureByteStride = 0;
//
//	if (streamout) {
//		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
//	}
//	else {
//		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	}
//
//	if (dynamic) {
//		desc.Usage = D3D11_USAGE_DYNAMIC;
//		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	}
//	else {
//		desc.Usage = D3D11_USAGE_IMMUTABLE;
//		desc.CPUAccessFlags = 0;
//	}
//
//	ID3D11Buffer* pBuffer = 0;
//	CE1_CALL(pd3d11->pDevice->CreateBuffer(&desc, pData, &pBuffer));
//	return (pBuffer);
//}
//
//ID3D11Buffer* cd3d11_CreateIndexBuffer(UINT size, bool dynamic, D3D11_SUBRESOURCE_DATA* pData) {
//	cd3d11* pd3d11 = Engine_GetCD3D11();
//	D3D11_BUFFER_DESC desc;
//	desc.ByteWidth = size;
//	desc.MiscFlags = 0;
//	desc.StructureByteStride = 0;
//	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//
//	if (dynamic) {
//		desc.Usage = D3D11_USAGE_DYNAMIC;
//		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	}
//	else {
//		desc.Usage = D3D11_USAGE_IMMUTABLE;
//		desc.CPUAccessFlags = 0;
//	}
//
//	ID3D11Buffer* pBuffer = 0;
//	CE1_CALL(pd3d11->pDevice->CreateBuffer(&desc, pData, &pBuffer));
//
//	return (pBuffer);
//}