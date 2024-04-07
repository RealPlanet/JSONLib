#include "JSONObject.h"
#include "JSONUtility.h"

using namespace json;

void Object::insert(const std::string& name, Element* value)
{
	this->_Elements.insert(std::make_pair(name, value));
}

std::string Object::to_string(bool prettyPrint /*= false*/, int indentLevel /*= 0*/)
{
	std::string indentation = utility::calculate_indentation(prettyPrint, indentLevel);
	std::string objString = prettyPrint ? "{\n" : "{";
	for (auto i{ _Elements.begin() }; i != _Elements.end(); i++)
	{
		objString += indentation + "\"" + i->first + "\" : " + i->second->to_string(prettyPrint, indentLevel + 1);

		if (std::next(i) != _Elements.end())
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
	for (auto p : _Elements)
		c->insert(p.first, p.second->copy());

	return c;
}

Object* Element::as_object()
{
	return dynamic_cast<Object*>(this);
}