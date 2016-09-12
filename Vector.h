#ifndef INCLUDE_VECTOR
#define INCLUDE_VECTOR

extern ID Vector_ID;
typedef struct Vector Vector;
Vector* Vector_New(unsigned long, unsigned int);
BOOL Vector_Delete(Vector*);
BOOL Vector_FullDelete(Vector*);
BOOL Vector_DeleteElement(Vector*, unsigned int);
BOOL Vector_DeleteAllElements(Vector*);
BOOL Vector_Resize(Vector*, unsigned int);
BOOL Vector_Insert(Vector*, unsigned int, void*);
unsigned int Vector_Elements(Vector*);
void* Vector_Get(Vector* pVector, unsigned int);
unsigned int Vector_Capacity(Vector*);
unsigned int Vector_Last(Vector*);
unsigned int Vector_Pushback(Vector*, void*);

#endif // !INCLUDE_VECTOR