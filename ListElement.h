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
	List<ObjectType>* mpList;
protected:
	virtual ~ListElement();
public:
	ListElement(List<ObjectType>* pList, ObjectType* pObj, ID id, ListElement<ObjectType>* pNext, ListElement<ObjectType>* pPrevious, bool memManageContent);
	ID getID() { return mID; };
	ListElement<ObjectType>* getNext() { return mpNext; };
	ListElement<ObjectType>* getPrevious() { return mpPrevious; };
	void setNext(ListElement<ObjectType>* p) { mpNext = p; };
	void setPrevious(ListElement<ObjectType>* p) { mpPrevious = p; };
	ObjectType* getObject() { return mpObject; };
	void setDeleteContent(bool b) { mDeleteContent = b; };
};

template<class ObjectType>
inline ListElement<ObjectType>::ListElement(List<ObjectType>* pList, ObjectType * pObj, ID id, ListElement<ObjectType>* pNext, ListElement<ObjectType>* pPrevious, bool memManageContent) : mpList(pList), mpObject(pObj), mID(id), mDeleteContent(memManageContent), mpNext(pNext), mpPrevious(pPrevious)
{
	if (!pNext) { //pushback
		if (pPrevious) { pPrevious->setNext(this); };
		mpList->setLast(this);
	}
	if (!pPrevious) { //pushfront
		if (pNext) { pNext->setPrevious(this); };
		mpList->setFirst(this);
	}
	mpList->setLength(mpList->getLength() + 1);
};

template <class ObjectType> ListElement<ObjectType>::~ListElement()
{
	if (!mpList) { if (mDeleteContent) { SAFE_RELEASE(mpObject); }; return; };
	if (mpList->getLength() == 1) {
		mpList->setFirst(NULL);
		mpList->setLast(NULL);
	}
	else {
		if (mpList->getFirst()->mID == mID) {
			if (mpNext) { mpNext->setPrevious(NULL); };
			mpList->setFirst(mpNext);
		}
		else if (mpList->getLast()->mID == mID) {
			if (mpPrevious) { mpPrevious->setNext(NULL); };
			mpList->setLast(mpPrevious);
		}
		else {
			mpNext->setPrevious(mpPrevious);
			mpPrevious->setNext(mpNext);
		}
	}
	mpList->setLength(mpList->getLength() - 1);
	if (mDeleteContent) { SAFE_RELEASE(mpObject); };
	return;
};

#endif