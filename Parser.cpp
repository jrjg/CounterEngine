#include "TopInclude.h"
#include "FileReader.h"
#include "List.h"
#include "Engine.h"

#include "Parser.h"

Parser * Parser_New()
{
	Parser* pParser;
	_NEW(Parser,pParser);
	pParser->pHandlers = List_New(sizeof(Handler*));
	pParser->pObjects = List_New(sizeof(Object*));
	pParser->pSingleCharDividers = List_New(sizeof(char));
	_NEW(FileContent,pParser->pFileContent);
	return pParser;
}

HRESULT Parser_LoadContentFromFile(Parser* pParser, String* pFileName)
{
	CEASSERT(pParser&&pFileName);
	SAFECALL(FileReader_Read(pFileName->buffer, &(pParser->pFileContent)));
	return S_OK;
}

HRESULT Parser_CollapseString(String* pObjectName, int index) {
	CEASSERT(pObjectName);
	if (pObjectName->length == 0) {
		return S_OK;
	}
	if (index < pObjectName->length) {
		for (int i = index; i < pObjectName->length - 1; i++) {
			pObjectName->buffer[i] = pObjectName->buffer[i + 1];
		}
	}
	pObjectName->length--;
	return S_OK;
}

HRESULT Parser_CleanString(String* pObjectName) {
	CEASSERT(pObjectName);
	char c;
	for (int i = 0; i < pObjectName->length; i++) {

		//single collapse
		c = pObjectName->buffer[i];
		if (c == ' '
			|| c == '"'
			|| c == '='
			) {
			SAFECALL(Parser_CollapseString(pObjectName, i));
		}

		//double collaps
		if (c == '\\' && pObjectName->buffer[i + 1] == 'n'
			|| c == '\\' && pObjectName->buffer[i + 1] == 'b'
			|| c == '\\' && pObjectName->buffer[i + 1] == 'r'
			|| c == '\\' && pObjectName->buffer[i + 1] == 'b'
			|| c == '\\' && pObjectName->buffer[i + 1] == 'f'
			|| c == '\\' && pObjectName->buffer[i + 1] == '\"'
			) {
			SAFECALL(Parser_CollapseString(pObjectName, i));
			SAFECALL(Parser_CollapseString(pObjectName, i));
		}
	}

}

HRESULT Parser_RegisterHandler(Parser* pParser, String* pObjectName, HandlerFunction pHandlerFunction, HandlerOnCompleteFunction pOnCompleteHandlerFunction)
{
	CEASSERT(pParser&&pObjectName&&pHandlerFunction);
	Handler* pHandler;
	_NEW(Handler, pHandler);
	pHandler->pHandlerFunction = pHandlerFunction;
	pHandler->pObjectName = pObjectName;
	pHandler->pHandlerOnCompleteFunction = pOnCompleteHandlerFunction;
	List_PushBack(pParser->pHandlers, pHandler);
	return S_OK;
}

HRESULT Parser_RegisterSingleCharHandler(Parser* pParser, char d, HandlerFunction pHandlerFunction)
{
	CEASSERT(pParser && d&&"invalid Parser");
	char* pCharacter;
	_NEW(char, pCharacter);
	*pCharacter = d;
	List_PushBack(pParser->pSingleCharDividers, (void*)pCharacter);
	String* pSingleCharString;
	_NEW(String,pSingleCharString);
	pSingleCharString->buffer = pCharacter;
	pSingleCharString->length = 1;
	SAFECALL(Parser_RegisterHandler(pParser, pSingleCharString, pHandlerFunction,NULL));
	return S_OK;
}

HRESULT Parser_CompleteCurrentObject(Parser* pParser) {
	CEASSERT(pParser && List_Length(pParser->pObjects)>0 && "parsing syntax errror");
	Object* pObject = (Object*)List_Pop(pParser->pObjects);
	pParser->pCurrentHandler->pHandlerOnCompleteFunction(&pObject->inst, pObject->pHandler->pObjectName);
	_DEL(pObject);
}

HRESULT Parser_Destroy(Parser * pParser)
{
	if (!pParser) {
		return S_OK;
	}
	_DEL(pParser->pFileContent->pBuffer);
	_DEL(pParser->pFileContent);

	//delete handler list
	Handler* pHandler;
	ExecOnList(
		pParser->pHandlers,
		pHandler = (Handler*)List_Get(itr);
		_DEL(pHandler->pObjectName->buffer);
		_DEL(pHandler->pObjectName);
	);
	SAFECALL(List_FullDelete(pParser->pHandlers,true));

	//delete object list ->there should not be elements in this list
	SAFECALL(List_FullDelete(pParser->pObjects, false));

	//full delete is enough
	SAFECALL(List_FullDelete(pParser->pSingleCharDividers, true));

	_DEL(pParser);
	return S_OK;
}

