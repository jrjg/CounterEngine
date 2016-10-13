#ifndef MEMMANAGED_INC
#define MEMMANAGED_INC

class MemManaged {
public:
	ID mMemID;
	ID getMemID() { return mMemID; };
	inline void* MemManaged::operator new(size_t size);
	inline void MemManaged::operator delete(void* pInst);
};

#endif