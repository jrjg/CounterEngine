#ifndef INCLUDE_GLOBALS
#define INCLUDE_GLOBALS

#include <stdlib.h>


typedef unsigned int TIME;									//time
typedef int ID;									//unique identifier
typedef int KEYCODE;								//provisorische variable für tastatur tasten
#define _DBG TRUE										//DEBUG MODE ACTIVATED
#define _NEW(StructName,Pointer) Pointer = (StructName*)malloc(sizeof(StructName)); ZeroMemory(Pointer, sizeof(StructName));			//ALLOCATE MEMORY
#define CE1_NEW(TYPE,pVAR) TYPE* pVAR = (TYPE*)malloc(sizeof(TYPE)); ZeroMemory(pVAR, sizeof(TYPE));			//ALLOCATE MEMORY
#define CE1_DEL(pObject) if(pObject != NULL){free(pObject);}	//FREE MEMORY
#define BACKGROUND_TRANSPARENT 1337							//LEET
#define BACKGROUND_ERASED 1773								//2x LEET kappa

#define STR(STMNT) #STMNT
#define CE1_CALL(HREXPR) if(FAILED(HREXPR)&& _DBG){MessageBox(NULL, L#HREXPR, L"Function call failed",MB_ICONEXCLAMATION | MB_OK);return ERROR_SUCCESS;}
#define SAFE_RELEASE( x ) if( x ){ x->Release( ); x = 0; }

//#define WINDOWTITLE L"Counter Engine"
//#define BUFFERWIDTH 800
//#define BUFFERHEIGHT 600
//#define FULLSCREEN FALSE


#define MAXIMUM_LIGHTS_SUPPORTED 8
#define fOPAQUE (1.0f)
#define cd3d11_CONSTANTBUFFER_MATRICES 0
#define cd3d11_CONSTANTBUFFER_MATERIAL 0
#define cd3d11_CONSTANTBUFFER_LIGHTING 0

#define CE1_STR(pS,pC) pS = (String*)malloc(sizeof(String)); ZeroMemory(pS, sizeof(String));pS->length = strlen(pC); pS->pBuffer = (char*)malloc(strlen(pC)*sizeof(char)); ZeroMemory(pS->pBuffer, strlen(pC)*sizeof(char)); for(int i=0;i<pS->length;i++){pS->pBuffer[i] = pC[i];};
#define CE1_WSTR(pS,pC) pS = (WString*)malloc(sizeof(WString)); ZeroMemory(pS, sizeof(WString));pS->length = wcslen(pC); pS->pBuffer = (wchar_t*)malloc(wcslen(pC)*sizeof(wchar_t)); ZeroMemory(pS->pBuffer, wcslen(pC)*sizeof(wchar_t)); for(int i=0;i<pS->length;i++){pS->pBuffer[i] = pC[i];};
#define CE1_ASSERT(expr) if(!(expr) && _DBG){MessageBox(NULL, L#expr, L"Assertion failed",MB_ICONEXCLAMATION | MB_OK);return ERROR_SUCCESS;}
#define CE1_LISTEXEC(pList,Exec) for(Iterator itr = List_Iterator(pList); itr != NULL; itr = List_Next(itr)){Exec}
#define CE1_VECEXEC(pVector,Exec) for(unsigned int i = 0; i < Vector_Last(pVector); i++){Exec}

typedef enum { EVENT_NULL, EVENT_KEYDOWN, EVENT_KEYUP, EVENT_MOVEBACK, EVENT_MOVEFORWARD, EVENT_CHECKFORFULLROWS, EVENT_FIXBLOCK, EVENT_MOVEFIX, EVENT_START, EVENT_END, EVENT_MOVELEFT, EVENT_MOVERIGHT, EVENT_MOVEUP, EVENT_MOVEDOWN, EVENT_ADDYAW, EVENT_ADDPITCH, EVENT_ROTATECW, EVENT_ROTATECCW, EVENT_SPAWNBLOCK, EVENT_NEWGAME } EVENTTYPE;
typedef enum {RenderPass_0, RenderPass_Static = RenderPass_0, RenderPass_Actor, RenderPass_Sky, RenderPass_NotRendered, RenderPass_Last} RenderPass;
typedef enum {AlphaOpague,AlphaTexture,AlphaMaterial,AlphaVertex} AlphaType;

#endif //!INCLUDE_GLOBALS 