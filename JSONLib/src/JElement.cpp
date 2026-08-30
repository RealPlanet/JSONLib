#include "JElement.h"

#include "JArray.h"
#include "JObject.h"
#include "JValue.h"

using namespace json;

JValue* JElement::as_literal()
{
	return dynamic_cast<JValue*>(this);
}

const JValue* JElement::as_literal() const
{
	return dynamic_cast<const JValue*>(this);
}

JObject* JElement::as_object()
{
	return dynamic_cast<JObject*>(this);
}

const JObject* json::JElement::as_object() const
{
	return dynamic_cast<const JObject*>(this);
}

JArray* JElement::as_array()
{
	return dynamic_cast<JArray*>(this);
}

const JArray* json::JElement::as_array() const
{
	return dynamic_cast<const JArray*>(this);
}

JElement::operator const std::string& () const
{
	const JValue* literal = as_literal();
	if (literal != nullptr)
	{
		return literal->as_string();
	}

	static std::string def;
	return def;
}

JElement::operator JObject* ()
{
	return as_object();
}

JElement::operator const JObject* () const
{
	return as_object();
}

JElement::operator JArray* ()
{
	return as_array();
}

JElement::operator const JArray* () const
{
	return as_array();
}

JElement::operator JValue* ()
{
	return as_literal();
}

JElement::operator const JValue* () const
{
	return as_literal();
}
