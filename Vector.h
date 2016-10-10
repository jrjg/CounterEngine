#ifndef INCLUDE_VECTOR
#define INCLUDE_VECTOR


template<class ObjectType> class Vector : public MemManaged
{
private:
	static ID gmIDCounter;
	MemManaged* mpMem;
	UINT mCapacity;
	UINT mSlotOfLast;
	UINT mElemSize;
	UINT mElemCount;
	ID mID;
	HRESULT resize(Vector*, UINT);
public:
	Vector(UINT stepsize) : mElemSize(sizeof(ObjectType)), mCapacity(0), mSlotOfLast(0), mpMem(NULL), mElemCount(0), mID(gmIDCounter++) { resize(mElemCount); };
	~Vector();
	HRESULT insert(Vector*, UINT, void*);
	UINT getElemCount() { return mElemCount; };
	void* Vector::operator[](UINT index);
	UINT getCapacity() { return mCapacity; };
	UINT getSlotOfLast() { return mSlotOfLast; };
	UINT pushback(void*);
	HRESULT getMem() { return mpMem; };
	HRESULT erase(UINT index, bool deletecontent);
};

template<class ObjectType>
inline void * Vector<ObjectType>::operator[](UINT index)
{
	return NULL;
}

HRESULT Vector_Erase(Vector * pVector, UINT index, bool deletecontent)
{
	CE1_ASSERT(pVector&&"Vector_Erase");
	if (pVector->_elems > 0) {
		CE1_CALL(Vector_DeleteElement(pVector, index, deletecontent));
		CE1_CALL(Vector_Insert(pVector, index, Vector_Get(pVector, Vector_Last(pVector))));
		CE1_CALL(Vector_DeleteElement(pVector, Vector_Last(pVector), false));
	}
	return S_OK;
}

HRESULT Vector_Resize(Vector* pVector, unsigned int elemcnt)
{
	CE1_ASSERT(pVector&&elemcnt);
	void* pMem = malloc(elemcnt * pVector->_elemsize);
	memset(pMem, 0, elemcnt * pVector->_elemsize);
	if (pVector->_elems>0)
	{
		memcpy(pMem, pVector->_pMem, pVector->_capacity * pVector->_elemsize);
	}
	CE1_DEL(pVector->_pMem);
	pVector->_pMem = pMem;
	pVector->_capacity = elemcnt;
	return S_OK;
}

HRESULT Vector_Insert(Vector* pVector, unsigned int index, void* pData)
{
	CE1_ASSERT(pVector&&pData);
	if (index > pVector->_capacity)
	{
		CE1_CALL(Vector_Resize(pVector, index * 2));
	}
	memcpy((char*)(pVector->_pMem) + index * pVector->_elemsize, &pData, pVector->_elemsize);
	if (index > pVector->_last)
	{
		pVector->_last = index;
	}
	pVector->_elems++;
	return S_OK;
}

unsigned int Vector_Pushback(Vector* pVector, void* pData)
{
	CE1_ASSERT(pVector&&pData);
	unsigned int index = Vector_Last(pVector);
	index++;
	if (index > pVector->_capacity)
	{
		CE1_CALL(Vector_Resize(pVector, index * 2));
	}
	memcpy((char*)(pVector->_pMem) + index * pVector->_elemsize, &pData, pVector->_elemsize);
	if (index > pVector->_last)
	{
		pVector->_last = index;
	}
	pVector->_elems++;
	return index;
}

unsigned int Vector_Elements(Vector* pVector)
{
	CE1_ASSERT(pVector);
	return pVector->_elems;
}

void* Vector_Get(Vector* pVector, unsigned int index)
{
	CE1_ASSERT(pVector);
	if (index <= pVector->_capacity)
	{
		return(*(void**)((char*)(pVector->_pMem) + index * pVector->_elemsize));
	}
	return NULL;
}

#endif // !INCLUDE_VECTOR