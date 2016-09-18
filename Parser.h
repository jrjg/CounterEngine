#ifndef PARSER_INC
#define PARSER_INC

// how to use parser
// 1. create parser by calling Parser_New
// 2. register types by calling Parser_registerType
// 3. register single character operators by calling Parser_RegisterSingleCharOperator
//		-> dont forget to define ignore and submit
// 4. start parsing by calling Parser_ParseFromFile


typedef HRESULT(*VariableHandlerFunction)(void* pParentObject, struct String* pChildObjectName, void* pChildObject);
typedef enum{ignore,submit}SingeCharOperatorCode;

struct Variable {
	struct String* pVariableName;
	size_t size;
	VariableHandlerFunction pVariableHandlerFunction;
};
typedef struct Variable Variable;

struct Object {
	void* pInst;
	Type* pType;
}; 
typedef struct Object Object;

struct SingleCharOperator {
	char* pChar;
	SingeCharOperatorCode code;
};
typedef struct SingleCharOperator SingleCharOperator;

struct Parser {
	struct List* pObjects;
	struct List* pVariables;
	struct List* pSingleCharOperators;
	VariableHandlerFunction pRootHandler;
}; 
typedef struct Parser Parser;

Parser* Parser_New();
HRESULT Parser_Destroy(Parser* pParser);
HRESULT Parser_CompleteCurrentObject(Parser* pParser);
HRESULT Parser_RegisterVariable(Parser* pParser, String* pVariableName, String* pTypeName, size_t size, VariableHandlerFunction pVariableHandlerFunction);
HRESULT Parser_RegisterSingleCharOperator(Parser* pParser, char, SingeCharOperatorCode code);
HRESULT Parser_ParseFromFile(Parser* pParser, String* pFileName, VariableHandlerFunction pRootHandler);

//HRESULT Parser_CollapseString(String* pObjectName, int index);
//HRESULT Parser_CleanString(String* pObjectName);


#endif