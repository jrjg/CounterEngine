#ifndef MEMORY_INC
#define MEMORY_INC

class MemoryManager{
private:
	static MemoryManager* mpInstance;
	SpecialList<void>* mpList;
	MemoryManager() { mpList = new SpecialList<void>(true); };
	~MemoryManager() { delete mpList; };
public:
	static MemoryManager* get() { if (!mpInstance) { mpInstance = new MemoryManager(); }; return mpInstance; };
	HRESULT release() { delete this; };
	HRESULT allocateMem(void**,size_t size, ID* pMemID);
	HRESULT freeMem(void* pMem) { mpList->deleteByID( ((MemManaged*)pMem)->getMemID() ); return S_OK; };
};

#endif
