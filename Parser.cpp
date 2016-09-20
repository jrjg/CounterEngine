#include "TopInclude.h"
#include "FileReader.h"
#include "List.h"
#include "Engine.h"
#include "Parser.h"



Parser * Parser_New()
{
	Parser* pParser;
	_NEW(Parser, pParser);
	pParser->pObjects = List_New(sizeof(Object*));
	pParser->pOperators = List_New(sizeof(Operator*));
	pParser->pTypes = List_New(sizeof(Type*));
	pParser->pVariables = List_New(sizeof(Variable*));

	//declare basic types
	CE1_CALL(Parser_DeclareType(pParser, "Bool",sizeof(bool),&Parser_ConvertFromStringToBool));
	CE1_CALL(Parser_DeclareType(pParser, "Float", sizeof(float), &Parser_ConvertFromStringToFloat));
	CE1_CALL(Parser_DeclareType(pParser, "LPCWSTR", sizeof(LPCWSTR), &Parser_ConvertFromStringToLPCWSTR));
	CE1_CALL(Parser_DeclareType(pParser, "String", sizeof(String), &Parser_ConvertFromStringToString));
	CE1_CALL(Parser_DeclareType(pParser, "UINT", sizeof(UINT), &Parser_ConvertFromStringToUINT));
	return pParser;
}

HRESULT Parser_RegisterOperator(Parser* pParser, char* d, OperatorCode code)
{
	CE1_ASSERT(pParser && d&&"invalid Parser");
	CE1_NEW(Operator, pOperator);
	CE1_STR(pOperator->pString,d);
	pOperator->code = code;
	List_PushBack(pParser->pOperators, (void*)pOperator); 
	return S_OK;
}

HRESULT Parser_DeclareType(Parser* pParser, char* pTypeName, size_t size, ConvertFromStringToTypeFunction pConvertFromStringToTypeFunction)
{
	CE1_ASSERT(pParser&& pTypeName &&size && "Parser_DeclareType");
	CE1_NEW(Type,pType);
	CE1_STR(pType->pTypeName, pTypeName);
	pType->size = size;
	pType->pConvertFromStringToTypeFunction = pConvertFromStringToTypeFunction;
	CE1_CALL(List_PushBack(pParser->pTypes, pType));
	return S_OK;
}

HRESULT Parser_DeclareVariable(Parser * pParser, char * pTypeName, char * pVariableName, ObjectHandlerFunction pObjectHandlerFunction)
{
	CE1_ASSERT(pParser && pTypeName && pVariableName && "Parser_DeclareVariable"); 
	CE1_NEW(Variable,pVar);
	CE1_STR(pVar->pTypeName, pTypeName);
	CE1_STR(pVar->pVariableName, pVariableName);
	pVar->pObjectHandlerFunction = pObjectHandlerFunction;
	CE1_CALL(List_PushBack(pParser->pVariables, pVar));
	return S_OK;
}

HRESULT Parser_SubmitObject(Parser* pParser) {
	CE1_ASSERT(pParser && List_Length(pParser->pObjects)>0 && "parsing syntax errror"); 
	Object* pChildObject = (Object*)List_Pop(pParser->pObjects);
	Object* pParentObject;
	if (List_Length(pParser->pObjects) > 0) {
		CE1_CALL(List_GetLast(pParser->pObjects, (void**)&pParentObject));
		CE1_CALL((*pParentObject->pVariable->pObjectHandlerFunction)(pParentObject, pChildObject->pVariable->pVariableName, pChildObject->pInst));
	}
	else {
		CE1_CALL((*pParser->pRootHandler)(NULL, pChildObject->pVariable->pVariableName, pChildObject->pInst));
	}
	CE1_DEL(pChildObject);
}

HRESULT Parser_Destroy(Parser * pParser)
{
	//exit if theres no parser
	if (!pParser) {
		return S_OK;
	}

	//delete variables
	Variable* pVariable;
	CE1_LISTEXEC(
		pParser->pVariables,
		pVariable = (Variable*)List_Get(itr);
		CE1_DEL(pVariable->pTypeName->pBuffer);
		CE1_DEL(pVariable->pTypeName);
		CE1_DEL(pVariable->pVariableName->pBuffer);
		CE1_DEL(pVariable->pVariableName);
	);
	CE1_CALL(List_FullDelete(pParser->pVariables, true));

	//delete types
	Type* pType;
	CE1_LISTEXEC(
		pParser->pTypes,
		pType = (Type*)List_Get(itr);
		CE1_DEL(pType->pTypeName->pBuffer);
		CE1_DEL(pType->pTypeName);
	);
	CE1_CALL(List_FullDelete(pParser->pTypes,true));
	
	//delete object list ->there should not be elements in this list
	CE1_CALL(List_FullDelete(pParser->pObjects, false));

	//delete Operators
	Operator* pOperator;
	CE1_LISTEXEC(
		pParser->pOperators,
		pOperator = (Operator*)List_Get(itr);
		CE1_DEL(pOperator->pString->pBuffer);
		CE1_DEL(pOperator->pString);
	);
	CE1_CALL(List_FullDelete(pParser->pOperators, true));

	CE1_DEL(pParser);
	return S_OK;
}

