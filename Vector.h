#ifndef INCLUDE_VECTOR
#define INCLUDE_VECTOR

#include "MemManaged.h"
#include "VectorMemBlock.h"
template<class ObjectType>class VectorMemBlock;

template<class ObjectType> class Vector : public MemManaged
{
private:
	UINT mMemBlockCapacity;
	UINT mMaxCapacity;
	UINT mIndexLastMemBlock;
	VectorMemBlock<ObjectType>* mpMemBlocks[MAX_VECTOR_BLOCKS];
	UINT length;
protected:
	virtual ~Vector();
public:
	UINT getIndexLast();
	ObjectType* get(UINT index);
	HRESULT set(UINT index, ObjectType *pObj);
	HRESULT pushback(ObjectType * pObj, UINT* pIndex);
	HRESULT restore();
	Vector(UINT memBlockCapacity) : mMemBlockCapacity(memBlockCapacity) { restore(); };
};

template<class ObjectType>
inline Vector<ObjectType>::~Vector() { 
	for (int i = 0; i < MAX_VECTOR_BLOCKS; i++) {
		if (mpMemBlocks[i]) {
			SAFE_RELEASE(mpMemBlocks[i]);
		} 
	} 
};

template<class ObjectType>
inline UINT Vector<ObjectType>::getIndexLast()
{
	return (mpMemBlocks[mIndexLastMemBlock]->getIndexLast() + mIndexLastMemBlock*mMemBlockCapacity-1);
}

template<class ObjectType>
inline ObjectType * Vector<ObjectType>::get(UINT index)
{
	if (index > mMaxCapacity) { return NULL; };
	return mpMemBlocks[index / mMemBlockCapacity]->get(index % mMemBlockCapacity);
};

template<class ObjectType>
inline HRESULT Vector<ObjectType>::set(UINT index, ObjectType *pObj)
{
	if (index > mMaxCapacity) { return ERROR_SUCCESS; };
	UINT indexMemBlock = index / mMemBlockCapacity;
	VectorMemBlock<ObjectType>* pMemBlock = mpMemBlocks[indexMemBlock];
	if (!pMemBlock) {
		pMemBlock = new VectorMemBlock<ObjectType>(mMemBlockCapacity);
		mpMemBlocks[indexMemBlock] = pMemBlock;
	}
	HRESULT hr = pMemBlock->set(index % mMemBlockCapacity, pObj);
	if (SUCCEEDED(hr)) {
		if (indexMemBlock > mIndexLastMemBlock) {
			mIndexLastMemBlock = indexMemBlock;
		}
	}
	return hr;
};

template<class ObjectType>
inline HRESULT Vector<ObjectType>::pushback(ObjectType * pObj,UINT* pIndex)
{
	HRESULT hr = ERROR_SUCCESS;
	UINT indexMemBlock;
	for (UINT i = 0; i < MAX_VECTOR_BLOCKS; i++) {
		if (mpMemBlocks[i]) {
			if (SUCCEEDED(mpMemBlocks[i]->pushback(pObj, pIndex))) {
				indexMemBlock = i;
				hr = S_OK;
				break;
			}
		}
	}
	for (UINT i = 0; i < MAX_VECTOR_BLOCKS; i++) {
		if (!mpMemBlocks[i]) {
			mpMemBlocks[i] = new VectorMemBlock<ObjectType>(mMemBlockCapacity);
			if (SUCCEEDED(mpMemBlocks[i]->pushback(pObj, pIndex))) {
				indexMemBlock = i;
				hr = S_OK;
				break;
			}
		}
	}
	if (SUCCEEDED(hr)) {
		if (indexMemBlock > mIndexLastMemBlock) {
			mIndexLastMemBlock = indexMemBlock;
		}
	}
	return hr;
};

template<class ObjectType>
inline HRESULT Vector<ObjectType>::restore()
{
	for (int i = 0; i < MAX_VECTOR_BLOCKS; i++) {
		if (mpMemBlocks[i]) {
			mpMemBlocks[i]->restore();
		}
	}
	mIndexLastMemBlock = 0;
	mMaxCapacity = MAX_VECTOR_BLOCKS*mMemBlockCapacity;
	if (!mpMemBlocks[0]) { mpMemBlocks[0] = new VectorMemBlock<ObjectType>(mMemBlockCapacity); };
	return S_OK;
};

#endif // !INCLUDE_VECTOR