#ifndef VECTORMEMBLOCK
#define VECTORMEMBLOCK

#include "MemManaged.h"
#include "Memory.h"

template<class ObjectType> 
class VectorMemBlock : public MemManaged {
	friend class Vector<ObjectType>;
private:
	UINT mIndexLast;
	void* mpMem;
	bool mDeleteContent;
	UINT mElemSize;
	UINT mCapacity;
	ID mBlockMemID;
	UINT mLength;
protected:
	virtual ~VectorMemBlock();
	ObjectType* VectorMemBlock<ObjectType>::get(UINT index);
	UINT getIndexLast() { return mIndexLast; };
	void setDeleteContent(bool b) { mDeleteContent = b; };
	HRESULT set(UINT index, ObjectType* pObj);
	HRESULT pushback(ObjectType* pObj, UINT* pIndex);
	VectorMemBlock(UINT capacity) : mCapacity(capacity), mpMem(NULL) { restore(); };
	HRESULT deleteVectorMemBlockElement(UINT index);
	HRESULT restore();
};

template<class ObjectType>
inline ObjectType * VectorMemBlock<ObjectType>::get(UINT index)
{
	if (index > mCapacity) { return NULL; };
	return(*(ObjectType**)(((char*)mpMem) + index * mElemSize));
};

template<class ObjectType>
inline HRESULT VectorMemBlock<ObjectType>::set(UINT index, ObjectType * pObj)
{
	if (index > mCapacity) { return ERROR_SUCCESS; };
	deleteVectorMemBlockElement(index);
	*(ObjectType**)(((char*)mpMem) + index * mElemSize) = pObj;
	if (index > mIndexLast) {
		mIndexLast = index;
	}
	mLength++;
	return S_OK;
};

template<class ObjectType>
inline HRESULT VectorMemBlock<ObjectType>::pushback(ObjectType * pObj, UINT * pIndex)
{
	UINT index;
	if (mLength == 0) {
		index = mIndexLast;
	}
	else {
		index = mIndexLast + 1;
	}
	if (SUCCEEDED(set(index, pObj))) {
		*pIndex = index;
		return S_OK;
	}
	return ERROR_SUCCESS;
};

template<class ObjectType>
inline HRESULT VectorMemBlock<ObjectType>::deleteVectorMemBlockElement(UINT index)
{
	ObjectType* pVectorMemBlockElement = get(index);
	if (pVectorMemBlockElement) {
		if (mIndexLast == index && mIndexLast != 0) {
			while(!get(--mIndexLast)){
				if (mIndexLast == 0) { 
					break; 
				}
			}
		}
		SAFE_RELEASE(pVectorMemBlockElement);
		mLength--;
	}
	return S_OK;
}

template<class ObjectType>
inline HRESULT VectorMemBlock<ObjectType>::restore()
{
	mLength = 0;
	mElemSize = sizeof(ObjectType*);
	mDeleteContent = true; 
	mIndexLast = 0;

	if (mpMem) { MemoryManager::get()->freeMem(mBlockMemID); };
	MemoryManager::get()->allocateMem(&mpMem, mCapacity * mElemSize, &mBlockMemID);
	return S_OK;
};

template<class ObjectType>
inline VectorMemBlock<ObjectType>::~VectorMemBlock()
{
	if (mDeleteContent) {
		ObjectType* pObject;
		for (UINT i = 0; i < mCapacity; i++) {
			pObject = get(i);
			if (pObject) { SAFE_RELEASE(pObject); };
		}
	}
	if (mpMem) { 
		MemoryManager* pMM = MemoryManager::get();
		if (pMM) {
			pMM->freeMem(mBlockMemID);
		}
	};
};

#endif