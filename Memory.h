#ifndef MEMORY_INC
#define MEMORY_INC

#include "UnManagedList.h"
#include "MemManaged.h"

class MemoryManager{
private:
	UnManagedList<MemManaged>* mpList;
	MemoryManager();
protected:
	virtual ~MemoryManager();
public:
	static MemoryManager* get();
	HRESULT allocateMem(MemManaged**,size_t size);
	HRESULT freeMem(MemManaged* pMem);
	void manualRelease() { delete this; }
};

#endif
