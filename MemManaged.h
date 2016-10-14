#ifndef MEMMANAGED_INC
#define MEMMANAGED_INC

class MemManaged {
	friend class MemoryManager;
private:
	ID mMemID;
protected:
	virtual ~MemManaged() {};
public:
	void* operator new(size_t size);
	void operator delete(void* pInst);
	virtual void Release() { delete this; };
};

#endif