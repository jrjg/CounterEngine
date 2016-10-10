#ifndef INCLUDE_GLOBALS
#define INCLUDE_GLOBALS

#include <stdlib.h>

typedef unsigned int TIME;
typedef int ID;	
typedef int KEYCODE;	
#define _DBG TRUE	

#define STR(STMNT) #STMNT
#define SAFE_RELEASE( x ) if( x ){ x->Release( ); x = 0; }

#define MAXIMUM_LIGHTS_SUPPORTED 8
#define fOPAQUE (1.0f)
#define cd3d11_CONSTANTBUFFER_MATRICES 0
#define cd3d11_CONSTANTBUFFER_MATERIAL 0
#define cd3d11_CONSTANTBUFFER_LIGHTING 0

#define MAX_VECTOR_BLOCKS 10

#define CORETICKSPEED 10

#define CE1_STR(pS,pC) pS = (String*)malloc(sizeof(String)); ZeroMemory(pS, sizeof(String));pS->length = strlen(pC); pS->pBuffer = (char*)malloc(strlen(pC)*sizeof(char)); ZeroMemory(pS->pBuffer, strlen(pC)*sizeof(char)); for(int i=0;i<pS->length;i++){pS->pBuffer[i] = pC[i];};
#define CE1_WSTR(pS,pC) pS = (WString*)malloc(sizeof(WString)); ZeroMemory(pS, sizeof(WString));pS->length = wcslen(pC); pS->pBuffer = (wchar_t*)malloc(wcslen(pC)*sizeof(wchar_t)); ZeroMemory(pS->pBuffer, wcslen(pC)*sizeof(wchar_t)); for(int i=0;i<pS->length;i++){pS->pBuffer[i] = pC[i];};
#define CE1_ASSERT(expr) if(!(expr) && _DBG){MessageBox(NULL, L#expr, L"Assertion failed",MB_ICONEXCLAMATION | MB_OK);}
#define CE1_VECEXEC(pVector,Exec) for(unsigned int i = 0; i < Vector_Last(pVector); i++){Exec}

typedef enum { EVENT_NULL, EVENT_UPDATE_WINPARAMS, EVENT_KEYDOWN, EVENT_KEYUP, EVENT_MOVEBACK, EVENT_MOVELEFT, EVENT_MOVERIGHT, EVENT_MOVEFORWARD, EVENT_RESTORE, EVENT_RELEASE, EVENT_MOVEUP, EVENT_MOVEDOWN} EVENTTYPE;
typedef enum {RenderPass_0, RenderPass_Static = RenderPass_0, RenderPass_Actor, RenderPass_Sky, RenderPass_NotRendered, RenderPass_Last} RenderPass;
typedef enum {AlphaOpague,AlphaTexture,AlphaMaterial,AlphaVertex} AlphaType;

#endif 