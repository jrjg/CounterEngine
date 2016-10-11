#ifndef INCLUDE_VECTOR
#define INCLUDE_VECTOR

template<class ObjectType> class VectorMemBlock : public MemManaged {
private:
	UINT mIndexLast;
	MemManaged* mpMem;
	bool mDeleteContent;
	UINT mElemSize;
	UINT mCapacity;
	size_t mSizeOfMemManaged;
public:
	ObjectType* VectorMemBlock<ObjectType>::get(UINT index);
	UINT getIndexLast() { return mIndexLast; };
	void setDeleteContent(bool b) { mDeleteContent = b; };
	HRESULT set(UINT index,ObjectType* pObj);
	HRESULT pushback(ObjectType* pObj, UINT* pIndex);
	VectorMemBlock(UINT capacity);
	~VectorMemBlock();
};

template<class ObjectType>
inline ObjectType * VectorMemBlock<ObjectType>::get(UINT index)
{
	if (index > mCapacity) { return NULL; };
	return(*(ObjectType**)((char*)(mpMem) + mSizeOfMemManaged + index * mElemSize));
}

template<class ObjectType>
inline HRESULT VectorMemBlock<ObjectType>::set(UINT index, ObjectType * pObj)
{
	if (index > mCapacity) { return ERROR_SUCCESS; };
	*(ObjectType**)((char*)(mpMem) + mSizeOfMemManaged + index * mElemSize) = pObj;
	if (index > mIndexLast) {
		mIndexLast = index;
	}
	return S_OK;
}

template<class ObjectType>
inline HRESULT VectorMemBlock<ObjectType>::pushback(ObjectType * pObj, UINT * pIndex)
{
	if (SUCCEEDED(set(mIndexLast + 1, pObj))) {
		*pIndex = ++mIndexLast;
		return S_OK;
	}
	return ERROR_SUCCESS;
}

template<class ObjectType>
VectorMemBlock<ObjectType>::VectorMemBlock(UINT capacity) : mSizeOfMemManaged(sizeof(MemManaged)), mElemSize(sizeof(ObjectType)), mCapacity(capacity), mDeleteContent(true), mIndexLast(-1){
	MemoryManager::get()->allocateMem(&mpMem,mCapacity * mElemSize + mSizeOfMemManaged,mpMem->mMemID);
}
template<class ObjectType>
inline VectorMemBlock<ObjectType>::~VectorMemBlock()
{
	if (mDeleteContent) {
		for (UINT i = 0; i < mCapacity; i++) {
			delete get(i);
		}
	}
	delete mpMem;
};

template<class ObjectType> class Vector : public MemManaged
{
private:
	UINT mMemBlockCapacity;
	UINT mMaxCapacity;
	VectorMemBlock<ObjectType>* mpMemBlocks[MAX_VECTOR_BLOCKS];
public:
	ObjectType* get(UINT index);
	HRESULT set(UINT index, ObjectType *pObj);
	HRESULT pushback(ObjectType * pObj, UINT* pIndex);
	HRESULT restore();
	Vector(UINT memBlockCapacity) : mMemBlockCapacity(memBlockCapacity) { restore(); };
	~Vector() { for (int i = 0; i < MAX_VECTOR_BLOCKS; i++) { if (mpMemBlocks[i]) { delete mpMemBlocks[i]; } } };
};

template<class ObjectType>
inline ObjectType * Vector<ObjectType>::get(UINT index)
{
	if (index > mMaxCapacity) { return NULL; };
	return mpMemBlocks[index / mMemBlockCapacity]->get(index % mMemBlockCapacity);
}

template<class ObjectType>
inline HRESULT Vector<ObjectType>::set(UINT index, ObjectType *pObj)
{
	if (index > mMaxCapacity) { return ERROR_SUCCESS; };
	VectorMemBlock<ObjectType>* pMemBlock = mpMemBlocks[index / mMemBlockCapacity]);
	if (!pMemBlock) {
		pMemBlock = new VectorMemBlock<ObjectType>(mMemBlockCapacity);
		mpMemBlocks[index / mMemBlockCapacity] = pMemBlock;
	}
	return pMemBlock->set(index % mMemBlockCapacity, pObj);
}

template<class ObjectType>
inline HRESULT Vector<ObjectType>::pushback(ObjectType * pObj,UINT* pIndex)
{
	VectorMemBlock<ObjectType>* pMemBlock;
	for (UINT i = 0; i < MAX_VECTOR_BLOCKS; i++) {
		pMemBlock = mpMemBlocks[i];
		if (pMemBlock) {
			if (SUCCEEDED(pMemBlock->pushback(pObj, pIndex))) {
				return S_OK;
			}
		}
	}
	for (UINT i = 0; i < MAX_VECTOR_BLOCKS; i++) {
		pMemBlock = mpMemBlocks[i];
		if (!pMemBlock) {
			pMemBlock = new VectorMemBlock<ObjectType>(mMemBlockCapacity);
			mpMemBlocks[i] = pMemBlock;
			if (SUCCEEDED(pMemBlock->pushback(pObj, pIndex))) {
				return S_OK;
			}
		}
	}
	return ERROR_SUCCESS;
}

template<class ObjectType>
inline HRESULT Vector<ObjectType>::restore()
{
	for (int i = 0; i < MAX_VECTOR_BLOCKS; i++) {
		delete mpMemBlocks[i]; 
	}
	mMaxCapacity(MAX_VECTOR_BLOCKS*memBlockCapacity);
	mpMemBlocks[0] = new VectorMemBlock<ObjectType>(memBlockCapacity);
	return S_OK;
}

#endif // !INCLUDE_VECTOR