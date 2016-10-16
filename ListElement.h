#ifndef LISTELEM_INC
#define LISTELEM_INC

#include "MemManaged.h"

template<class ObjectType>class List;

template <class ObjectType>
class ListElement : public MemManaged
{
private:
	ListElement<ObjectType>* mpPrevious;
	ListElement<ObjectType>* mpNext;
	ObjectType* mpObject;
	ID mID;
	bool mDeleteContent;
protected:
	virtual ~ListElement() { if (mDeleteContent) { SAFE_RELEASE(mpObject); }; };
public:
	ListElement(ObjectType* pObj, ID id, ListElement<ObjectType>* pNext, ListElement<ObjectType>* pPrevious, bool memManageContent) : mpObject(pObj), mID(id), mDeleteContent(memManageContent), mpNext(pNext), mpPrevious(pPrevious) {};
	ID getID() { return mID; };
	ListElement<ObjectType>* getNext() { return mpNext; };
	ListElement<ObjectType>* getPrevious() { return mpPrevious; };
	void setNext(ListElement<ObjectType>* p) { mpNext = p; };
	void setPrevious(ListElement<ObjectType>* p) { mpPrevious = p; };
	ObjectType* getObject() { return mpObject; };
	void setDeleteContent(bool b) { mDeleteContent = b; };
};

#endif