#ifndef MEMORY_INC
#define MEMORY_INC

#include "List.h"
#include "MemManaged.h"
#include "Singleton.h"

class MemoryManager : public Singleton<MemoryManager>{
	friend class Singleton<MemoryManager>;
private:
	UnManagedList<MemManaged>* mpList;
	MemoryManager();
	ID mLatestMemID;
protected:
	~MemoryManager();
public:
	HRESULT allocateMem(MemManaged**,size_t size);
	HRESULT freeMem(MemManaged* pMem);
	HRESULT allocateMem(void** ppMem, size_t size, ID* pID);
	HRESULT freeMem(ID id);
	void manualRelease() { delete this; }
	ID retrieveMyMemID();
	static MemoryManager* get();
};

#endif
