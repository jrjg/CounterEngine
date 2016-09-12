#ifndef INCLUDE_LIST
#define INCLUDE_LIST

extern ID List_ID;
typedef struct ListElement ListElement;
typedef struct List List;
typedef ListElement* Iterator;
ID List_PushBack(List*, void*);				// adds element to the list's front
BOOL List_Delete(List*);					// deletes whole list and objects
BOOL List_FullDelete(List*);
BOOL List_DeleteAllElements(List*);			//delete all elements contained by the list
BOOL List_DeleteElement(List*, ID);			// deletes element and also object
ListElement* List_Next(ListElement*);		// returns the ´next element of the element pointed at with the itarator
unsigned int List_Length(List*);			// returns the length of the list
Iterator List_Iterator(List*);				// return list iterator
struct List* List_New(unsigned long);		// returns new list
void* List_Get(ListElement*);				// returns object contained by given element
void* List_Pop(List*);						// returns object of the first element, deletes first element, does not delete object

#endif // !INCLUDE_LIST