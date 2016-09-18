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
	pParser->pSingleCharOperators = List_New(sizeof(SingleCharOperator*));
	pParser->pTypes = List_New(sizeof(Type*));
	return pParser;
}

HRESULT Parser_RegisterSingleCharOperator(Parser* pParser, char d, SingeCharOperatorCode code)
{
	CEASSERT(pParser && d&&"invalid Parser");
	CE1_NEW(SingleCharOperator, pSC);
	_NEW(char, (pSC->pChar));
	*(pSC->pChar) = d;
	pSC->code = code;
	List_PushBack(pParser->pSingleCharOperators, (void*)pSC);
	return S_OK;
}

HRESULT Parser_RegisterVariable(Parser* pParser, String* pVariableName, String* pTypeName, size_t size, ObjectHandlerFunction pObjectHandlerFunction)
{
	CEASSERT(pParser&& pTypeName&& pVariableName &&size && "Parser_RegisterObject");
	CE1_NEW(Type,pType);
	pType->pTypeName = pTypeName;
	pType->size = size;
	pType->pObjectHandlerFunction = pObjectHandlerFunction;
	CE1_CALL(List_PushBack(pParser->pTypes, pType));
	return S_OK;
}

HRESULT Parser_CompleteCurrentObject(Parser* pParser) {
	CEASSERT(pParser && List_Length(pParser->pObjects)>0 && "parsing syntax errror");

	//get the just created object
	Object* pChildObject = (Object*)List_Pop(pParser->pObjects);

	//get its parent
	Object* pParentObject;
	if (List_Length(pParser->pObjects) > 0) {
		CE1_CALL(List_GetLast(pParser->pObjects, (void**)&pParentObject));
	}
	else {
		(*pParser->pRootHandler)(NULL, pChildObject->pType->pTypeName, pChildObject->pInst);
	}

	//submit the child to its parent
	CE1_CALL((*pParentObject->pType->pObjectHandlerFunction)(pParentObject, pChildObject->pType->pTypeName, pChildObject->pInst));

	//release the carrier
	CE1_DEL(pChildObject);
}

HRESULT Parser_Destroy(Parser * pParser)
{
	//exit if theres no parser
	if (!pParser) {
		return S_OK;
	}

	//delete types
	Type* pType;
	ExecOnList(
		pParser->pTypes,
		pType = (Type*)List_Get(itr);
		CE1_DEL(pType->pTypeName->pBuffer);
		CE1_DEL(pType->pTypeName);
	);
	CE1_CALL(List_FullDelete(pParser->pTypes,true));
	
	//delete object list ->there should not be elements in this list
	CE1_CALL(List_FullDelete(pParser->pObjects, false));

	//delete SingleCharOperators
	SingleCharOperator* pSingleCharOperator;
	ExecOnList(
		pParser->pSingleCharOperators,
		pSingleCharOperator = (SingleCharOperator*)List_Get(itr);
		CE1_DEL(pSingleCharOperator->pChar);
	);
	CE1_CALL(List_FullDelete(pParser->pSingleCharOperators, true));

	CE1_DEL(pParser);
	return S_OK;
}

