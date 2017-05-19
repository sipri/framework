#pragma once

#include <list>
#include <string>
#include <Windows.h>


/************************************************************
 *	@brief		JSON TYPE
 ************************************************************/
enum JSON_TYPE
{
	JSON_TYPE_OBJECT,
	JSON_TYPE_ARRAY,
	JSON_TYPE_STRING,
};


class CJsonValue;


/************************************************************
 *	@brief		JSON OBJECT
 ************************************************************/
class CJsonObject
{
public:
	// Constructor
	CJsonObject() {}

	// Destructor
	~CJsonObject() {}


	// Operator []
	CJsonValue& operator [] (std::string name);


	// Get size
	size_t GetSize(void);

	// Add
	virtual void Add(std::string name, CJsonValue& value);

	// Merge
	virtual void Merge(CJsonObject& object);

	// Deserialize
	virtual bool Deserialize(std::string& json);

	// Serialize
	virtual std::string Serialize(int nDepth = 0);


protected:
	std::list<std::pair<std::string, CJsonValue>> m_Object;
};


/************************************************************
 *	@brief		JSON ARRAY
 ************************************************************/
class CJsonArray
{
public:
	// Constructor
	CJsonArray() {}

	// Destructor
	~CJsonArray() {}


	// Operator []
	CJsonValue& operator [] (size_t index);


	// Get size
	size_t GetSize(void);

	// Add
	virtual void Add(CJsonValue& value);

	// Merge
	virtual void Merge(CJsonArray& array);

	// Deserialize
	virtual bool Deserialize(std::string& json);

	// Serialize
	virtual std::string Serialize(int nDepth = 0);


protected:
	std::list<CJsonValue> m_Array;
};


/************************************************************
 *	@brief		JSON VALUE
 ************************************************************/
class CJsonValue
{
public:
	// Constructor
	CJsonValue() {}

	// Constructor
	CJsonValue(CJsonObject& object);

	// Constructor
	CJsonValue(CJsonArray& array);

	// Constructor
	CJsonValue(std::string& value);

	// Destructor
	~CJsonValue() {}


	// Operator []
	CJsonValue& operator [] (std::string name);

	// Operator []
	CJsonValue& operator [] (size_t index);

	// Operator std::string
	operator std::string ();


	// Get type
	JSON_TYPE GetType(void);

	// Get size
	size_t GetSize(void);

	// Get object
	virtual CJsonObject& GetObject(void);

	// Get array
	virtual CJsonArray& GetArray(void);

	// Get string
	virtual std::string& GetString(void);


	// Set object
	virtual void SetObject(CJsonObject& object);

	// Set array
	virtual void SetArray(CJsonArray& array);

	// Set string
	virtual void SetString(std::string& string);


	// Deserialize
	virtual bool Deserialize(std::string& json);

	// Serialize
	virtual std::string Serialize(int nDepth = 0);


protected:
	JSON_TYPE m_Type;

	CJsonObject m_Object;

	CJsonArray m_Array;

	std::string m_String;
};
