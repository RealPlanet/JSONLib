#include "JValue.h"

using namespace json;

JValuePtr JValue::create_str_value(const std::string& s)
{
	return JValuePtr(new JValue(ValueType::String, s));
}

JValuePtr JValue::create_number_value(const double d)
{
	return JValuePtr(new JValue(ValueType::Number, std::to_string(d)));
}

JValuePtr JValue::create_number_value(const int64_t i)
{
	return JValuePtr(new JValue(ValueType::Number, std::to_string(i)));
}

JValuePtr JValue::create_boolean_value(const bool b)
{
	return JValuePtr(new JValue(ValueType::Boolean, b ? "true" : "false"));
}

JValuePtr JValue::create_null_value()
{
	return JValuePtr(new JValue(ValueType::Null, "null"));
}

JValue::JValue(ValueType type, const std::string& source)
{
	m_Value = source;
	m_Type = type;

	m_Value.shrink_to_fit();
}

bool JValue::is_string() const
{
	return m_Type == ValueType::String;
}

bool JValue::is_number() const
{
	return m_Type == ValueType::Number;
}

bool JValue::is_integer_number() const
{
	return is_number() && m_Value.find('.') == -1;
}

bool JValue::is_boolean() const
{
	return m_Type == ValueType::Boolean;
}

bool JValue::is_null() const
{
	return m_Type == ValueType::Null;
}

const std::string& JValue::as_string() const
{
	return m_Value;
}

int64_t JValue::as_integer() const
{
	return std::stoi(m_Value);
}

double JValue::as_fractional() const
{
	return std::stod(m_Value);
}

bool JValue::as_bool() const
{
	return m_Value == "true";
}

void JValue::operator=(const char* newVal)
{
	m_Type = ValueType::String;
	m_Value = newVal;
}

void JValue::operator=(bool newVal)
{
	m_Type = ValueType::Boolean;
	m_Value = std::to_string(newVal);
}

void JValue::operator=(int64_t newVal)
{
	m_Type = ValueType::Number;
	m_Value = std::to_string(newVal);
}

void JValue::operator=(double newVal)
{
	m_Type = ValueType::Number;
	m_Value = std::to_string(newVal);
}

JElementPtr JValue::copy() const
{
	return JValuePtr(new JValue(m_Type, as_string()));
}

std::string JValue::to_string(bool prettyPrint /*= false*/, int indentLevel /*= 0*/) const
{
	if (m_Type == ValueType::Number || m_Type == ValueType::Boolean)
	{
		return as_string();
	}

	std::string output;
	const std::string& val = as_string();
	for (int i{ 0 }; i < val.size(); i++)
	{
		switch (val[i])
		{
		case '\b':
			output += "\\b";
			break;
		case '\f':
			output += "\\f";
			break;
		case '\n':
			output += "\\n";
			break;
		case '\r':
			output += "\\r";
			break;
		case '\t':
			output += "\\t";
			break;
		case '\\':
			output += "\\\\";
			break;
		case '/':
			output += "\\/";
			break;
		case '\"':
			output += "\\\"";
			break;
		default:
			output += val[i];
		}
	}

	return "\"" + output + "\"";
}
