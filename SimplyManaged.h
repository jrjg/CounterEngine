#ifndef SIMPLYMANAGED
#define SIMPLYMANAGED

#include "MemManaged.h"

template<class ObjectType> class SimplyManaged : public MemManaged {
private:
	ObjectType* mpObject;
protected:
	virtual ~SimplyManaged() { delete mpObject; };
public:
	SimplyManaged(ObjectType &o) { mpObject = new ObjectType(o); };
	ObjectType* getObject() { return mpObject; };
};

#endif