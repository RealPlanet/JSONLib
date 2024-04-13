#include "JSONObject.h"
#include "JSONUtility.h"

using namespace json;

void Object::insert(const std::string& name, Element* value)
{
	this->m_Elements.insert(std::make_pair(name, value));
}

std::string Object::to_string(bool prettyPrint /*= false*/, int indentLevel /*= 0*/)
{
	std::string indentation = utility::calculate_indentation(prettyPrint, indentLevel);
	std::string objString = prettyPrint ? "{\n" : "{";
	for (auto i{ m_Elements.begin() }; i != m_Elements.end(); i++)
	{
		objString += indentation + "\"" + i->first + "\" : " + i->second->to_string(prettyPrint, indentLevel + 1);

		if (std::next(i) != m_Elements.end())
			objString += ",";

		if (prettyPrint)
			objString += "\n";
	}

	objString += utility::calculate_indentation(prettyPrint, indentLevel - 1) + "}";
	return objString;
}

Element* Object::copy() const
{
	Object* c = new Object();
	for (auto p : m_Elements)
		c->insert(p.first, p.second->copy());

	return c;
}

Object* Element::as_object()
{
	return dynamic_cast<Object*>(this);
}