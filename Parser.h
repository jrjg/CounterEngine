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

typedef HRESULT(*HandlerFunction)(void* pCurrentObject,String* pObjectName);
typedef HandlerFunction HandlerOnCompleteFunction;

struct Handler {
	String* pObjectName;
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
	FileContent* pFileContent;
	List* pObjects;
	List* pHandlers;
	List* pSingleCharDividers;
	Handler* pCurrentHandler;
}; 
typedef struct Parser Parser;

Parser* Parser_New();
HRESULT Parser_LoadContentFromFile(Parser* pParser, String* pFileName);
HRESULT Parser_RegisterHandler(Parser* pParser, String* pObjectName, HandlerFunction pHandlerFunction, HandlerOnCompleteFunction pHandlerOnCompleteFunction);
HRESULT Parser_RegisterSingleCharHandler(Parser* pParser, char*, HandlerFunction pHandlerFunction);
HRESULT Parser_Parse(Parser* pParser);
HRESULT Parser_CompleteCurrentObject(Parser* pParser);

#endif