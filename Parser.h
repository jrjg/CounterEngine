//#ifndef PARSER_INC
//#define PARSER_INC
//
//typedef HRESULT(*ObjectHandlerFunction)(void* pParentObject, String<char>* pChildObjectName, void* pChildObject);
//typedef HRESULT(*ConvertFromStringToTypeFunction)(void* pObject, String<char>* pString);
//typedef enum { ignore, submit }OperatorCode;
//
//class Type : public MemManaged{
//public:
//	String<char>* mpTypeName;
//	size_t mSize;
//	ConvertFromStringToTypeFunction mpConvertFromStringToTypeFunction;
//	Type(String<char>* pTypeName, size_t Size, ConvertFromStringToTypeFunction pConvertFromStringToTypeFunction) : mpTypeName(pTypeName), mSize(Size), mpConvertFromStringToTypeFunction(pConvertFromStringToTypeFunction) {}
//	~Type() { delete mpTypeName; }
//};
//
//class Variable : public MemManaged{
//public:
//	String<char>* mpVariableName;
//	String<char>* mpTypeName;
//	ObjectHandlerFunction mpObjectHandlerFunction;
//	Variable(String<char>* pVariableName, String<char>* pTypeName, ObjectHandlerFunction pObjectHandlerFunction) : mpVariableName(pVariableName), mpTypeName(pTypeName), mpObjectHandlerFunction(pObjectHandlerFunction) {};
//	~Variable() {delete mpVariableName;delete mpTypeName;};
//};
//
//class Object : public MemManaged {
//public:
//	MemManaged* mpInst;
//	Type* mpType;
//	Variable* mpVariable;
//	bool mDeleteContent;
//	Object(MemManaged* pInst, Type* pType, Variable* pVariable) : mpInst(pInst), mpType(pType), mpVariable(pVariable),mDeleteContent(true) {};
//	~Object() { if (mDeleteContent) { delete mpInst; } };
//}; 
//
//class Operator : public MemManaged {
//public:
//	String<char>* mpString;
//	OperatorCode mCode;
//	Operator(String<char>* pString, OperatorCode Code) : mpString(pString), mCode(Code) {};
//	~Operator() { delete mpString; };
//};
//
//class Parser : public MemManaged {
//private:
//	List<Variable>* mpVariables;
//	List<Object>* mpObjects;
//	List<Type>* mpTypes;
//	List<Operator>* mpOperators;
//	ObjectHandlerFunction mpRootHandler;
//	void* mpRoot;
//	HRESULT submitObject();
//	HRESULT convertFromStringToFloat(void*,String<char>* pString);
//	HRESULT convertFromStringToBool(void*, String<char>* pString);
//	HRESULT convertFromStringToString(void*, String<char>* pString);
//	HRESULT convertFromStringToUINT(void*, String<char>* pString);
//	HRESULT convertFromStringToLPCWSTR(void*, String<char>* pString);
//	BOOL isOperator(char* pBuffer, Operator** ppOperator);
//	BOOL isVariable(char* pBuffer, Variable** ppVariable);
//	HRESULT getTypeByName(String<char>* pTypeName, Type** ppType);
//
//public:
//	Parser();
//	~Parser() { delete mpVariables; delete mpObjects; delete mpOperators; delete mpTypes; };
//	HRESULT declareType(char* pTypeName, size_t size, ConvertFromStringToTypeFunction pConvertFromStringToTypeFunction) {mpTypes->pushBack(new Type(new String<char>(pTypeName, strlen(pTypeName)), size, pConvertFromStringToTypeFunction));return S_OK;};
//	HRESULT declareVariable(char* pTypeName, char* pVariableName, ObjectHandlerFunction pObjectHandlerFunction) { mpVariables->pushBack(new Variable(new String<char>(pVariableName,strlen(pVariableName)), new String<char>(pTypeName, strlen(pTypeName)), pObjectHandlerFunction)); };
//	HRESULT registerOperator(char* pChars, OperatorCode code) { mpOperators->pushBack(new Operator(new String<char>(pChars,strlen(pChars)),code)); return S_OK; };
//	HRESULT parseFile(char* pFileName, ObjectHandlerFunction pRootHandler, void* pRoot);
//}; 
//
//#endif