#ifndef STRING_INC
#define STRING_INC


template<class ObjectType> 
class String : public MemManaged {
private:
	MemManaged* mpBuffer;
	size_t mLength;
public:
	String<ObjectType>::String(ObjectType* pChars, size_t length) : mLength(length) {MemoryManager::get()->allocateMem(&mpBuffer, mLength * sizeof(ObjectType), mpBuffer->mMemID);for (int i = 0; i<mLength; i++) { mpBuffer[i] = pChars[i]; };};
	String<ObjectType>::String(size_t length) : mLength(length) { MemoryManager::get()->allocateMem(&mpBuffer, mLength * sizeof(ObjectType), mpBuffer->mMemID); };
	String<ObjectType>::~String() { delete mpBuffer; };
	bool String<ObjectType>::String::operator==(const String<ObjectType> &s2);
	bool String<ObjectType>::String::operator==(const ObjectType* pC);
	inline ObjectType* String<ObjectType>::getBuffer() {return mpBuffer;};
	inline size_t String<ObjectType>::getLength() { return mLength; };
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
}

template<class ObjectType>
bool String<ObjectType>::operator==(const ObjectType* pC)
{
	size_t pCLength = 0;
	(sizeof(ObjectType) == sizeof(wchar_t)) ? pCLength = wcslen(pC) : pCLength = strlen(pC);
	if (mLength != pCLength) {
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
}

#endif 