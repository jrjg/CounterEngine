#ifndef FILEREADER_INC
#define FILEREADER_INC

struct FileContent {
	char* pBuffer;
	size_t size;
};

HRESULT FileReader_Read(char * filename, FileContent** ppContent);

#endif