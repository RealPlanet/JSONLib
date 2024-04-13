#include "JSONValue.h"

using namespace json;

LiteralValue* LiteralValue::create_str_value(const std::string& s)
{
	return new LiteralValue(ValueType::String, s);
}

LiteralValue* LiteralValue::create_number_value(const double d)
{
	return new LiteralValue(ValueType::Number, std::to_string(d));
}

LiteralValue* LiteralValue::create_number_value(const int64_t i)
{
	return new LiteralValue(ValueType::Number, std::to_string(i));
}

LiteralValue* LiteralValue::create_boolean_value(const bool b)
{
	return new LiteralValue(ValueType::Boolean, b ? "true" : "false");
}

LiteralValue* LiteralValue::create_null_value()
{
	return new LiteralValue(ValueType::Null, "null");
}

LiteralValue* Element::as_literal()
{
	return dynamic_cast<LiteralValue*>(this);
}

LiteralValue::LiteralValue(ValueType type, const std::string& source)
{
	m_Value = source;
	m_Type = type;

	m_Value.shrink_to_fit();
}

bool LiteralValue::is_string()
{
	return m_Type == ValueType::String;
}

bool LiteralValue::is_number()
{
	return m_Type == ValueType::Number;
}

bool LiteralValue::is_integer_number()
{
	return is_number() && m_Value.find('.') == -1;
}

bool LiteralValue::is_boolean()
{
	return m_Type == ValueType::Boolean;
}

bool LiteralValue::is_null()
{
	return m_Type == ValueType::Null;
}

const std::string& LiteralValue::as_str() const
{
	return m_Value;
}

int64_t json::LiteralValue::as_integer() const
{
	return std::stoi(m_Value);
}

double json::LiteralValue::as_fractional() const
{
	return std::stod(m_Value);
}

bool json::LiteralValue::as_bool() const
{
	return m_Value == "true";
}

void json::LiteralValue::operator=(const char* newVal)
{
	m_Type = ValueType::String;
	m_Value = newVal;
}

void LiteralValue::operator=(bool newVal)
{
	m_Type = ValueType::Boolean;
	m_Value = std::to_string(newVal);
}

void LiteralValue::operator=(int64_t newVal)
{
	m_Type = ValueType::Number;
	m_Value = std::to_string(newVal);
}

void LiteralValue::operator=(double newVal)
{
	m_Type = ValueType::Number;
	m_Value = std::to_string(newVal);
}

Element* LiteralValue::copy() const
{
	return new LiteralValue(this->m_Type, this->as_str());
}

std::string LiteralValue::to_string(bool prettyPrint /*= false*/, int indentLevel /*= 0*/)
{
	if (m_Type == ValueType::Number ||
		m_Type == ValueType::Boolean)
		return as_str();

	std::string output;
	const std::string& val = as_str();
	for (int i{ 0 }; i < val.size(); i++)
	{
		switch (val[i])
		{
		case '\b':  output += "\\b"; break;
		case '\f':  output += "\\f"; break;
		case '\n':  output += "\\n"; break;
		case '\r':  output += "\\r"; break;
		case '\t':  output += "\\t"; break;
		case '\\':  output += "\\\\"; break;
			//case '/':  output += "\\/"; break;
		case '\"':  output += "\\\""; break;
		default: output += val[i];
		}
	}

	return "\"" + output + "\"";
}
