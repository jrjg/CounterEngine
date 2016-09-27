#ifndef OBJMODEL_INC
#define OBJMODEL_INC

struct SurfaceMaterial
{
	struct String* name;
	XMFLOAT4 difColor;
	int texArrayIndex;
	bool hasTexture;
	bool transparent;
};
typedef struct SurfaceMaterial SurfaceMaterial;

struct ObjModel {
	bool alive;
	ID3D11BlendState* Transparency;

	ID3D11Buffer* meshVertBuff;
	ID3D11Buffer* meshIndexBuff;
	XMMATRIX meshWorld;
	int meshSubsets = 0;
	struct Vector* pMeshSubsetIndexStart;
	struct Vector* pMeshSubsetTexture;

	struct Vector* pMeshSRV;
	struct Vector* pTextureNameArray;

	struct Vector* pSurfaceMaterial;
};
typedef struct ObjModel ObjModel;

HRESULT ObjModel_New(ObjModel** ppObjModel);
HRESULT ObjModel_Delete(ObjModel* pObjModel);

#endif