#ifndef INCLUDE_LIST
#define INCLUDE_LIST

extern ID List_ID;
struct ListElement
{
	struct ListElement* _pNext;
	void* _pObj;
	ID _id;
};
typedef struct ListElement ListElement;
struct List {
	ListElement* _pFirstElem;
	ListElement* _pLastElem;
	unsigned long _length;
	unsigned long _elemsize;
	ID _idcnt;
	ID _id;
};
typedef struct List List;
typedef ListElement* Iterator;
ID List_PushBack(List*, void*);				// adds element to the list's front				
HRESULT List_FullDelete(List*, BOOL deleteObject);
HRESULT List_DeleteAllElements(List*, BOOL deleteObject);			//delete all elements contained by the list
HRESULT List_DeleteElement(List*, ID, BOOL deleteObject);			// deletes element and also object
ListElement* List_Next(ListElement*);		// returns the ´next element of the element pointed at with the itarator
unsigned int List_Length(List*);			// returns the length of the list
Iterator List_Iterator(List*);				// return list iterator
struct List* List_New(unsigned long);		// returns new list
void* List_Get(ListElement*);				// returns object contained by given element
void* List_Pop(List*);						// returns object of the first element, deletes first element, does not delete object
HRESULT List_GetLast(List* pList, void** ppObject);
HRESULT List_Copy(List* pFrom, List* pTo);
HRESULT List_Get(List* pList, ID id, void** ppObject);
#endif // !INCLUDE_LIST