BOOL Parser_isSingleCharOperator(Parser* pParser, char c, SingeCharOperatorCode* pCode) {
	CEASSERT(pParser && c&&"invalid Parser");
	SingleCharOperator* pSingleCharOperator;
	ExecOnList(
		pParser->pSingleCharOperators,
		pSingleCharOperator = (SingleCharOperator*)List_Get(itr);
		if ((*pSingleCharOperator->pChar) == c) {
			*pCode = pSingleCharOperator->code;
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

#define CASE(TYPESTRING,EXEC) if (CHAREQS(pType->pBuffer, TYPESTRING, pType->length)) {EXEC}

HRESULT Parser_ConvertStringToType(String* pValueString, String* pType,void* pValue) {
	CASE("FLOAT", 
		*(FLOAT*)pValue = strtod(pValueString->pBuffer, NULL); 
		return S_OK;);

	CASE("BOOL", 
		(pValueString->pBuffer[0] == 'f') ? *(bool*)pValue = FALSE : *(bool*)pValue = TRUE; 
		return S_OK;);

	CASE("STRING", 
		for (int i = 0; i < pValueString->length; i++) { 
			((String*)pValue)->pBuffer[i] = pValueString->pBuffer[i]; 
		} 
		((String*)pValue)->length = pValueString->length; 
		return S_OK;);

	CASE("UINT", 
		*(UINT*)pValue = (UINT)atoi(pValueString->pBuffer); 
		return S_OK;);

	CASE("LPCWSTR", 
		MultiByteToWideChar(CP_ACP, 0, pValueString->pBuffer, pValueString->length, (LPWSTR)pValue, pValueString->length); 
		return S_OK;);

	CEASSERT(0&&"Could not convert String: Type unknown");
	return ERROR_SUCCESS;
}

HRESULT Parser_SetObjectValueFromString(Object* pObject,String* pValueString) {
	CE1_CALL(Parser_ConvertStringToType(pValueString, pObject->pType->pTypeName, pObject->pInst));
	return S_OK;
}

HRESULT Parser_ParseFromFile(Parser * pParser, String* pFileName, ObjectHandlerFunction pRootHandler)
{
	//check for valid parser
	CEASSERT(pParser && pRootHandler && List_Length(pParser->pTypes)>0 && "invalid parser");

	//read file 
	String* pFileContent;
	CE1_CALL(FileReader_Read(pFileName->pBuffer, &(pFileContent)));

	//set root handler
	pParser->pRootHandler = pRootHandler;

	//initialize variables
	Type* pType = 0;
	Object* pCurrentObject = 0;
	size_t currentStatementIndex = 0;
	char* currentStatement = 0;
	size_t BufferIndex = 0;
	List* pTypesCopy = List_New(sizeof(Type*));
	Object* pObj;
	CE1_NEW(String, pValueString);

	//query whole buffer
	for (BufferIndex = 0; BufferIndex < pFileContent->length; BufferIndex = BufferIndex + currentStatementIndex) {

		//reset 
		CE1_CALL(List_DeleteAllElements(pTypesCopy, false));
		pValueString->length = 0;

		//copy Handler list for next statement
		ExecOnList(
			pParser->pTypes,
			List_PushBack(pTypesCopy, List_Get(itr));
		);

		//identify type
		for (currentStatementIndex = 0; ; currentStatementIndex++) {

			//exit if this character is registered as an single char operator
			SingeCharOperatorCode code = ignore;
			if (Parser_isSingleCharOperator(pParser, pFileContent->pBuffer[BufferIndex + currentStatementIndex], &code)) {
				switch (code) {
					case submit:
						pValueString->length = currentStatementIndex-1; //dont take in single char operator
						pValueString->pBuffer = pFileContent->pBuffer + BufferIndex * sizeof(char);
						CE1_CALL(List_GetLast(pParser->pObjects,(void**)&pObj));
						CE1_CALL(Parser_SetObjectValueFromString(pObj, pValueString));
						CE1_CALL(Parser_CompleteCurrentObject(pParser));
						break;
					case ignore:
						break;
				}
				break;
			}

			//There is only 1 type remaining in the list -> check if its the right one
			if (List_Length(pTypesCopy) == 1) {
				CE1_CALL(List_GetLast(pTypesCopy, (void**)&pType));
				if (CHAREQS(pType->pTypeName->pBuffer, pFileContent->pBuffer + BufferIndex * sizeof(char), pType->pTypeName->length)) {
					_NEW(Object, pObj);
					CE1_CALL(Parser_CreateObjectFromType(pType, pObj));
					CE1_CALL(List_PushBack(pParser->pObjects, pObj));
					currentStatementIndex = pType->pTypeName->length;
					break;
				
				//value 
				}else {
					CE1_CALL(List_DeleteAllElements(pTypesCopy, false));
				}
			}

			//delete all mismatching types
			ExecOnList(
				pTypesCopy,
				pType = ((Type*)List_Get(itr));
				if (pType->pTypeName->pBuffer[currentStatementIndex] != pFileContent->pBuffer[BufferIndex + currentStatementIndex]) {
					CE1_CALL(List_DeleteElement(pTypesCopy, itr->_id, FALSE));
				}
			);
		}
	}
	CE1_DEL(pValueString);
	CE1_DEL(pFileContent->pBuffer);
	CE1_DEL(pFileContent);

	return S_OK;
}















//HRESULT Parser_CollapseString(String* pObjectName, int index) {
//	CEASSERT(pObjectName);
//	if (pObjectName->length == 0) {
//		return S_OK;
//	}
//	if (index < pObjectName->length) {
//		for (int i = index; i < pObjectName->length - 1; i++) {
//			pObjectName->buffer[i] = pObjectName->buffer[i + 1];
//		}
//	}
//	pObjectName->length--;
//	return S_OK;
//}
//
//HRESULT Parser_CleanString(String* pObjectName) {
//	CEASSERT(pObjectName);
//	char c;
//	for (int i = 0; i < pObjectName->length; i++) {
//
//		//single collapse
//		c = pObjectName->buffer[i];
//		if (c == ' '
//			|| c == '"'
//			|| c == '='
//			) {
//			CE1_CALL(Parser_CollapseString(pObjectName, i));
//		}
//
//		//double collaps
//		if (c == '\\' && pObjectName->buffer[i + 1] == 'n'
//			|| c == '\\' && pObjectName->buffer[i + 1] == 'b'
//			|| c == '\\' && pObjectName->buffer[i + 1] == 'r'
//			|| c == '\\' && pObjectName->buffer[i + 1] == 'b'
//			|| c == '\\' && pObjectName->buffer[i + 1] == 'f'
//			|| c == '\\' && pObjectName->buffer[i + 1] == '\"'
//			) {
//			CE1_CALL(Parser_CollapseString(pObjectName, i));
//			CE1_CALL(Parser_CollapseString(pObjectName, i));
//		}
//	}
//
//}

//HRESULT Parser_RegisterHandler(Parser* pParser, String* pObjectName, HandlerFunction pHandlerFunction, HandlerOnCompleteFunction pOnCompleteHandlerFunction)
//{
//	CEASSERT(pParser&&pObjectName&&pHandlerFunction);
//	Handler* pHandler;
//	_NEW(Handler, pHandler);
//	pHandler->pHandlerFunction = pHandlerFunction;
//	pHandler->pObjectName = pObjectName;
//	pHandler->pHandlerOnCompleteFunction = pOnCompleteHandlerFunction;
//	List_PushBack(pParser->pHandlers, pHandler);
//	return S_OK;
//}

//HRESULT CallHandler(Parser* pParser, Handler* pHandler, size_t BufferIndex, size_t currentStatementIndex, String* pTempString) {
//	Object* pCurrentObject = 0;
//	CEASSERT(pHandler&&"no valid Handler found");
//
//	//get the currently processed object
//	CE1_CALL(List_GetLast(pParser->pObjects, (void**)&pCurrentObject));
//
//	//call the handler function and submit the currently processed object and buffer snippet
//	pTempString->buffer = pParser->pFileContent->pBuffer + BufferIndex * sizeof(char*);
//	pTempString->length = currentStatementIndex;
//	CE1_CALL((*(pHandler->pHandlerFunction))((void**)&pCurrentObject, pTempString));
//}