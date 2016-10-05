#ifndef MEMORY_INC
#define MEMORY_INC

template<class ObjectType> class SimplyManaged : public MemManaged 
{
public:
	ObjectType mX;
	SimplyManaged<ObjectType>::SimplyManaged(ObjectType x) : mX(x) {}; 
};

template<class ObjectType> class SpecialList : public List<ObjectType>{
public:
	inline void* SpecialList<ObjectType>::operator new(size_t size); //reoverride for not registering in own list
	SpecialList<ObjectType>::SpecialList(bool deleteContent) : List<ObjectType>(deleteContent) {};
};

class MemManaged {
private:
	ID mMemID;
public:
	ID getMemID() {return mMemID;};
	inline void* MemManaged::operator new(size_t size);
	inline void MemManaged::operator delete(void* pInst);
};

class MemoryManager{
private:
	SpecialList<void*>* mpList;
	MemoryManager() { mpList = new SpecialList<void*>(true); };
	~MemoryManager() { delete mpList; };
public:
	HRESULT allocateMem(void**,size_t size, ID* pMemID);
	HRESULT freeMem(void* pMem) {get()->mpList->deleteByID(((MemManaged*)pMem)->getMemID());return S_OK;};
	static MemoryManager* get();
};

template<class ObjectType>
inline void * SpecialList<ObjectType>::operator new(size_t size)
{
	void* pMem = malloc(size);
	ZeroMemory(pMem, size);
	return pMem;
}

#endif