BOOL Parser_isDivider(Parser* pParser, char c) {
	CEASSERT(pParser && c&&"invalid Parser");
	char* d;
	ExecOnList(
		pParser->pSingleCharDividers,
		d = (char*)List_Get(itr);
		if ((*d) == c) {
			return true;
		}
	);
	return false;
}


HRESULT CallHandler(Parser* pParser, Handler* pHandler, size_t BufferIndex, size_t currentStatementIndex, String* pTempString) {
	Object* pCurrentObject = 0;
	CEASSERT(pHandler&&"no valid Handler found");

	//get the currently processed object
	SAFECALL(List_GetLast(pParser->pObjects, (void**)&pCurrentObject));

	//call the handler function and submit the currently processed object and buffer snippet
	pTempString->buffer = pParser->pFileContent->pBuffer + BufferIndex * sizeof(char*);
	pTempString->length = currentStatementIndex;
	SAFECALL((*(pHandler->pHandlerFunction))((void**)&pCurrentObject, pTempString));
}

HRESULT Parser_Parse(Parser * pParser)
{
	//check for valid parser
	CEASSERT(pParser && pParser->pFileContent->size>0 && List_Length(pParser->pHandlers)>0 && "invalid parser");

	//initialize variables
	pParser->pCurrentHandler = 0;
	Handler* pTempHandler = 0;
	HandlerFunction f = 0;
	Object* pCurrentObject = 0;
	size_t currentStatementIndex = 0;
	char* currentStatement = 0;
	size_t BufferIndex = 0;
	List* pHandlersCopy = List_New(sizeof(Handler*));
	String* pTempString;
	_NEW(String, pTempString);
	BOOL skipHandler;

	//query whole buffer
	for (BufferIndex = 0; BufferIndex < pParser->pFileContent->size; BufferIndex = BufferIndex + currentStatementIndex) {

		//copy Handler list for next statement
		ExecOnList(
			pParser->pHandlers,

			//before copying, make sure that we dont take in the singlechar handler: these are handled differently
			pTempHandler = (Handler*)List_Get(itr);
			if (pTempHandler->pHandlerOnCompleteFunction) {
				List_PushBack(pHandlersCopy, List_Get(itr));
			}
		);

		skipHandler = false;

		//identify Handler
		for (currentStatementIndex = 0; ; currentStatementIndex++) {

			//exit if this character is an divider
			if (Parser_isDivider(pParser, pParser->pFileContent->pBuffer[BufferIndex + currentStatementIndex])) {
				
				//search handler for this character
				ExecOnList(
					pParser->pHandlers,
					pTempHandler = ((Handler*)List_Get(itr));
					if (pTempHandler->pObjectName->buffer[0] == pParser->pFileContent->pBuffer[BufferIndex + currentStatementIndex]) {
						
						//call singe-char-handler
						CallHandler(pParser, pTempHandler, BufferIndex, currentStatementIndex, pTempString);
					}
				);
				skipHandler = true;
				break;
			}

			//There is only 1 handler remaining in the list -> check if its the right one
			if (List_Length(pHandlersCopy) == 1) {

				//get the only Handler left, which is also the last one in the list
				pTempHandler = 0;
				SAFECALL(List_GetLast(pHandlersCopy, (void**)&pTempHandler));

				//if the found name equals the handlers name
				if (CHAREQS(pTempHandler->pObjectName->buffer, pParser->pFileContent->pBuffer + BufferIndex * sizeof(char*), pTempHandler->pObjectName->length)) {

					//make this handler the current handler
					pParser->pCurrentHandler = pTempHandler;
					
					//extend currentStatementIndex to full namelength and exit
					currentStatementIndex = pTempHandler->pObjectName->length;
					break;
				
				//If the names dont match, then no handler is available -> the datachunk is handled by current handler
				}else {

					//Delete the remaining handler
					SAFECALL(List_DeleteAllElements(pHandlersCopy, false));
				}
			}

			//delete all mismatching Handlers
			ExecOnList(
				pHandlersCopy,
				pTempHandler = ((Handler*)List_Get(itr));
				if (pTempHandler->pObjectName->buffer[currentStatementIndex] != pParser->pFileContent->pBuffer[BufferIndex + currentStatementIndex]) {
					SAFECALL(List_DeleteElement(pHandlersCopy, itr->_id, FALSE));
				}
			);
		}

		if (!skipHandler) {

			//two or more handlers are matching
			CEASSERT(List_Length(pHandlersCopy) <= 1 && "nonunique handlers");

			//call current handler
			CallHandler(pParser, pParser->pCurrentHandler, BufferIndex, currentStatementIndex, pTempString);
		}

		//reset handler list copy
		List_DeleteAllElements(pHandlersCopy,false);
	}

	_DEL(pTempString);
	return S_OK;
}

