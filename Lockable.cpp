#include "TopInclude.h"

#include "Lockable.h"

HRESULT Lockable::access()
{
	if (mLocked) {
		return false;
	}
	return true;
}

HRESULT Lockable::lock(ID * pID) {
	if (mLocked) {
		return ERROR_SUCCESS;
	}
	mLocked = true;
	mCurrentKey = rand() % ID_MAX;
	*pID = mCurrentKey;
	return S_OK;
}

HRESULT Lockable::unlock(ID id)
{
	if (!mLocked) { return S_OK; }
	if (mCurrentKey == id) {
		mLocked = false;
		mCurrentKey = 0;
		return S_OK;
	}
	return ERROR_SUCCESS;
}
