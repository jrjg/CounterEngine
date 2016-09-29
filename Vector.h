#ifndef INCLUDE_VECTOR
#define INCLUDE_VECTOR

extern ID Vector_ID;
typedef struct Vector Vector;
Vector* Vector_New(unsigned long, unsigned int);
HRESULT Vector_Delete(Vector*);
HRESULT Vector_FullDelete(Vector*, bool deletecontent);
HRESULT Vector_DeleteElement(Vector*, unsigned int, bool deletecontent);
HRESULT Vector_DeleteAllElements(Vector*, bool deletecontent);
HRESULT Vector_Resize(Vector*, unsigned int);
HRESULT Vector_Insert(Vector*, unsigned int, void*);
unsigned int Vector_Elements(Vector*);
void* Vector_Get(Vector* pVector, unsigned int);
unsigned int Vector_Capacity(Vector*);
unsigned int Vector_Last(Vector*);
unsigned int Vector_Pushback(Vector*, void*);
HRESULT Vector_GetMem(Vector* pVector, void** ppMem);
HRESULT Vector_Erase(Vector* pVector,UINT index, bool deletecontent);

#endif // !INCLUDE_VECTOR