
#include "JSON.h"
#include "JSONValue.h"
#include "JSONArray.h"
#include "JSONObject.h"
#include "JSONUtility.h"

#include <fstream>
#include <cassert>
#include <sstream>
#include <iostream>


namespace json {
	/*
	char get_escaped_char(std::istream& stream) {
		//
		//		escape
		//		'"'
		//		'\'
		//		'/'
		//		'b'
		//		'f'
		//		'n'
		//		'r'
		//		't'
		//		'u' hex hex hex hex ???
		//

		JSON_PEEK_AND_SKIP(stream, '\\');

		char next;
		stream.read(&next, 1);

		if (next == 'u') {
			// Unsupported ?
			throw json_exception("Unicode characters curretly not supported");
		}

		switch (next)
		{
		case '"':
		case '\\':
		case '/':
			return next;
		case 'r':
			return '\r';
		case 'b':
			return '\b';
		case 'n':
			return '\n';
		case 'f':
			return '\f';
		case 't':
			return '\t';
		}

		throw json_exception("Escape character not supported!");
	}
	*/

	JSON::JSON(JSON&& m) noexcept
	{
		this->m_Root = m.m_Root;
		m.m_Root = nullptr;
	}

	JSON::JSON(const JSON& m)
	{
		this->m_Root = m.m_Root->copy();
	}

	JSON::~JSON()
	{
		if (m_Root != nullptr)
			delete m_Root;
	}

	Element* JSON::operator[](std::string key)
	{
		Object* o = dynamic_cast<Object*>(m_Root);
		if (o != nullptr)
		{
			auto it = o->_Elements.find(key);
			if (it != o->_Elements.end())
				return it->second;
		}

		return nullptr;
	}

	//JSONValue* JSON::operator[](size_t key)
	//{
	//	JSONArray* o = dynamic_cast<JSONArray*>(m_Root);
	//	if (o != nullptr)
	//	{
	//		return o->_Elements[key];
	//	}
	//}

	bool JSON::operator!=(const JSON& other) const
	{
		return !(*this == other);
	}

	bool JSON::operator==(const JSON& other) const
	{
		std::string thisString = to_string(false);
		std::string otherString = other.to_string(false);
		return  thisString == otherString;
	}

	std::string JSON::to_string(bool prettyPrint) const
	{
		if (m_Root == nullptr)
			return "";

		return m_Root->to_string(prettyPrint, 1);
	}

	bool JSON::is_valid() const
	{
		return m_Root != nullptr;
	}
}
