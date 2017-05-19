#include "json_parser.h"
#include <sstream>


#define JSON_TABS	2


/************************************************************
 *	@brief		Operator []
 *	@param[in]	name				Name
 *	@retval		const CJsonValue&	Value
 ************************************************************/
CJsonValue& CJsonObject::operator [] (std::string name)
{
	static CJsonValue dummy_value;


	std::list<std::pair<std::string, CJsonValue>>::iterator it;
	
	for (it = m_Object.begin(); it != m_Object.end(); it++)
	{
		if (it->first == name)
		{
			return it->second;
		}
	}


	return dummy_value;
}


/************************************************************
 *	@brief		Get size
 *	@retval		size_t			Size
 ************************************************************/
size_t CJsonObject::GetSize(void)
{
	return m_Object.size();
}


/************************************************************
 *	@brief		Add
 *	@param[in]	name			Name
 *	@param[in]	value			Value
 *	@retval		None
 ************************************************************/
void CJsonObject::Add(std::string name, CJsonValue& value)
{
	m_Object.push_back(std::pair<std::string, CJsonValue> (name, value));
}


/************************************************************
 *	@brief		Merge
 *	@param[in]	object			Object
 *	@retval		None
 ************************************************************/
void CJsonObject::Merge(CJsonObject& object)
{
	m_Object.insert(m_Object.end(), object.m_Object.begin(), object.m_Object.end());
}


/************************************************************
 *	@brief		Deserialize
 *	@param[in]	json			JSON string
 *	@retval		true / false	Success / Failure
 ************************************************************/
bool CJsonObject::Deserialize(std::string& json)
{
	std::string temp = json;

	// Check if object start
	size_t object_spos = temp.find("{");

	if (std::string::npos == object_spos)
	{
		return false;
	}

	temp.erase(0, object_spos + 1);


	for (;;)
	{
		// Get name start
		size_t name_spos = temp.find_first_of("\"}");

		if (std::string::npos == name_spos)
		{
			return false;
		}


		// Check if object is empty
		if ('}' == temp[name_spos])
		{
			temp.erase(0, name_spos + 1);

			json = temp;

			return true;
		}


		// Get name end
		size_t name_epos = temp.find("\"", name_spos + 1);

		if (std::string::npos == name_epos)
		{
			return false;
		}


		// Get name
		std::string name (temp, name_spos + 1, name_epos - name_spos - 1);

		temp.erase(0, name_epos + 1);


		// Get value start
		size_t value_spos = temp.find_first_not_of(":\t\n\v\f\r ");

		if (std::string::npos == value_spos)
		{
			return false;
		}

		temp.erase(0, value_spos);


		// Deserialize value
		CJsonValue value;

		if (false == value.Deserialize(temp))
		{
			return false;
		}


		// Add object
		m_Object.push_back(std::pair<std::string, CJsonValue> (name, value));


		// Check if object end
		size_t object_epos = temp.find_first_of(",}");

		if (std::string::npos == object_epos)
		{
			return false;
		}

		if (',' == temp[0])
		{
			temp.erase(0, object_epos + 1);

			continue;
		}
		else
		{
			temp.erase(0, object_epos + 1);

			break;
		}
	}


	json = temp;

	return true;
}


/************************************************************
 *	@brief		Serialize
 *	@param[in]	nDepth			Depth
 *	@retval		string			JSON string
 ************************************************************/
std::string CJsonObject::Serialize(int nDepth)
{
	std::stringstream st;


	std::string depth_tabs;

	for (int cnt = 0; cnt < nDepth * JSON_TABS; cnt++)
	{
		depth_tabs.append(" ");
	}


	std::string tabs;

	for (int cnt = 0; cnt < JSON_TABS; cnt++)
	{
		tabs.append(" ");
	}


	st << depth_tabs << "{" << std::endl;

	std::list<std::pair<std::string, CJsonValue>>::iterator it;

	int cnt = 0;

	for (it = m_Object.begin(); it != m_Object.end(); it++)
	{
		st << depth_tabs << tabs << "\"" << it->first << "\"" << " : ";
		
		st << it->second.Serialize(nDepth + 1);

		if (++cnt != m_Object.size())
		{
			st << ",";
		}

		st << std::endl;
	}

	st << depth_tabs << "}";


	return st.str();
}


