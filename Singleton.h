#ifndef SINGLETON_INC
#define SINGLETON_INC

#include "TopInclude.h"
#include "EventListener.h"
#include "SimplyManaged.h"
#include "MemManaged.h"

template<class T>
class AllowInstancingListener;

template<class T>
class Singleton{
	friend class AllowInstancingListener<T>;
	friend class MemoryManager;
protected:
	AllowInstancingListener<T>* mpAllowInstancingListener;
	static bool mAllowInstancing;
	static T* mpInstance;
	Singleton() { mpAllowInstancingListener = new AllowInstancingListener<T>(this); };
	Singleton(bool autoRegister);
	virtual ~Singleton() { 
		if (mpAllowInstancingListener) { SAFE_RELEASE(mpAllowInstancingListener); };
	};
public:
	static T* get();
};

template<class T>
T* Singleton<T>::mpInstance = NULL;

template<class T>
bool Singleton<T>::mAllowInstancing = true;

template<class T>
inline Singleton<T>::Singleton(bool autoRegister) { 
	if (autoRegister) { 
		mpAllowInstancingListener = new AllowInstancingListener<T>(this); 
	}
	else {
		mpAllowInstancingListener = 0;
	}
};

template<class T>
inline T * Singleton<T>::get()
{
	if (!mpInstance) {
		if (mAllowInstancing) {
			mpInstance = new T();
		}
		else {
			CE1_ASSERT(0 && "Not allowed to instance Singleton");
		}
	}
	return mpInstance;
}

template<class T>
class AllowInstancingListener : public EventListener {
private:
	Singleton<T>* mpSingleton;
	HRESULT handle(MemManaged* pData) override { mpSingleton->mAllowInstancing = *(((SimplyManaged<bool>*)pData)->getObject()); return S_OK; };
public:
	AllowInstancingListener(Singleton<T>* pSingleton) : EventListener(EVENT_ALLOWINSTANCING), mpSingleton(pSingleton){};
};

#endif