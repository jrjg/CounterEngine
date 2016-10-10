#ifndef FILEREADER_INC
#define FILEREADER_INC

template<class ObjectType>class FileReader : public MemManaged {
public:
	static String<ObjectType>* FileReader<ObjectType>::read(char * filename);
};

template<class ObjectType>
String<ObjectType>* FileReader<ObjectType>::read(char * filename) {
	if (!filename) { return ERROR_SUCCESS; }
	FILE *pFile = fopen(filename, "r");
	CE1_ASSERT(pFile && "File not found");

	size_t pos = ftell(pFile);
	fseek(pFile, 0, SEEK_END); 
	size_t length = ftell(pFile);
	fseek(pFile, pos, SEEK_SET);  

	String<ObjectType>* pString = new String<ObjectType>(length);
	ObjectType* pBuffer = pString->getBuffer();

	int c;
	for (int n = 0; ((c = getc(pFile)) != EOF); n++) {
		pBuffer[n] = c;
	}

	fclose(pFile);
	return pString;
}

#endif