/************************************************************
 *	@brief		Operator []
 *	@param[in]	index				Index
 *	@retval		const CJsonValue&	Value
 ************************************************************/
CJsonValue& CJsonArray::operator [] (size_t index)
{
	static CJsonValue dummy_value;


	int cnt = 0;

	std::list<CJsonValue>::iterator it;

	for (it = m_Array.begin(); it != m_Array.end(); it++, cnt++)
	{
		if (cnt == index)
		{
			return *it;
		}
	}


	return dummy_value;
}


/************************************************************
 *	@brief		Get size
 *	@retval		size_t			Size
 ************************************************************/
size_t CJsonArray::GetSize(void)
{
	return m_Array.size();
}


/************************************************************
 *	@brief		Add
 *	@param[in]	value			Value
 *	@retval		None
 ************************************************************/
void CJsonArray::Add(CJsonValue& value)
{
	m_Array.push_back(value);
}


/************************************************************
 *	@brief		Merge
 *	@param[in]	array			Array
 *	@retval		None
 ************************************************************/
void CJsonArray::Merge(CJsonArray& array)
{
	m_Array.insert(m_Array.end(), array.m_Array.begin(), array.m_Array.end());
}


/************************************************************
 *	@brief		Deserialize
 *	@param[in]	json			JSON string
 *	@retval		true / false	Success / Failure
 ************************************************************/
bool CJsonArray::Deserialize(std::string& json)
{
	std::string temp = json;


	// Check if array start
	size_t array_spos = temp.find("[");

	if (std::string::npos == array_spos)
	{
		return false;
	}

	temp.erase(0, array_spos + 1);


	for (;;)
	{
		// Deserialize value
		CJsonValue value;

		if (false == value.Deserialize(temp))
		{
			return false;
		}


		// Add object
		m_Array.push_back(value);


		// Check if array end
		size_t array_epos = temp.find_first_of(",]");

		if (std::string::npos == array_epos)
		{
			return false;
		}

		if (',' == temp[0])
		{
			temp.erase(0, array_epos + 1);

			continue;
		}
		else
		{
			temp.erase(0, array_epos + 1);

			break;
		}
	}


	json = temp;

	return true;
}


/************************************************************
 *	@brief		Serialize
 *	@param[in]	nDepth			Depth
 *	@retval		string			JSON string
 ************************************************************/
std::string CJsonArray::Serialize(int nDepth)
{
	std::stringstream st;


	std::string depth_tabs;

	for (int cnt = 0; cnt < nDepth * JSON_TABS; cnt++)
	{
		depth_tabs.append(" ");
	}


	st << depth_tabs << "[" << std::endl;

	std::list<CJsonValue>::iterator it;

	int cnt = 0;

	for (it = m_Array.begin(); it != m_Array.end(); it++)
	{
		st << it->Serialize(nDepth + 1);

		if (++cnt != m_Array.size())
		{
			st << ",";
		}

		st << std::endl;
	}

	st << depth_tabs << "]";


	return st.str();
}


/************************************************************
 *	@brief		Constructor
 *	@param[in]	object			Object
 *	@retval		None
 ************************************************************/
CJsonValue::CJsonValue(CJsonObject& object)
{
	SetObject(object);
}


/************************************************************
 *	@brief		Constructor
 *	@param[in]	array			Array
 *	@retval		None
 ************************************************************/
CJsonValue::CJsonValue(CJsonArray& array)
{
	SetArray(array);
}


/************************************************************
 *	@brief		Constructor
 *	@param[in]	value			Value
 *	@retval		None
 ************************************************************/
CJsonValue::CJsonValue(std::string& value)
{
	SetString(value);
}


/************************************************************
 *	@brief		Operator []
 *	@param[in]	name				Name
 *	@retval		const CJsonValue&	Value
 ************************************************************/
CJsonValue& CJsonValue::operator [] (std::string name)
{
	static CJsonValue dummy_value;


	// Object
	if (JSON_TYPE_OBJECT == m_Type)
	{
		return m_Object[name];
	}


	return dummy_value;
}


/************************************************************
 *	@brief		Operator []
 *	@param[in]	index				Index
 *	@retval		const CJsonValue&	Value
 ************************************************************/
CJsonValue& CJsonValue::operator [] (size_t index)
{
	static CJsonValue dummy_value;


	// Array
	if (JSON_TYPE_ARRAY == m_Type)
	{
		return m_Array[index];
	}


	return dummy_value;
}


