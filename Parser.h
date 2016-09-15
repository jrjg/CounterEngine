#ifndef PARSER_INC
#define PARSER_INC

//	use Parser like this:
//	1. create Parser by calling Parser_New
//	2. register each Handler by calling Parser_RegisterHandler
//	3. register each SingleCharHandler (Handlers for single chars, eg. ";")
//	4. make sure that one Handler is calling Parser_CompleteCurrentObject-function
//	5. load content, eg. from file by calling Parser_LoadContentFromFile
//	6. call Parser_Parse to start parsing

//	note that the Handler-function will be responsible to create the object when the objectname is submitted

#define EVAL_FLOAT_ATTRIBUTE(NAME,VAR) if (CHAREQS(pObjectName->buffer, NAME, strlen(NAME))) {VAR = strtod(pObjectName->buffer + strlen(NAME), NULL);return S_OK;}
#define EVAL_BOOL_ATTRIBUTE(NAME,VAR) if (CHAREQS(pObjectName->buffer, NAME, strlen(NAME))) {(pObjectName->buffer[strlen(NAME)]=='f') ? VAR = FALSE: VAR = TRUE;}
#define EVAL_CHAR_ATTRIBUTE(NAME,VAR) if (CHAREQS(pObjectName->buffer, NAME, strlen(NAME))) {int stringlength = pObjectName->length - strlen(NAME);  VAR = (char*)malloc(stringlength); for (int i = 0; i < stringlength; i++) {VAR[i] = pObjectName->buffer[strlen(NAME)+i];}}
#define EVAL_UNSIGNEDINT_ATTRIBUTE(NAME,VAR) if (CHAREQS(pObjectName->buffer, NAME, strlen(NAME))) {VAR = (unsigned int)atoi(pObjectName->buffer);}
#define EVAL_LPCWSTR_ATTRIBUTE(NAME,VAR) if (CHAREQS(pObjectName->buffer, NAME, strlen(NAME))) {int stringlength = pObjectName->length - strlen(NAME);VAR=(wchar_t*)malloc(stringlength);MultiByteToWideChar(CP_ACP, 0, pObjectName->buffer, stringlength, VAR, stringlength);}


typedef HRESULT(*HandlerFunction)(void** ppCurrentObject,struct String* pObjectName);
typedef HandlerFunction HandlerOnCompleteFunction;

struct Handler {
	struct String* pObjectName;
	HandlerFunction pHandlerFunction;
	HandlerOnCompleteFunction pHandlerOnCompleteFunction;
}; 
typedef struct Handler Handler;

struct Object {
	void* inst;
	Handler* pHandler;
}; 
typedef struct Object Object;

struct Parser {
	struct FileContent* pFileContent;
	struct List* pObjects;
	struct List* pHandlers;
	struct List* pSingleCharDividers;
	Handler* pCurrentHandler;
}; 
typedef struct Parser Parser;

Parser* Parser_New();
HRESULT Parser_LoadContentFromFile(Parser* pParser, String* pFileName);
HRESULT Parser_RegisterHandler(Parser* pParser, String* pObjectName, HandlerFunction pHandlerFunction, HandlerOnCompleteFunction pHandlerOnCompleteFunction);
HRESULT Parser_RegisterSingleCharHandler(Parser* pParser, char, HandlerFunction pHandlerFunction);
HRESULT Parser_Parse(Parser* pParser);
HRESULT Parser_CompleteCurrentObject(Parser* pParser);
HRESULT Parser_Destroy(Parser* pParser);
HRESULT Parser_CollapseString(String* pObjectName, int index);
HRESULT Parser_CleanString(String* pObjectName);

#endif