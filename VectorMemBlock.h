#ifndef VECTORMEMBLOCK
#define VECTORMEMBLOCK

#include "MemManaged.h"

template<class ObjectType> 
class VectorMemBlock : public MemManaged {
private:
	UINT mIndexLast;
	MemManaged* mpMem;
	bool mDeleteContent;
	UINT mElemSize;
	UINT mCapacity;
	size_t mSizeOfMemManaged;
protected:
	virtual ~VectorMemBlock();
public:
	ObjectType* VectorMemBlock<ObjectType>::get(UINT index);
	UINT getIndexLast() { return mIndexLast; };
	void setDeleteContent(bool b) { mDeleteContent = b; };
	HRESULT set(UINT index, ObjectType* pObj);
	HRESULT pushback(ObjectType* pObj, UINT* pIndex);
	VectorMemBlock(UINT capacity);
};

template<class ObjectType>
inline ObjectType * VectorMemBlock<ObjectType>::get(UINT index)
{
	if (index > mCapacity) { return NULL; };
	return(*(ObjectType**)((char*)(mpMem)+mSizeOfMemManaged + index * mElemSize));
};

template<class ObjectType>
inline HRESULT VectorMemBlock<ObjectType>::set(UINT index, ObjectType * pObj)
{
	if (index > mCapacity) { return ERROR_SUCCESS; };
	*(ObjectType**)((char*)(mpMem)+mSizeOfMemManaged + index * mElemSize) = pObj;
	if (index > mIndexLast) {
		mIndexLast = index;
	}
	return S_OK;
};

template<class ObjectType>
inline HRESULT VectorMemBlock<ObjectType>::pushback(ObjectType * pObj, UINT * pIndex)
{
	if (SUCCEEDED(set(mIndexLast + 1, pObj))) {
		*pIndex = ++mIndexLast;
		return S_OK;
	}
	return ERROR_SUCCESS;
};

template<class ObjectType>
VectorMemBlock<ObjectType>::VectorMemBlock(UINT capacity) : mSizeOfMemManaged(sizeof(MemManaged)), mElemSize(sizeof(ObjectType)), mCapacity(capacity), mDeleteContent(true), mIndexLast(-1) {
	MemoryManager::get()->allocateMem(&mpMem, mCapacity * mElemSize + mSizeOfMemManaged);
};

template<class ObjectType>
inline VectorMemBlock<ObjectType>::~VectorMemBlock()
{
	if (mDeleteContent) {
		ObjectType* pObject;
		for (UINT i = 0; i < mCapacity; i++) {
			pObject = get(i);
			if (pObject) { pObject->release(); };
		}
	}
	mpMem->release();
};

#endif