/************************************************************
 *	@brief		Operator std::string
 *	@param[in]	name			Name
 *	@retval		None
 ************************************************************/
CJsonValue::operator std::string ()
{
	return m_String;
}


/************************************************************
 *	@brief		Get type
 *	@retval		JSON_TYPE		Type
 ************************************************************/
JSON_TYPE CJsonValue::GetType(void)
{
	return m_Type;
}


/************************************************************
 *	@brief		Get size
 *	@retval		size_t			Size
 ************************************************************/
size_t CJsonValue::GetSize(void)
{
	// Object
	if (JSON_TYPE_OBJECT == m_Type)
	{
		return m_Object.GetSize();
	}

	// Array
	else if (JSON_TYPE_ARRAY == m_Type)
	{
		return m_Array.GetSize();
	}

	return 0;
}


/************************************************************
 *	@brief		Get object
 *	@retval		CJsonObject&	Object
 ************************************************************/
CJsonObject& CJsonValue::GetObject(void)
{
	return m_Object;
}


/************************************************************
 *	@brief		Get array
 *	@retval		CJsonArray&		Array
 ************************************************************/
CJsonArray& CJsonValue::GetArray(void)
{
	return m_Array;
}


/************************************************************
 *	@brief		Get string
 *	@retval		std::string&	String
 ************************************************************/
std::string& CJsonValue::GetString(void)
{
	return m_String;
}


/************************************************************
 *	@brief		Set object
 *	@param[in]	object			Object
 *	@retval		None
 ************************************************************/
void CJsonValue::SetObject(CJsonObject& object)
{
	m_Type = JSON_TYPE_OBJECT;

	m_Object = object;
}


/************************************************************
 *	@brief		Set array
 *	@param[in]	array			Array
 *	@retval		None
 ************************************************************/
void CJsonValue::SetArray(CJsonArray& array)
{
	m_Type = JSON_TYPE_ARRAY;

	m_Array = array;
}


/************************************************************
 *	@brief		Set string
 *	@param[in]	value			Value
 *	@retval		None
 ************************************************************/
void CJsonValue::SetString(std::string& string)
{
	m_Type = JSON_TYPE_STRING;

	m_String = string;
}


/************************************************************
 *	@brief		Deserialize
 *	@param[in]	json			JSON string
 *	@retval		true / false	Success / Failure
 ************************************************************/
bool CJsonValue::Deserialize(std::string& json)
{
	std::string temp = json;


	// Check if value start
	size_t array_spos = temp.find_first_not_of("\t\n\v\f\r ");

	if (std::string::npos == array_spos)
	{
		return false;
	}

	temp.erase(0, array_spos);


	if (!temp.size())
	{
		return false;
	}


	// Object
	if ('{' == temp[0])
	{
		CJsonObject object;

		object.Deserialize(temp);

		SetObject(object);
	}

	// Array
	else if ('[' == temp[0])
	{
		CJsonArray array;

		array.Deserialize(temp);

		SetArray(array);
	}

	// String
	else if ('\"' == temp[0])
	{
		// Get string start
		size_t string_spos = temp.find("\"");

		if (std::string::npos == string_spos)
		{
			return false;
		}


		// Get string end
		size_t string_epos = temp.find("\"", string_spos + 1);

		if (std::string::npos == string_epos)
		{
			return false;
		}


		// Get string
		std::string string (temp, string_spos + 1, string_epos - string_spos - 1);

		temp.erase(0, string_epos + 1);


		// Set string
		SetString(string);
	}

	// Others
	else
	{
		return false;
	}


	json = temp;

	return true;
}


/************************************************************
 *	@brief		Serialize
 *	@param[in]	nDepth			Depth
 *	@retval		string			JSON string
 ************************************************************/
std::string CJsonValue::Serialize(int nDepth)
{
	std::stringstream st;


	// Object
	if (JSON_TYPE_OBJECT == m_Type)
	{
		st << m_Object.Serialize(nDepth);
	}

	// Array
	else if (JSON_TYPE_ARRAY == m_Type)
	{
		st << m_Array.Serialize(nDepth);
	}

	// String
	else if (JSON_TYPE_STRING == m_Type)
	{
		st << "\"" << m_String << "\"";
	}


	return st.str();
}
