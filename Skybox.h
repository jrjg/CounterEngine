#ifndef SKYBOX_INC
#define SKYBOX_INC

//struct Skybox_ConstantBuffer
//{
//	XMMATRIX  WVP;
//	XMMATRIX World;
//};
//typedef struct Skybox_ConstantBuffer Skybox_ConstantBuffer;

struct Float3 {
	float x;
	float y;
	float z;
};
typedef struct Float3 Float3;

struct Skybox {
	bool alive;
	ID3D11Buffer* sphereIndexBuffer;
	ID3D11Buffer* sphereVertBuffer;

	ID3D11VertexShader* SKYMAP_VS;
	ID3D11PixelShader* SKYMAP_PS;
	ID3D10Blob* SKYMAP_VS_Buffer;
	ID3D10Blob* SKYMAP_PS_Buffer;

	ID3D11ShaderResourceView* smrv;

	int NumSphereVertices;
	int NumSphereFaces;

	XMMATRIX sphereWorld;

	XMMATRIX Rotationx;
	XMMATRIX Rotationy;

	Float3* pVertices;
	DWORD* pIndices;

	UINT stride;
	UINT offset;

	/*ID3D11Buffer* cbPerObjectBuffer;

	cbPerObject cbPerObj;

	XMMATRIX WVP;*/
};
typedef struct Skybox Skybox;

HRESULT Skybox_New(Skybox** ppSkybox);
HRESULT Skybox_CreateSphere(Skybox* pSkybox,int LatLines, int LongLines);
HRESULT Skybox_Delete(void* p0);
HRESULT Skybox_CreateShaders(Skybox* pSkybox);
HRESULT Skybox_Run(TIME elapsed);
HRESULT Skybox_Startup(void* p0);
HRESULT Skybox_CreateRasterizerAndDepthStencilState(Skybox* pSkybox);
HRESULT Skybox_Render(Skybox* pSkybox);
#endif