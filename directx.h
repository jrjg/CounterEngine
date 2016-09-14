#ifndef D3D11_INC
#define D3D11_INC

const D3D11_INPUT_ELEMENT_DESC D3D11VertexLayout_UnlitTextured[] =
{
	{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC D3D11VertexLayout_UnlitTexturedColored[] =
{
	{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,    0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC D3D11VertexLayout_PositionColored[] =
{
	{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC D3D11VertexLayout_Position[] =
{
	{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

struct Vector4d_FLOAT {
	float x;
	float y;
	float z;
	float w;
}; 
typedef struct Vector4d_FLOAT Vector4d_FLOAT;

typedef struct D3DXCOLOR {
	FLOAT r;
	FLOAT g;
	FLOAT b;
	FLOAT a;
} COLOR;

typedef struct D3DMATERIAL9 {
	D3DCOLORVALUE Diffuse;
	D3DCOLORVALUE Ambient;
	D3DCOLORVALUE Specular;
	D3DCOLORVALUE Emissive;
	float         Power;
} Material;


//
//struct cd3d11_ConstantBuffer_Matrices {
//	 Mat4x4 m_WorldViewProj;
//	Mat4x4 m_World;
//};
//
//struct cd3d11_ConstantBuffer_Material {
//	Vec4 m_vDiffuseObjectColor;
//	Vec4 m_vAmbientObjectColor;
//	BOOL m_bHasTexture;
//	Vec3 m_vUnused;
//};

struct cd3d11_VertexShader {
	ID3D11VertexShader* pVertexShader;
	ID3D11InputLayout* pVertexLayout11;
	ID3D11Buffer* pcbVSMatrices;
	ID3D11Buffer* pcbVSMaterial;
	ID3D11Buffer* pcbVSLighting;
	bool enable_lights;
};

struct cd3d11_PixelShader {
	ID3D11PixelShader* pPixelShader;
	ID3D11Buffer* pcbPSMaterial;
	void* pTextureRessource;
};
typedef struct VertexObject VertexObject;
struct cd3d11 {
	IDXGISwapChain *pSwapChain;
	ID3D11Device *pDevice;
	D3D_FEATURE_LEVEL FeatureLevel;
	ID3D11DeviceContext *pImmediateContext;
	ID3D11Texture2D* pSwapChainBuffer;
	ID3D11Debug* pDebug;
	ID3D11RenderTargetView* pView;
	ID3D11Texture2D* pDepthStencilBuffer;
	ID3D11DepthStencilView* pDepthView;
	VertexObject* pDemoObject;
	struct Vector* pShaders;
	unsigned int vsID;
	unsigned int psID;
	D3D11_VIEWPORT* pViewport;
	FLOAT pClsColor[4];

	XMMATRIX WorldViewProjection;
	XMMATRIX World;
	float rot = 0.0f;
	XMMATRIX camView;
	XMMATRIX camProjection;
};

HRESULT cd3d11_SetTexture(ID3D11ShaderResourceView* const *pDiffuseRV, ID3D11SamplerState * const *ppSamplers);
HRESULT cd3d11_CreateAndSetViewport(cd3d11* pd3d11);

struct ConstantBuffer_Matrices {
	XMFLOAT4X4 WorldViewProj;
	XMFLOAT4X4 World;
};

struct TextureResource{
	ID3D11ShaderResourceView* pResView;
	ID3D11Resource* pTex;
};
typedef struct TextureResource TextureResource;

struct RenderComponent {
	ID3D11Buffer* pIndexBuffer;
	ID3D11Buffer* pVertexBuffer;
	UINT stride;
	UINT numPolys;
	ID TexResID;
};
typedef struct RenderComponent RenderComponent;

struct RenderComponentDesc {
	char* texturename;
};
typedef RenderComponentDesc RenderComponentDesc;

struct ConstantBuffer_Material {
	XMFLOAT4 DiffuseObjectColor;
	XMFLOAT4 AmbientObjectColor;
	BOOL hasTexture;
	XMFLOAT3 Unused;
};

struct D3D11Vertex_UnlitTextured
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 Uv;
};

struct VertexObject {
	ID3D11Buffer* pIndexBuffer;
	ID3D11Buffer* pVertexBuffer;
	UINT stride;
	UINT numPolys;
	ID3D11ShaderResourceView* pResView;
	ID3D11SamplerState* pSamplerState;
	ID3D11Resource* pTex;
};

struct ConstantBuffer_Lighting {
	XMFLOAT4 LightDiffuse[MAXIMUM_LIGHTS_SUPPORTED];
	XMFLOAT4 LightDir[MAXIMUM_LIGHTS_SUPPORTED];
	XMFLOAT4 LightAmbient;
	UINT NumLights;
	XMFLOAT3 Unused;
};
typedef struct ConstantBuffer_Lighting ConstantBuffer_Lighting;
typedef struct ConstantBuffer_Material ConstantBuffer_Material; 
typedef struct cd3d11 cd3d11;
typedef struct ConstantBuffer_Matrices ConstantBuffer_Matrices;
typedef struct cd3d11_PixelShader cd3d11_PixelShader;

HRESULT Resource_CreateTextureFromJPG(char* name, void** ppData);
HRESULT Resource_DestroyTextureFromJPG(void* pData);

HRESULT Component_CreateRenderComponent(void* pCreatorDesc, void** ppData);
HRESULT Component_DestroyRenderComponent(void* pData);

HRESULT cd3d11_getSwapChain(cd3d11*);
//BOOL cd3d11_setupRenderVertexShader(Scene * pScene, SceneNode * pSceneNode);
//BOOL cd3d11_setupRenderPixelShader(Scene * pScene, SceneNode * pSceneNode);
HRESULT cd3d11_setupRenderVertexShader();
HRESULT cd3d11_setupRenderPixelShader();
HRESULT cd3d11_createVertexShader(cd3d11* pd3d11, LPCSTR szEntryPoint, LPCWSTR name, LPCSTR szShaderModel, cd3d11_VertexShader**);
HRESULT cd3d11_createPixelShader(cd3d11* pd3d11, LPCSTR szEntryPoint, LPCWSTR name, LPCSTR szShaderModel, cd3d11_PixelShader**);

HRESULT cd3d11_NEW(cd3d11**);
HRESULT cd3d11_DELETE();
HRESULT cd3d11_Run(TIME);
HRESULT cd3d11_clearTargets();
HRESULT cd3d11_render();

HRESULT cd3d11_CreateDemoObject(cd3d11* pd3d11);
//BOOL cd3d11_PrepareInputAssembler(ID3D11Buffer* pIndexBuffer, ID3D11Buffer* pVertexBuffer, UINT* pStride);
HRESULT cd3d11_registerHandlers(cd3d11* pd3d11);

#endif