#ifndef PARSER_INC
#define PARSER_INC

template<class ObjectType>typedef HRESULT(*ObjectHandlerFunction)(void* pParentObject, String<ObjectType>* pChildObjectName, void* pChildObject);
template<class ObjectType>typedef HRESULT(*ConvertFromStringToTypeFunction)(void* pObject, String<ObjectType>* pString);
typedef enum { ignore, submit }OperatorCode;

template<class ObjectType> class Type : public MemManaged{
public:
	Type(String<ObjectType>* pTypeName, size_t Size, ConvertFromStringToTypeFunction<ObjectType> pConvertFromStringToTypeFunction) : mpTypeName(pTypeName), mSize(Size), mpConvertFromStringToTypeFunction(pConvertFromStringToTypeFunction) {}
	~Type() { delete mpTypeName; }
	String<ObjectType>* mpTypeName;
	size_t mSize;
	ConvertFromStringToTypeFunction<ObjectType> mpConvertFromStringToTypeFunction;
};

template<class ObjectType>class Variable : public MemManaged{
public:
	String<ObjectType>* mpVariableName;
	String<ObjectType>* mpTypeName;
	ObjectHandlerFunction<ObjectType> mpObjectHandlerFunction;
	Variable(String<ObjectType>* pVariableName, String<ObjectType>* pTypeName, ObjectHandlerFunction<ObjectType> pObjectHandlerFunction) : mpVariableName(pVariableName), mpTypeName(pTypeName), mpObjectHandlerFunction(pObjectHandlerFunction) {};
};

template<class ObjectType>class Object : public MemManaged {
public:
	void* mpInst;
	Type<ObjectType>* mpType;
	Variable<ObjectType>* mpVariable;
	Object(void* pInst, Type<ObjectType>* pType, Variable<ObjectType>* pVariable) : mpInst(pInst), mpType(pType), mpVariable(pVariable) {};
}; 

template<class ObjectType>class Operator : public MemManaged {
public:
	Operator(String<ObjectType>* pString, OperatorCode Code) : mpString(pString), mCode(Code) {};
	String<ObjectType>* mpString;
	OperatorCode mCode;
};

template<class ObjectType>class Parser : public MemManaged {
private:
	List<Variable<ObjectType>>* mpVariables;
	List<Object<ObjectType>>* mpObjects;
	List<Type<ObjectType>>* mpTypes;
	List<Operator<ObjectType>>* mpOperators;
	ObjectHandlerFunction<ObjectType> mpRootHandler;
	void* mpRoot;
	HRESULT submitObject();
	float* convertFromStringToFloat(String<ObjectType>* pString);
	bool* convertFromStringToBool(String<ObjectType>* pString);
	template<class ObjectType> String<ObjectType>* convertFromStringToString(String<ObjectType>* pString);
	UINT* convertFromStringToUINT(String<ObjectType>* pString);
	LPCWSTR* convertFromStringToLPCWSTR(String<ObjectType>* pString);
public:
	Parser();
	~Parser();
	HRESULT declareType(ObjectType* pTypeName, size_t size, ConvertFromStringToTypeFunction<ObjectType> pConvertFromStringToTypeFunction);
	HRESULT declareVariable(ObjectType* pTypeName, ObjectType* pVariableName, ObjectHandlerFunction<ObjectType> pObjectHandlerFunction);
	HRESULT registerOperator(ObjectType* d, OperatorCode code) { mpOperators->pushBack(new Operator<ObjectType>(d,code)); return S_OK; };
	HRESULT parseFile(char* pFileName, ObjectHandlerFunction<ObjectType> pRootHandler, void* pRoot);
}; 


template<class ObjectType> Parser<ObjectType>::Parser()
{
	mpObjects = new List<Object<ObjectType>>(false);
	mpOperators = new List<Operator<ObjectType>>(true);
	mpTypes = new List<Type<ObjectType>>(true);
	mpVariables = new List<Variable<ObjectType>>(true);

	//declare basic types
	declareType("Bool", sizeof(bool), &convertFromStringToBool);
	declareType("Float", sizeof(float), &convertFromStringToFloat);
	declareType("LPCWSTR", sizeof(LPCWSTR), &convertFromStringToLPCWSTR);
	declareType("String", sizeof(String<ObjectType>), &convertFromStringToString);
	declareType("UINT", sizeof(UINT), &convertFromStringToUINT);
}

template<class ObjectType>HRESULT Parser<ObjectType>::declareType(ObjectType* pTypeName, size_t size, ConvertFromStringToTypeFunction<ObjectType> pConvertFromStringToTypeFunction)
{

	mpTypes->pushBack( new Type<ObjectType>( new String<ObjectType>(pTypeName, size) ) );
	return S_OK; 
	CE1_NEW(Type, pType);
	CE1_STR(pType->pTypeName, pTypeName);
	pType->size = size;
	pType->pConvertFromStringToTypeFunction = pConvertFromStringToTypeFunction;
	CE1_CALL(List_PushBack(pParser->pTypes, pType));
	return S_OK;
}

HRESULT Parser::DeclareVariable(Parser * pParser, char * pTypeName, char * pVariableName, ObjectHandlerFunction pObjectHandlerFunction)
{
	CE1_ASSERT(pParser && pTypeName && pVariableName && "Parser_DeclareVariable");
	CE1_NEW(Variable, pVar);
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
		CE1_CALL((*pParser->pRootHandler)(pParser->pRoot, pChildObject->pVariable->pVariableName, pChildObject->pInst));
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
	CE1_CALL(List_FullDelete(pParser->pTypes, true));

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
	if (CE1_CMPSTR(pOperator->pString->pBuffer, pBuffer, pOperator->pString->length)) {
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

HRESULT Parser_GetTypeByName(Parser* pParser, String* pTypeName, Type** ppType) {
	Type* pType;
	CE1_LISTEXEC(
		pParser->pTypes,
		pType = (Type*)List_Get(itr);
	if (CE1_CMPSTR(pType->pTypeName->pBuffer, pTypeName->pBuffer, pTypeName->length)) {
		*ppType = pType;
		return S_OK;
	}
	);
	CE1_ASSERT(0 && "Type not found");
	return ERROR_SUCCESS;
}

HRESULT Parser_SortVariables(Parser * pParser) {
	void* pNewFirstElem;

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

HRESULT Parser_ConvertFromStringToFloat(void* pObject, String* pString) {
	*(FLOAT*)pObject = strtod(pString->pBuffer, NULL);
	return S_OK;
}

HRESULT Parser_ConvertFromStringToBool(void* pObject, String* pString) {
	(pString->pBuffer[0] == 'f') ? *(bool*)pObject = FALSE : *(bool*)pObject = TRUE;
	return S_OK;
}

HRESULT Parser_ConvertFromStringToString(void* pObject, String* pString) {
	((String*)pObject)->pBuffer = (char*)malloc(pString->length * sizeof(char));
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
	LPCWSTR pWString = (wchar_t*)malloc((pString->length + 2) * sizeof(wchar_t));
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, (wchar_t*)pWString, pString->length + 2, pString->pBuffer, pString->length + 1);
	*(LPCWSTR*)pObject = pWString;
	return S_OK;
}

#endif