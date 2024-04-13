#include "JSONArray.h"
#include "JSONUtility.h"

using namespace json;

Array::~Array()
{
	this->clear();
}

void Array::push_back(Element* value)
{
	this->m_Elements.push_back(value);
}

void Array::clear()
{
	for (auto ptr : m_Elements)
		delete ptr;

	m_Elements.clear();
}

Element* Array::operator[](size_t index)
{
	return this->m_Elements[index];
}

Element* json::Array::at(size_t index)
{
	return this->operator[](index);
}

std::string Array::to_string(bool prettyPrint /*= false*/, int indentLevel /*= 0*/)
{
	std::string indentation = ::utility::calculate_indentation(prettyPrint, indentLevel);

	std::string result = prettyPrint ? "[\n" : "[";
	for (int i{ 0 }; i < m_Elements.size(); i++)
	{
		result += indentation + m_Elements[i]->to_string(prettyPrint, indentLevel + 1);
		if (i + 1 < m_Elements.size())
		{
			result += ", ";
		}

		if (prettyPrint)
			result += "\n";
	}

	result += ::utility::calculate_indentation(prettyPrint, indentLevel - 1) + "] ";
	return result;
}

Element* Array::copy() const
{
	Array* c = new Array();
	for (Element* p : m_Elements)
		c->push_back(p->copy());

	return c;
}

Array* Element::as_array()
{
	return dynamic_cast<Array*>(this);
}
