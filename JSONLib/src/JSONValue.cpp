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

LiteralValue::LiteralValue(ValueType type, std::string source)
{
	source.shrink_to_fit();
	_value = std::move(source);
	_type = type;
}

bool LiteralValue::is_string()
{
	return _type == ValueType::String;
}

bool LiteralValue::is_number()
{
	return _type == ValueType::Number;
}

bool LiteralValue::is_integer_number()
{
	return is_number() && _value.find('.') == -1;
}

bool LiteralValue::is_boolean()
{
	return _type == ValueType::Boolean;
}

bool LiteralValue::is_null()
{
	return _type == ValueType::Null;
}

const std::string& LiteralValue::as_str() const
{
	return _value;
}

const std::u16string& json::LiteralValue::as_wstring() const
{
	throw 1;
}

int64_t json::LiteralValue::as_integer() const
{
	//assert(is_integer_number());
	return std::stoi(_value);
}

double json::LiteralValue::as_fractional() const
{
	return std::stod(_value);
}

bool json::LiteralValue::as_bool() const
{
	return _value == "true";
}

void json::LiteralValue::operator=(const char* newVal)
{
	_type = ValueType::String;
	_value = newVal;
}

void LiteralValue::operator=(bool newVal)
{
	_type = ValueType::Boolean;
	_value = std::to_string(newVal);
}

void LiteralValue::operator=(int64_t newVal)
{
	_type = ValueType::Number;
	_value = std::to_string(newVal);
}

void LiteralValue::operator=(double newVal)
{
	_type = ValueType::Number;
	_value = std::to_string(newVal);
}

Element* LiteralValue::copy() const
{
	return new LiteralValue(this->_type, this->as_str());
}

std::string LiteralValue::to_string(bool prettyPrint /*= false*/, int indentLevel /*= 0*/)
{
	if (_type == ValueType::Number ||
		_type == ValueType::Boolean)
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
