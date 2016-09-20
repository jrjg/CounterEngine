#ifndef PARSER_INC
#define PARSER_INC

typedef HRESULT(*ObjectHandlerFunction)(void* pParentObject, struct String* pChildObjectName, void* pChildObject);
typedef HRESULT(*ConvertFromStringToTypeFunction)(void* pObject,String* pString);
typedef enum{ignore,submit}OperatorCode;

struct Type {
	struct String* pTypeName;
	size_t size;
	ConvertFromStringToTypeFunction pConvertFromStringToTypeFunction;
};
typedef struct Type Type;

struct Variable {
	struct String* pVariableName;
	struct String* pTypeName;
	ObjectHandlerFunction pObjectHandlerFunction;
};
typedef struct Variable Variable;

struct Object {
	void* pInst;
	Type* pType;
	Variable* pVariable;
}; 
typedef struct Object Object;

struct Operator {
	String* pString;
	OperatorCode code;
};
typedef struct Operator Operator;

struct Parser {
	struct List* pVariables;
	struct List* pObjects;
	struct List* pTypes;
	struct List* pOperators;
	ObjectHandlerFunction pRootHandler;
}; 
typedef struct Parser Parser;

Parser* Parser_New();
HRESULT Parser_Destroy(Parser* pParser);
HRESULT Parser_SubmitObject(Parser* pParser);
HRESULT Parser_DeclareType(Parser* pParser, char* pTypeName, size_t size, ConvertFromStringToTypeFunction pConvertFromStringToTypeFunction);
HRESULT Parser_DeclareVariable(Parser* pParser, char* pTypeName, char* pVariableName, ObjectHandlerFunction pObjectHandlerFunction);
HRESULT Parser_RegisterOperator(Parser* pParser, char*, OperatorCode code);
HRESULT Parser_ParseFile(Parser* pParser, char* pFileName, ObjectHandlerFunction pRootHandler);

HRESULT Parser_ConvertFromStringToFloat(void* pObject, String* pString);
HRESULT Parser_ConvertFromStringToBool(void* pObject, String* pString);
HRESULT Parser_ConvertFromStringToString(void* pObject, String* pString);
HRESULT Parser_ConvertFromStringToUINT(void* pObject, String* pString);
HRESULT Parser_ConvertFromStringToLPCWSTR(void* pObject, String* pString);


#endif