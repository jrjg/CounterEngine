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
	VectorMemBlock<ObjectType>* mpMemBlocks[MAX_VECTOR_BLOCKS];
protected:
	virtual ~Vector();
public:
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
inline ObjectType * Vector<ObjectType>::get(UINT index)
{
	if (index > mMaxCapacity) { return NULL; };
	return mpMemBlocks[index / mMemBlockCapacity]->get(index % mMemBlockCapacity);
};

template<class ObjectType>
inline HRESULT Vector<ObjectType>::set(UINT index, ObjectType *pObj)
{
	if (index > mMaxCapacity) { return ERROR_SUCCESS; };
	VectorMemBlock<ObjectType>* pMemBlock = mpMemBlocks[index / mMemBlockCapacity];
	if (!pMemBlock) {
		pMemBlock = new VectorMemBlock<ObjectType>(mMemBlockCapacity);
		mpMemBlocks[index / mMemBlockCapacity] = pMemBlock;
	}
	return pMemBlock->set(index % mMemBlockCapacity, pObj);
};

template<class ObjectType>
inline HRESULT Vector<ObjectType>::pushback(ObjectType * pObj,UINT* pIndex)
{
	for (UINT i = 0; i < MAX_VECTOR_BLOCKS; i++) {
		if (mpMemBlocks[i]) {
			if (SUCCEEDED(mpMemBlocks[i]->pushback(pObj, pIndex))) {
				return S_OK;
			}
		}
	}
	for (UINT i = 0; i < MAX_VECTOR_BLOCKS; i++) {
		if (!mpMemBlocks[i]) {
			mpMemBlocks[i] = new VectorMemBlock<ObjectType>(mMemBlockCapacity);
			if (SUCCEEDED(mpMemBlocks[i]->pushback(pObj, pIndex))) {
				return S_OK;
			}
		}
	}
	return ERROR_SUCCESS;
};

template<class ObjectType>
inline HRESULT Vector<ObjectType>::restore()
{
	for (int i = 0; i < MAX_VECTOR_BLOCKS; i++) {
		if (mpMemBlocks[i]) {
			mpMemBlocks[i]->restore();
		}
	}
	mMaxCapacity = MAX_VECTOR_BLOCKS*mMemBlockCapacity;
	if (!mpMemBlocks[0]) { mpMemBlocks[0] = new VectorMemBlock<ObjectType>(mMemBlockCapacity); };
	return S_OK;
};

#endif // !INCLUDE_VECTOR