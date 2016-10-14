#ifndef SIMPLYMANAGED
#define SIMPLYMANAGED

#include "MemManaged.h"

template<class ObjectType> class SimplyManaged : public MemManaged {
private:
	ObjectType* pObject;
protected:
	virtual ~SimplyManaged() { delete pObject; };
public:
	SimplyManaged(ObjectType &o) { pObject = new ObjectType(o); };
};

#endif