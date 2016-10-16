#ifndef CStrike
#define Cstrike

#include "CoreComponent.h"

class CStrike : public CoreComponent,public Singleton<CStrike>{
	friend class Singleton<CStrike>;
private:
	static CStrike* mpCStrike;
	ID mDefaultControls;
protected:
	CStrike() { restore(); };
	~CStrike() {};
public:
	HRESULT restore();
	HRESULT run(TIME elapsed) { return S_OK; };
};

#endif
