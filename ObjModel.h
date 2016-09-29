#ifndef OBJMODEL_INC
#define OBJMODEL_INC

struct SurfaceMaterial;
typedef struct SurfaceMaterial SurfaceMaterial;
struct ObjModel;
typedef struct ObjModel ObjModel;
struct ObjModelCreator;
typedef struct ObjModelCreator ObjModelCreator;

HRESULT ObjModel_New(ObjModel** ppObjModel);
HRESULT ObjModel_Delete(ObjModel* pObjModel);

#endif