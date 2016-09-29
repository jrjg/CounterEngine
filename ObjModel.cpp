#include "TopInclude.h"
#include "Engine.h"
#include "Vector.h"
#include "List.h"
#include "Parser.h"

#include "ObjModel.h"

struct SurfaceMaterial
{
	struct String* name;
	XMFLOAT4 difColor;
	int texArrayIndex;
	bool hasTexture;
	bool transparent;
};

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

struct ObjModelCreator {
	ObjModel* pObjModel;

	bool isRHCoordSys;
	bool computeNormals;
	bool hasNorm;
	String* pMatLibName;
	Vector* pIndices;
	Vector* pVertPos;
	Vector* pVertNorm;
	Vector* pVertTexCoord;
	Vector* pMeshMaterials;
	int vIndex;
};

HRESULT ObjModel_NewCreator(ObjModelCreator** ppObjModelCreator) {
	_NEW(ObjModelCreator,*ppObjModelCreator);
	ObjModelCreator* pObjModelCreator = *ppObjModelCreator;
	CE1_CALL(ObjModel_New(&pObjModelCreator->pObjModel));
	pObjModelCreator->isRHCoordSys = false;
	pObjModelCreator->computeNormals = true;
	pObjModelCreator->pMatLibName = 0;
	pObjModelCreator->pIndices = Vector_New(sizeof(DWORD*), 100);
	pObjModelCreator->pVertPos = Vector_New(sizeof(XMFLOAT3*), 100);
	pObjModelCreator->pVertNorm = Vector_New(sizeof(XMFLOAT3*), 100);
	pObjModelCreator->pVertTexCoord = Vector_New(sizeof(XMFLOAT2*), 100);
	pObjModelCreator->pMeshMaterials = Vector_New(sizeof(String*), 100);
	pObjModelCreator->hasNorm = false;
	pObjModelCreator->vIndex = 0;
	return S_OK;
}



#define CASE(NAME,EXEC)if (CE1_CMPSTR(pChildName->pBuffer, NAME, pChildName->length)) {EXEC;return S_OK;}

HRESULT ObjModel_Handler(void* pParent, String* pChildName, void* pChild) {
	ObjModelCreator* pObjModelCreator = (ObjModelCreator*)pParent;
	CE1_ASSERT(pObjModelCreator&&pChildName&&pChild&&"ObjModel_Handler");

	CASE("#", 1); 
	CASE("v",
		if (pObjModelCreator->isRHCoordSys) { ((XMFLOAT3*)pChild)->z *= -1.0f; }
		Vector_Pushback(pObjModelCreator->pVertPos, pChild);
	);
	CASE("vt",
		if (pObjModelCreator->isRHCoordSys) { ((XMFLOAT2*)pChild)->y *= -1.0f; }
		Vector_Pushback(pObjModelCreator->pVertTexCoord, pChild);
	);
	CASE("vn",
		if (pObjModelCreator->isRHCoordSys) { ((XMFLOAT3*)pChild)->z *= -1.0f; }
		Vector_Pushback(pObjModelCreator->pVertNorm, pChild);
		pObjModelCreator->hasNorm = true;
	);
	CASE("g",
		CE1_NEW(int,pIndex);
		*pIndex = pObjModelCreator->vIndex;
		Vector_Pushback(pObjModelCreator->pObjModel->pMeshSubsetIndexStart, pIndex);
		pObjModelCreator->pObjModel->meshSubsets++;
	);
	CASE("f",
		
	);
	CASE("mtllib",

	);
	CASE("usemtl",

	);
	CE1_ASSERT(0 && "unknown Object received");
}

HRESULT ObjModel_LoadFromFile(String* pObjFilename) {
	Parser* pParser = Parser_New();
	CE1_CALL(Parser_DeclareVariable(pParser, "String", "mtllib", &ObjectModelHandler));
	CE1_CALL(Parser_DeclareVariable(pParser, "String", "usemtl", &ObjectModelHandler));
	CE1_CALL(Parser_DeclareVariable(pParser, "XMFLOAT3", "vt", &ObjectModelHandler));
	CE1_CALL(Parser_DeclareVariable(pParser, "XMFLOAT3", "vn", &ObjectModelHandler));
	CE1_CALL(Parser_DeclareVariable(pParser, "XMFLOAT3", "v", &ObjectModelHandler));
	CE1_CALL(Parser_DeclareVariable(pParser, "String", "g", &ObjectModelHandler));
	CE1_CALL(Parser_DeclareVariable(pParser, "String", "#", &ObjectModelHandler));


	//CE1_CALL(Parser_DeclareType(pParser,"XMFLOAT3",sizeof(XMFLOAT3),&Parser_ConvertStringToVertex));
	CE1_CALL(Parser_RegisterOperator(pParser, "\n", OperatorCode::submit));

	//parse
	ObjModelCreator* pObjModelCreator;
	CE1_CALL(ObjModel_NewCreator(&pObjModelCreator));
	CE1_CALL(Parser_ParseFile(pParser, "spaceCompound.obj", &ObjectModelHandler, pObjModelCreator));

}

HRESULT ObjModel_New(ObjModel ** ppObjModel)
{
	_NEW(ObjModel,*ppObjModel);
	ObjModel* pObjModel = *ppObjModel;
	pObjModel->alive = true;

	int expectedMeshSubsetCount = 100;
	pObjModel->pMeshSubsetIndexStart = Vector_New(sizeof(int), expectedMeshSubsetCount);
	pObjModel->pMeshSubsetTexture = Vector_New(sizeof(int), expectedMeshSubsetCount);

	int expectedTextureCount = 10;
	pObjModel->pMeshSRV = Vector_New(sizeof(ID3D11ShaderResourceView*), expectedTextureCount);
	pObjModel->pTextureNameArray = Vector_New(sizeof(String*), expectedTextureCount);

	pObjModel->pSurfaceMaterial = Vector_New(sizeof(SurfaceMaterial*), expectedTextureCount);

	pObjModel->meshWorld = XMMatrixIdentity();
	return S_OK;
}

HRESULT ObjModel_Delete(ObjModel* pObjModel)
{
	if (!pObjModel) { return S_OK; }
	CE1_ASSERT(!pObjModel->alive && "Deleting dead ObjModel");
	pObjModel->alive = false;
	CE1_CALL(Vector_Delete(pObjModel->pMeshSubsetIndexStart));
	CE1_CALL(Vector_Delete(pObjModel->pMeshSubsetTexture));

	ID3D11ShaderResourceView* pMeshSRV;
	CE1_VECEXEC(
		pObjModel->pMeshSRV,
		pMeshSRV = (ID3D11ShaderResourceView*)Vector_Get(pObjModel->pMeshSRV, i);
		SAFE_RELEASE(pMeshSRV);
	);
	CE1_CALL(Vector_Delete(pObjModel->pMeshSRV));

	SurfaceMaterial* pSurfaceMaterial;
	CE1_VECEXEC(
		pObjModel->pSurfaceMaterial,
		pSurfaceMaterial = (SurfaceMaterial*)Vector_Get(pObjModel->pSurfaceMaterial, i);
		CE1_DEL(pSurfaceMaterial->name->pBuffer);
		CE1_DEL(pSurfaceMaterial->name);
	);
	CE1_CALL(Vector_FullDelete(pObjModel->pSurfaceMaterial));

	CE1_CALL(Vector_FullDelete(pObjModel->pTextureNameArray));

	CE1_DEL(pObjModel);
	return S_OK;
}
