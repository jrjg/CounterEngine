#include "TopInclude.h"
#include "FileReader.h"
#include "List.h"

#include "Parser.h"

Parser * Parser_New()
{
	Parser* pParser;
	_NEW(Parser,pParser);
	pParser->pCMDCallbacks = List_New(sizeof(CMDHandler*));
	pParser->pContent = 0;
	pParser->pCurrentObject = nullptr;
	return pParser;
}

HRESULT Parser_LoadContentFromFile(Parser * pParser, char * filename)
{
	pParser->pContent = FileReader_Read(filename); 
	return S_OK;
}

HRESULT Parser_RegisterCommand(Parser * pParser, char * command, CMDCB pCB)
{
	CMDHandler* pCMDH;
	_NEW(CMDHandler,pCMDH);
	pCMDH->command = command;
	pCMDH->pCB = pCB;
	List_PushBack(pParser->pCMDCallbacks, pCMDH);
	return S_OK;
}
