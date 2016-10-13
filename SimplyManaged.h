#ifndef SIMPLYMANAGED
#define SIMPLYMANAGED

template<class ObjectType> class SimplyManaged : public ObjectType, public MemManaged {
public:
	SimplyManaged(ObjectType &o) { *this = o; };
};

#endif