#include "TopInclude.h"
#include "Engine.h"
#include "Vector.h"
#include "List.h"
#include "Parser.h"

#include "ObjModel.h"


#define CASE(NAME,EXEC)if (CE1_CMPSTR(pChildName->pBuffer, NAME, pChildName->length)) {EXEC;return S_OK;}

HRESULT ObjModel_Handler(void* pParent, String* pChildName, void* pChild) {
	CASE("#", 1); 
	CASE("v",

	);
	CASE("vt",

	);
	CASE("vn",

	);
	CASE("g",

	);
	CASE("mtllib",

	);
	CASE("usemtl",

	);
	CE1_ASSERT(0 && "unknown Object received");
}

HRESULT ObjModel_LoadFromFile(String* pObjFilename) {
	bool isRHCoordSys = false;
	bool computeNormals = true;
	String* pMatLibName;

	Vector* pIndices = Vector_New(sizeof(DWORD*),100);
	Vector* pVertPos = Vector_New(sizeof(XMFLOAT3*), 100);
	Vector* pVertNorm = Vector_New(sizeof(XMFLOAT3*), 100);
	Vector* pVertTexCoord = Vector_New(sizeof(XMFLOAT2*), 100);

	Vector* pMeshMaterials = Vector_New(sizeof(String*), 100);

	//Vertex definition indices
	std::vector<int> vertPosIndex;
	std::vector<int> vertNormIndex;
	std::vector<int> vertTCIndex;


	Parser* pParser = Parser_New();
	//declare variables
	CE1_CALL(Parser_DeclareVariable(pParser, "String", "#", &ObjectModelHandler));
	CE1_CALL(Parser_DeclareVariable(pParser, "XMFLOAT3", "v", &ObjectModelHandler));
	CE1_CALL(Parser_DeclareVariable(pParser, "XMFLOAT3", "vt", &ObjectModelHandler));
	CE1_CALL(Parser_DeclareVariable(pParser, "XMFLOAT3", "vn", &ObjectModelHandler));
	CE1_CALL(Parser_DeclareVariable(pParser, "String", "g", &ObjectModelHandler));
	CE1_CALL(Parser_DeclareVariable(pParser, "String", "mtllib", &ObjectModelHandler));
	CE1_CALL(Parser_DeclareVariable(pParser, "String", "usemtl", &ObjectModelHandler));

	//CE1_CALL(Parser_DeclareType(pParser,"XMFLOAT3",sizeof(XMFLOAT3),&Parser_ConvertStringToVertex));

	//register operators
	CE1_CALL(Parser_RegisterOperator(pParser, "\n", OperatorCode::submit));

	//parse
	CE1_CALL(Parser_ParseFile(pParser, "spaceCompound.obj", &ObjectModelHandler));



	CE1_NEW(ObjModel, pObject);
	pObject;

	Vector* pMaterial = Vector_New(sizeof(SurfaceMaterial*), 100);
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
