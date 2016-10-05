#include "TopInclude.h"
#include "FileReader.h"
#include "List.h"
#include "Engine.h"
#include "String.h"
#include "Memory.h"

#include "Parser.h"

HRESULT Parser::submitObject() {
	Object* pChildObject = (Object*)mpObjects->pop(); 
	Object* pParentObject;
	if (mpObjects->getLength() > 0) {
		pParentObject = mpObjects->getLast()->getObject();
		(*pParentObject->mpVariable->mpObjectHandlerFunction)(pParentObject, pChildObject->mpVariable->mpVariableName, pChildObject->mpInst);
	}
	else {
		(*mpRootHandler)(mpRoot, pChildObject->mpVariable->mpVariableName, pChildObject->mpInst);
	}
	delete pChildObject;
}

BOOL Parser::isOperator(char* pBuffer, Operator** ppOperator) {
	for (ListElement<Operator>* pListElem = mpOperators->getFirst(); pListElem != NULL; pListElem = pListElem->getNext()) {
		if (*pListElem->getObject()->mpString == pBuffer) {
			*ppOperator = pListElem->getObject();
			return true;
		}
	}
	return false;
}

BOOL Parser::isVariable(char* pBuffer, Variable** ppVariable) {
	for (ListElement<Variable>* pListElem = mpVariables->getFirst(); pListElem != NULL; pListElem = pListElem->getNext()) {
		if (*pListElem->getObject()->mpVariableName == pBuffer) {
			*ppVariable = pListElem->getObject();
			return true;
		}
	}
	return false;
}

HRESULT Parser_CreateObjectFromType(Type* pType, Object* pObject) {
	pObject->mpInst = (MemManaged*)malloc(pType->mSize);
	pObject->mpType = pType;
	return S_OK;
}

HRESULT Parser::getTypeByName(String<char>* pTypeName, Type** ppType) {
	for (ListElement<Type>* pListElem = mpTypes->getFirst(); pListElem != NULL; pListElem = pListElem->getNext()) {
		if (*pListElem->getObject()->mpTypeName == *pTypeName) {
			*ppType = pListElem->getObject();
			return S_OK;
		}
	}
	return ERROR_SUCCESS;
}

HRESULT Parser_ParseFile(Parser * pParser, char* pFileName, ObjectHandlerFunction pRootHandler, void* pRoot)
{
	CE1_ASSERT(pParser && pRootHandler && List_Length(pParser->pTypes)>0 && "invalid parser");
	String* pFileContent;
	CE1_CALL(FileReader_Read(pFileName, &(pFileContent)));
	pParser->pRootHandler = pRootHandler;
	pParser->pRoot = pRoot;

	Variable* pVariable = 0;
	Object* pCurrentObject = 0;
	size_t currentStatementIndex = 0;
	char* currentStatement = 0;
	size_t BufferIndex = 0;
	Object* pObj;
	Type* pType;
	Operator* pOperator;
	CE1_NEW(String, pValueString);

	for (BufferIndex = 0; BufferIndex < pFileContent->length; BufferIndex = BufferIndex + currentStatementIndex + 1) {
		for (currentStatementIndex = 0; ; currentStatementIndex++) {
			if (Parser_isOperator(pParser, pFileContent->pBuffer + (BufferIndex + currentStatementIndex) * sizeof(char), &pOperator)) {
				switch (pOperator->code) {
				case submit:
					pValueString->length = currentStatementIndex - pOperator->pString->length;
					pValueString->pBuffer = pFileContent->pBuffer + BufferIndex * sizeof(char);
					CE1_CALL(List_GetLast(pParser->pObjects, (void**)&pObj));
					CE1_CALL((*pObj->pType->pConvertFromStringToTypeFunction)(pObj->pInst, pValueString));
					CE1_CALL(Parser_SubmitObject(pParser));
					break;
				case ignore:
					break;
				}
				break;
			}
			else if (Parser_isVariable(pParser, pFileContent->pBuffer + (BufferIndex + currentStatementIndex) * sizeof(char), &pVariable)) {
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

HRESULT Parser::convertFromStringToFloat(void* pObject, String<char>* pString) {
	*(FLOAT*)pObject = strtod(pString->getBuffer(), NULL);
	return S_OK;
}

HRESULT Parser_ConvertFromStringToBool(void* pObject, String<char>* pString) {
	(pString->getBuffer()[0] == 'f') ? *(bool*)pObject = FALSE : *(bool*)pObject = TRUE;
	return S_OK;
}

HRESULT Parser_ConvertFromStringToString(void* pObject, String<char>* pString) {
	char* pBuffer = ( (String<char>*)pObject )->getBuffer();
	pBuffer = (char*)malloc(pString->length * sizeof(char));
	
	for (int i = 0; i < pString->length; i++) {
		((String<char>*)pObject)->pBuffer[i] = pString->pBuffer[i];
	}
	((String<char>*)pObject)->length = pString->length;
	return S_OK;
}

HRESULT Parser_ConvertFromStringToUINT(void* pObject, String* pString) {
	*(UINT*)pObject = (UINT)atoi(pString->pBuffer);
	return S_OK;
}

HRESULT Parser_ConvertFromStringToLPCWSTR(void* pObject, String* pString) {
	LPCWSTR pWString = (wchar_t*)malloc((pString->length + 2) * sizeof(wchar_t));
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, (wchar_t*)pWString, pString->length + 2, pString->pBuffer, pString->length + 1);
	*(LPCWSTR*)pObject = pWString;
	return S_OK;
}

Parser::Parser()
{
	mpObjects = new List<Object>(true); //delete remaining objects
	mpOperators = new List<Operator>(true);
	mpTypes = new List<Type>(true);
	mpVariables = new List<Variable>(true);

	declareType("Bool", sizeof(bool), &convertFromStringToBool);
	declareType("Float", sizeof(float), &convertFromStringToFloat);
	declareType("LPCWSTR", sizeof(LPCWSTR), &convertFromStringToLPCWSTR);
	declareType("String", sizeof(String<char>), &convertFromStringToString);
	declareType("UINT", sizeof(UINT), &convertFromStringToUINT);
}