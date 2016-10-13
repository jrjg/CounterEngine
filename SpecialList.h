#ifndef SPECIALLIST_INC
#define SPECIALLIST_INC

#include "List.h"

template<class ObjectType> 
class SpecialList : public List<ObjectType> {
public:
	inline void* SpecialList<ObjectType>::operator new(size_t size) override; //reoverride for not registering in own list
	SpecialList<ObjectType>::SpecialList(bool deleteContent) : List<ObjectType>(deleteContent) {};
};

template<class ObjectType>
inline void * SpecialList<ObjectType>::operator new(size_t size)
{
	void* pMem = malloc(size);
	ZeroMemory(pMem, size);
	return pMem;
};

#endif 