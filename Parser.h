#ifndef PARSER_INC
#define PARSER_INC
typedef struct Parser Parser;
typedef HRESULT(*CMDCB)(void*);

struct CMDHandler {
	char* command;
	CMDCB pCB;
};

struct Parser {
	char* pContent;
	void* pCurrentObject;
	List* pObjectsList;
	List* pCMDCallbacks;
};


Parser* Parser_New();
HRESULT Parser_LoadContentFromFile(Parser* pParser, char* filename); 
HRESULT Parser_RegisterCommand(Parser* pParser, char* command, CMDCB pCB);
HRESULT Parser_Parse(Parser* pParser);

#endif