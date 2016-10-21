#ifndef INCLUDE_GLOBALS
#define INCLUDE_GLOBALS

#include <stdlib.h>

typedef time_t TIME;
typedef unsigned long long ID;	
typedef int KEYCODE;
#define _DBG TRUE	

#define STR(STMNT) #STMNT
#define SAFE_RELEASE( x ) if( x ){ x->Release( ); x = NULL; }

#define MAXIMUM_LIGHTS_SUPPORTED 8
#define fOPAQUE (1.0f)
#define cd3d11_CONSTANTBUFFER_MATRICES 0
#define cd3d11_CONSTANTBUFFER_MATERIAL 0
#define cd3d11_CONSTANTBUFFER_LIGHTING 0

#define MAX_VECTOR_BLOCKS 10

#define CORETICKSPEED 10

#define CE1_ASSERT(expr) if(!(expr) && _DBG){MessageBox(NULL, L#expr, L"Assertion failed",MB_ICONEXCLAMATION | MB_OK);}

#define V_RETURN(expr) if(FAILED(expr)){return ERROR_SUCCESS;}

typedef enum { EVENT_NULL, EVENT_CREATEWINDOW, EVENT_GAMEINIT, EVENT_WINDOWCREATED, EVENT_ALLOWINSTANCING, EVENT_WINPARAMS, EVENT_SETCONTROLSET, EVENT_UPDATE_WINPARAMS, EVENT_KEYDOWN, EVENT_KEYUP, EVENT_MOVEBACK, EVENT_MOVELEFT, EVENT_MOVERIGHT, EVENT_MOVEFORWARD, EVENT_RESTORE, EVENT_RELEASE, EVENT_MOVEUP, EVENT_MOVEDOWN} EVENTTYPE;
typedef enum {RenderPass_0, RenderPass_Static = RenderPass_0, RenderPass_Actor, RenderPass_Sky, RenderPass_NotRendered, RenderPass_Last} RenderPass;
typedef enum {AlphaOpague,AlphaTexture,AlphaMaterial,AlphaVertex} AlphaType;

#endif 