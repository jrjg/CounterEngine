#ifndef UNMANAGEDLISTELEMENT
#define UNMANAGEDLISTELEMENT

#include "MemManaged.h"

template<class ObjectType>class UnManagedList;

template <class ObjectType>
class UnManagedListElement
{
private:
	UnManagedListElement<ObjectType>* mpPrevious;
	UnManagedListElement<ObjectType>* mpNext;
	ObjectType* mpObject;
	ID mID;
	bool mDeleteContent;
protected:
	virtual ~UnManagedListElement() { if (mDeleteContent) { SAFE_RELEASE(mpObject); }; };
public:
	UnManagedListElement(ObjectType* pObj, ID id, UnManagedListElement<ObjectType>* pNext, UnManagedListElement<ObjectType>* pPrevious, bool memManageContent) : mpObject(pObj), mID(id), mDeleteContent(memManageContent), mpNext(pNext), mpPrevious(pPrevious) {};
	ID getID() { return mID; };
	UnManagedListElement<ObjectType>* getNext() { return mpNext; };
	UnManagedListElement<ObjectType>* getPrevious() { return mpPrevious; };
	void setNext(UnManagedListElement<ObjectType>* p) { mpNext = p; };
	void setPrevious(UnManagedListElement<ObjectType>* p) { mpPrevious = p; };
	ObjectType* getObject() { return mpObject; };
	void setDeleteContent(bool b) { mDeleteContent = b; };
	bool getDeleteContent() { return mDeleteContent; };
	HRESULT Release() { delete this; return S_OK; };
};

#endif