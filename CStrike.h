#ifndef CStrike
#define Cstrike

#include "CoreComponent.h"

class CStrike : public CoreComponent {
private:
	ID mDefaultControls;
protected:
	CStrike() { restore(); };
	~CStrike() {};
public:
	static CStrike* get();
	HRESULT restore();
	HRESULT run(TIME elapsed) { return S_OK; };
};

#endif
