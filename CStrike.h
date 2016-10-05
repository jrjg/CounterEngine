#ifndef CStrike
#define Cstrike

class CStrike : public MemManaged {
private:
	ID mControlsDefault;
	CStrike();
	~CStrike() {};
public:
	static CStrike* get();
	static HRESULT run(TIME elapsed) {};
	static HRESULT release(void* p0) {};
	static HRESULT restore(void* p0);
};

#endif