BOOL Parser_isOperator(Parser* pParser, char* pBuffer, Operator** ppOperator) {
	CE1_ASSERT(pParser &&"invalid Parser"); 
	Operator* pOperator;
	CE1_LISTEXEC(
		pParser->pOperators,
		pOperator = (Operator*)List_Get(itr);
		if(CE1_CMPSTR(pOperator->pString->pBuffer, pBuffer, pOperator->pString->length)) {
			*ppOperator = pOperator;
			return true;
		}
	);
	return false;
}

BOOL Parser_isVariable(Parser* pParser, char* pBuffer, Variable** ppVariable) {
	CE1_ASSERT(pParser &&"invalid Parser");
	Variable* pVariable;
	CE1_LISTEXEC(
		pParser->pVariables,
		pVariable = (Variable*)List_Get(itr);
		if (CE1_CMPSTR(pVariable->pVariableName->pBuffer, pBuffer, pVariable->pVariableName->length)) {
			*ppVariable = pVariable;
			return true;
		}
	);
	return false;
}

HRESULT Parser_CreateObjectFromType(Type* pType, Object* pObject) {
	pObject->pInst = malloc(pType->size);
	pObject->pType = pType;
	return S_OK;
}

HRESULT Parser_GetTypeByName(Parser* pParser, String* pTypeName, Type** ppType){
	Type* pType;
	CE1_LISTEXEC(
		pParser->pTypes,
		pType = (Type*)List_Get(itr);
		if (CE1_CMPSTR(pType->pTypeName->pBuffer, pTypeName->pBuffer, pTypeName->length)) {
			*ppType = pType;
			return S_OK;
		}
	);
	CE1_ASSERT(0&&"Type not found");
	return ERROR_SUCCESS;
}

HRESULT Parser_ParseFile(Parser * pParser, char* pFileName, ObjectHandlerFunction pRootHandler)
{
	CE1_ASSERT(pParser && pRootHandler && List_Length(pParser->pTypes)>0 && "invalid parser");
	String* pFileContent;
	CE1_CALL(FileReader_Read(pFileName, &(pFileContent)));
	pParser->pRootHandler = pRootHandler;

	Variable* pVariable = 0;
	Object* pCurrentObject = 0;
	size_t currentStatementIndex = 0;
	char* currentStatement = 0;
	size_t BufferIndex = 0;
	Object* pObj;
	Type* pType;
	Operator* pOperator;
	CE1_NEW(String, pValueString);

	for (BufferIndex = 0; BufferIndex < pFileContent->length; BufferIndex = BufferIndex + currentStatementIndex+1) {
		for (currentStatementIndex = 0; ; currentStatementIndex++) {
			if (Parser_isOperator(pParser, pFileContent->pBuffer + (BufferIndex + currentStatementIndex) * sizeof(char), &pOperator)) {
				switch (pOperator->code) {
					case submit:
						pValueString->length = currentStatementIndex- pOperator->pString->length; 
						pValueString->pBuffer = pFileContent->pBuffer + BufferIndex * sizeof(char);
						CE1_CALL(List_GetLast(pParser->pObjects,(void**)&pObj));
						CE1_CALL((*pObj->pType->pConvertFromStringToTypeFunction)(pObj->pInst,pValueString));
						CE1_CALL(Parser_SubmitObject(pParser));
						break;
					case ignore:
						break;
				}
				break;
			}else if (Parser_isVariable(pParser, pFileContent->pBuffer + (BufferIndex+currentStatementIndex) * sizeof(char), &pVariable)) {
				_NEW(Object, pObj);
				pObj->pVariable = pVariable;
				CE1_CALL(Parser_GetTypeByName(pParser, pVariable->pTypeName, &pType));
				CE1_CALL(Parser_CreateObjectFromType(pType, pObj));
				CE1_CALL(List_PushBack(pParser->pObjects, pObj));
				currentStatementIndex = pVariable->pVariableName->length;
				break;
			}
		}
	}

	CE1_DEL(pValueString);
	CE1_DEL(pFileContent->pBuffer);
	CE1_DEL(pFileContent);
	return S_OK;
}

HRESULT Parser_ConvertFromStringToFloat(void* pObject, String* pString) {
	*(FLOAT*)pObject = strtod(pString->pBuffer, NULL);
	return S_OK;
}

HRESULT Parser_ConvertFromStringToBool(void* pObject, String* pString) {
	(pString->pBuffer[0] == 'f') ? *(bool*)pObject = FALSE : *(bool*)pObject = TRUE;
	return S_OK;
}

HRESULT Parser_ConvertFromStringToString(void* pObject, String* pString) {
	((String*)pObject)->pBuffer = (char*)malloc(pString->length*sizeof(char));
	for (int i = 0; i < pString->length; i++) {
		((String*)pObject)->pBuffer[i] = pString->pBuffer[i];
	}
	((String*)pObject)->length = pString->length;
	return S_OK;
}

HRESULT Parser_ConvertFromStringToUINT(void* pObject, String* pString) {
	*(UINT*)pObject = (UINT)atoi(pString->pBuffer);
	return S_OK;
}

HRESULT Parser_ConvertFromStringToLPCWSTR(void* pObject, String* pString) {
	LPCWSTR pWString = (wchar_t*)malloc((pString->length+2)*sizeof(wchar_t));
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, (wchar_t*)pWString, pString->length+2, pString->pBuffer, pString->length+1);
	*(LPCWSTR*)pObject = pWString;
	return S_OK;
}