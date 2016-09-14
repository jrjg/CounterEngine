#ifndef CStrike
#define Cstrike

struct CStrike {
	ID _ctrlsGame;
};

typedef struct CStrike CStrike;

CStrike* CStrike_NEW();
HRESULT CStrike_Run(TIME);
HRESULT CStrike_DELETE(void);
HRESULT CStrike_CreateControls(CStrike*);

#endif
