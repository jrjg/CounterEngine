#ifndef INCLUDE_LIST
#define INCLUDE_LIST

template <class ObjectType> class ListElement : public MemManaged
{
private:
	ListElement<ObjectType>* mpPrevious;
	ListElement<ObjectType>* mpNext;
	ObjectType* mpObject;
	ID mID;
	bool mDeleteContent;
	List<ObjectType>* mpList;
public:
	ListElement<ObjectType>::ListElement(List<ObjectType>* pList, ObjectType* pObj, ID id, ListElement<ObjectType>* pNext, ListElement<ObjectType>* pPrevious, bool memManageContent) : mpList(pList), mpObject(pObj), mID(id), mDeleteContent(deleteContent), mpNext(pNext), mpPrevious(pPrevious) {};
	ListElement<ObjectType>::~ListElement();
	ID ListElement<ObjectType>::getID() { return mID; };
	ListElement<ObjectType>* ListElement<ObjectType>::getNext() { return mpNext; };
	ListElement<ObjectType>* ListElement<ObjectType>::getPrevious() { return mpPrevious; };
	void ListElement<ObjectType>::setNext(ListElement<ObjectType>* p) { mpNext = p; };
	void ListElement<ObjectType>::setPrevious(ListElement<ObjectType>* p) { mpPrevious = p; };
	ObjectType* ListElement<ObjectType>::getObject() { return mpObject; };
	void ListElement<ObjectType>::setDeleteContent(bool b) { mDeleteContent = b; };
};

template <class ObjectType> ListElement<ObjectType>::~ListElement()
{
	mpList->setLength(mpList->getLength() - 1);

	if (mpList->getLength() == 1) {
		mpList->setFirst(NULL);
		mpList->setLast(NULL);
		return;
	}
	if (mpList->getFirst()->mID == mID) {
		mpNext ? mpNext->setPrevious(NULL) : 1;
		mpList->setFirst(mpNext);
		return;
	}
	if (mpList->getLast()->mID == mID) {
		mpPrevious ? mpPrevious->setNext(NULL) : 1;
		mpList->setLast(mpPrevious);
		return;
	}
	mpNext->setPrevious(mpPrevious);
	mpPrevious->setNext(mpNext);

	(mDeleteContent) ? delete mpObject : 1;
	return;
}

template <class ObjectType> class List : public MemManaged {
private:
	ListElement<ObjectType>* mpFirstElem;
	ListElement<ObjectType>* mpLastElem;
	UINT mLength;
	ID mElemIDCounter;
	bool mDeleteContent;
public:
	List<ObjectType>::List(bool deleteContent) : mpFirstElem(NULL), mpLastElem(NULL), mLength(0), mElemIDCounter(0), mDeleteContent(deleteContent) {};
	List<ObjectType>::~List();
	ID List<ObjectType>::pushBack(void* pObj);
	ID List<ObjectType>::pushFront(void* pObj);
	
	unsigned int List<ObjectType>::getLength() { return mLength; };
	void List<ObjectType>::setLength(unsigned int length) {mLength = length;};
	ListElement<ObjectType>* List<ObjectType>::getFirst() { return mpFirstElem; };
	HRESULT List<ObjectType>::setFirst(ListElement<ObjectType>* pElem) {mpFirstElem = pElem;};
	HRESULT List<ObjectType>::setLast(ListElement<ObjectType>* pElem) { mpLastElem = pElem; };
	ObjectType* List<ObjectType>::pop();
	ListElement<ObjectType>* List<ObjectType>::getLast() {return mpLastElem;};
	void* List<ObjectType>::getByID(ID id);
	HRESULT List<ObjectType>::deleteByID(ID id);
};

template<class ObjectType>
inline HRESULT List<ObjectType>::deleteByID(ID id)
{
	for (ListElement<ObjectType>* pElem = mpFirstElem; pElem != NULL; pElem = pElem->getNext()) {
		if (pElem->getID() == id) {
			delete pElem;
			return S_OK;
		}
	}
	return S_OK;
}

template <class ObjectType> 
void* List<ObjectType>::getByID(ID id) {
	ObjectType* pObj = nullptr;
	for (ListElement<ObjectType>* pElem = mpFirstElem; pElem != NULL; pElem = pElem->getNext()) {
		if (pElem->getID() == id) {
			pObj = pElem->getObject();
			return pObj;
		}
	}
	return pObj;
}

template <class ObjectType> 
ObjectType* List<ObjectType>::pop()
{
	(mLength == 0) ? return NULL : 1;
	ObjectType* pObj = mpFirstElem->getObject();
	mpFirstElem->setDeleteContent(false); //dont delete what is returned
	delete mpFirstElem;
	return pObj;
}

template <class ObjectType> 
ID List<ObjectType>::pushBack(void* pObject)
{
	ListElement<ObjectType>* pNewElem = new ListElement(this, pObject, mElemIDCounter++, NULL, mpLastElem, mDeleteContent);
	(mLength == 0) ? mpFirstElem = pNewElem : mpLastElem->setNext(pNewElem);
	mpLastElem = pNewElem;
	mLength++;
	return pNewElem->getID();
}

template <class ObjectType>
ID List<ObjectType>::pushFront(void* pObject)
{
	ListElement<ObjectType>* pNewElem = new ListElement(this, pObject, mElemIDCounter++, mpFirstElem , NULL, mDeleteContent);
	(mLength == 0) ? mpLastElem = pNewElem : mpFirstElem->setPrevious(pNewElem);
	mpFirstElem = pNewElem;
	mLength++;
	return pNewElem->getID();
}

template <class ObjectType> 
List<ObjectType>::~List() {
	ListElement<ObjectType>* pElem2 = NULL;
	for (ListElement<ObjectType>* pElem = mpFirstElem; pElem != NULL; pElem = pElem->getNext()) {
		delete pElem2;
		pElem2 = pElem;
	}
	return;
}

#endif