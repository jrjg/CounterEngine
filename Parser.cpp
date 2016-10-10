//#include "TopInclude.h"
//#include "FileReader.h"
//#include "List.h"
//#include "Engine.h"
//#include "String.h"
//#include "Memory.h"
//
//#include "Parser.h"
//
//
//Parser::Parser()
//{
//	mpObjects = new List<Object>(false);
//	mpOperators = new List<Operator>(true);
//	mpTypes = new List<Type>(true);
//	mpVariables = new List<Variable>(true);
//
//	//declare basic types
//	declareType("Bool", sizeof(bool), &convertFromStringToBool);
//	declareType("Float", sizeof(float), &convertFromStringToFloat);
//	declareType("LPCWSTR", sizeof(LPCWSTR), &convertFromStringToLPCWSTR);
//	declareType("String", sizeof(String<char>), &convertFromStringToString);
//	declareType("UINT", sizeof(UINT), &convertFromStringToUINT);
//}
//
//HRESULT Parser::submitObject() {
//	Object* pChildObject = mpObjects->pop();
//	if (mpObjects->getLength() > 0) {
//		Object* pParentObject = mpObjects->getLast()->getObject();
//		(*pParentObject->mpVariable->mpObjectHandlerFunction)(pParentObject->mpInst, pChildObject->mpVariable->mpVariableName, pChildObject->mpInst);
//	}
//	else {
//		(*mpRootHandler)(mpRoot, pChildObject->mpVariable->mpVariableName, pChildObject->mpInst);
//	}
//	pChildObject->mDeleteContent = false;
//	delete pChildObject;
//}
//
//BOOL Parser::isOperator(char* pBuffer, Operator** ppOperator) {
//	for (ListElement<Operator>* pListELem = mpOperators->getFirst(); pListELem != NULL; pListELem = pListELem->getNext()) {
//		if (*pListELem->getObject()->mpString == pBuffer) {
//			*ppOperator = pListELem->getObject();
//			return true;
//		}
//	}
//	return false;
//}
//
//BOOL Parser::isVariable(char* pBuffer, Variable** ppVariable) {
//	for (ListElement<Variable>* pListELem = mpVariables->getFirst(); pListELem != NULL; pListELem = pListELem->getNext()) {
//		if (*pListELem->getObject()->mpVariableName == pBuffer) {
//			*ppVariable = pListELem->getObject();
//			return true;
//		}
//	}
//	return false;
//}
//
//HRESULT Parser_CreateObjectFromType(Type* pType, Object* pObject) {
//	MemoryManager::get()->allocateMem((void**)&pObject->mpInst, pType->mSize, &pObject->mpInst->mMemID);
//	pObject->mpType = pType;
//	return S_OK;
//}
//
//HRESULT Parser::getTypeByName(String<char>* pTypeName, Type** ppType) {
//	for (ListElement<Type>* pListElem = mpTypes->getFirst(); pListElem != NULL; pListElem = pListElem->getNext()) {
//		if (*pTypeName == *pListElem->getObject()->mpTypeName) {
//			*ppType = pListElem->getObject();
//			return S_OK;
//		}
//	}
//	return ERROR_SUCCESS;
//}
//
//HRESULT Parser::parseFile(char* pFileName, ObjectHandlerFunction pRootHandler, void* pRoot)
//{
//	String<char>* pFileContent = FileReader<char>::read(pFileName);
//	mpRootHandler = pRootHandler;
//	mpRoot = pRoot;
//
//	Variable* pVariable = 0;
//	Object* pCurrentObject = 0;
//	size_t currentStatementIndex = 0;
//	char* currentStatement = 0;
//	size_t BufferIndex = 0;
//	Object* pObj;
//	Type* pType;
//	Operator* pOperator;
//	String<char>* pValueString;
//
//	for (BufferIndex = 0; BufferIndex < pFileContent->getLength(); BufferIndex = BufferIndex + currentStatementIndex + 1) {
//		for (currentStatementIndex = 0; ; currentStatementIndex++) {
//			if (isOperator(pFileContent->getBuffer() + (BufferIndex + currentStatementIndex) * sizeof(char), &pOperator)) {
//				switch (pOperator->mCode) {
//				case submit:
//					pValueString-> = currentStatementIndex - pOperator->mpString->getLength();
//					pValueString->pBuffer = pFileContent->pBuffer + BufferIndex * sizeof(char);
//					CE1_CALL(List_GetLast(pParser->pObjects, (void**)&pObj));
//					CE1_CALL((*pObj->pType->pConvertFromStringToTypeFunction)(pObj->pInst, pValueString));
//					CE1_CALL(Parser_SubmitObject(pParser));
//					break;
//				case ignore:
//					break;
//				}
//				break;
//			}
//			else if (Parser_isVariable(pParser, pFileContent->pBuffer + (BufferIndex + currentStatementIndex) * sizeof(char), &pVariable)) {
//				_NEW(Object, pObj);
//				pObj->pVariable = pVariable;
//				CE1_CALL(Parser_GetTypeByName(pParser, pVariable->pTypeName, &pType));
//				CE1_CALL(Parser_CreateObjectFromType(pType, pObj));
//				CE1_CALL(List_PushBack(pParser->pObjects, pObj));
//				currentStatementIndex = pVariable->pVariableName->length;
//				break;
//			}
//		}
//	}
//
//	CE1_DEL(pValueString);
//	CE1_DEL(pFileContent->pBuffer);
//	CE1_DEL(pFileContent);
//	return S_OK;
//}
//
//HRESULT Parser::convertFromStringToFloat(void* pObject, String<char>* pString) {
//	*(FLOAT*)pObject = strtod(pString->getBuffer(), NULL);
//	return S_OK;
//}
//
//HRESULT Parser::convertFromStringToBool(void* pObject, String<char>* pString) {
//	(pString->getBuffer()[0] == 'f') ? *(bool*)pObject = FALSE : *(bool*)pObject = TRUE;
//	return S_OK;
//}
//
//HRESULT Parser::convertFromStringToString(void* pObject, String<char>* pString) {
//	((String*)pObject)->pBuffer = (char*)malloc(pString->length * sizeof(char));
//	for (int i = 0; i < pString->length; i++) {
//		((String*)pObject)->pBuffer[i] = pString->pBuffer[i];
//	}
//	((String*)pObject)->length = pString->length;
//	return S_OK;
//}
//
//HRESULT Parser::convertFromStringToUINT(void* pObject, String<char>* pString) {
//	*(UINT*)pObject = (UINT)atoi(pString->getBuffer());
//	return S_OK;
//}
//
//HRESULT Parser::convertFromStringToLPCWSTR(void* pObject, String<char>* pString) {
//	LPCWSTR pWString = (wchar_t*)malloc((pString->getLength() + 2) * sizeof(wchar_t));
//	MemoryManager::get()->allocateMem();
//	size_t convertedChars = 0;
//	mbstowcs_s(&convertedChars, (wchar_t*)pWString, pString->getLength() + 2, pString->pBuffer, pString->length + 1);
//	*(LPCWSTR*)pObject = pWString;
//	return S_OK;
//}