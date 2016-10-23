#ifndef LOCKABLE_INC
#define LOCKABLE_INC

#include "TopInclude.h"

class Lockable {
private:
	bool mLocked;
	ID mCurrentKey;
public:
	Lockable() :mLocked(false), mCurrentKey(0){};
	virtual ~Lockable() {};
	HRESULT access();
	HRESULT lock(ID* pID);
	HRESULT unlock(ID id);
};


#endif