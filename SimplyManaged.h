#ifndef SIMPLYMANAGED
#define SIMPLYMANAGED

#include "MemManaged.h"

template<class ObjectType> class SimplyManaged : public MemManaged {
private:
	ObjectType mObject;
public:
	virtual ~SimplyManaged() {};
	SimplyManaged(ObjectType &o) { mObject = o; };
	ObjectType* getObject() { return &mObject; };
};

#endif