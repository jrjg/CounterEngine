#ifndef LISTELEM_INC
#define LISTELEM_INC

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

#endif