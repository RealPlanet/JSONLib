#include "JSerialization.h"

json::JElementPtr json::Serialize(int64_t source)
{
	return json::JValue::create_number_value(source);
}

json::JElementPtr json::Serialize(bool source)
{
	return json::JValue::create_boolean_value(source);
}

json::JElementPtr json::Serialize(double source)
{
	return json::JValue::create_number_value(source);
}

json::JElementPtr json::Serialize(float source)
{
	return json::JValue::create_number_value((double)source);
}

json::JElementPtr json::Serialize(const std::string& source)
{
	return json::JValue::create_str_value(source);
}
