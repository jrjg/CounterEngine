#include "TopInclude.h"
#include "Engine.h"
#include "FileReader.h"

HRESULT FileReader_Read(char * filename, String** ppContent)
{
	//open file
	if (!filename) { return ERROR_SUCCESS; }
	FILE *pFile = fopen(filename, "r");
	CE1_ASSERT(pFile && "File not found");

	//get file size
	size_t pos = ftell(pFile);    // Current position
	fseek(pFile, 0, SEEK_END);    // Go to end
	size_t length = ftell(pFile); // read the position which is the size
	fseek(pFile, pos, SEEK_SET);  // restore original position

	//create buffer
	String* pString;
	_NEW(String,pString);
	pString->pBuffer = (char*)malloc(length);
	pString->length = length;

	//read file
	int c;
	for (int n = 0; ((c = getc(pFile)) != EOF);n++ ) {
		pString->pBuffer[n] = c;
	}

	//close file
	fclose(pFile);

	*ppContent = pString;

	return S_OK;
}