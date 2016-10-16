#ifndef MEMMANAGED_INC
#define MEMMANAGED_INC

class MemManaged {
	friend class MemoryManager;
private:
	ID mMemID;
public:
	void* MemManaged::operator new(size_t size);
	void MemManaged::operator delete(void* pInst);
	virtual ~MemManaged() {};
	virtual void Release() { delete this; };
	void setMemID(ID id) { mMemID = id; };
	ID getMemID() { return mMemID; };
};

#endif