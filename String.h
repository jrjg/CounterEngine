#ifndef STRING_INC
#define STRING_INC

#include "MemManaged.h"

template<class ObjectType> 
class String : public MemManaged {
private:
	MemManaged* mpBuffer;
	size_t mLength;
protected:
	virtual ~String();
public:
	String(ObjectType* pChars, size_t length);
	String(size_t length);
	inline bool String::operator==(const String<ObjectType> &s2);
	/*inline bool String::operator==(const ObjectType* pC) {
		if ((mLength != strlen(pC)) && (mLength != wcslen(pC))) {
			return false;
		}
		else {
			for (UINT i = 0; i < mLength; i++) {
				if (mpBuffer[i] != pC[i]) {
					return false;
				}
			}
		}
		return true;
	};*/
	inline ObjectType* getBuffer() {return mpBuffer;};
	inline size_t getLength() { return mLength; };
};

template<class ObjectType>
bool String<ObjectType>::operator==(const String<ObjectType> &s2)
{
	if (mLength != s2.mLength) {
		return false;
	}
	else {
		for (UINT i = 0; i < s2.mLength; i++) {
			if (mpBuffer[i] != s2.mpBuffer[i]) {
				return false;
			}
		}
	}
	return true;
};

template<class ObjectType>
inline String<ObjectType>::String(ObjectType * pChars, size_t length) : mLength(length) {
	MemoryManager::get()->allocateMem(&mpBuffer, mLength * sizeof(ObjectType), mpBuffer->mMemID);
	for (int i = 0; i<mLength; i++) {
		mpBuffer[i] = pChars[i];
	};
};

template<class ObjectType>
inline String<ObjectType>::String(size_t length) : mLength(length) {
	MemoryManager::get()->allocateMem(&mpBuffer, mLength * sizeof(ObjectType), mpBuffer->mMemID);
}
template<class ObjectType>
inline String<ObjectType>::~String() { mpBuffer->release(); };

#endif 
