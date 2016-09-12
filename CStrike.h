#ifndef CStrike
#define Cstrike

struct CStrike {
	ID _ctrlsGame;
};

typedef struct CStrike CStrike;

CStrike* CStrike_NEW();
BOOL CStrike_Run(TIME);
BOOL CStrike_DELETE(void);
BOOL CStrike_CreateControls(CStrike*);

#endif
