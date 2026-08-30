#include "JArray.h"
#include "JUtility.h"
using namespace json;

void JArray::push_back(JElementPtr value)
{
	m_Elements.push_back(std::move(value));
}

void JArray::clear()
{
	m_Elements.clear();
}

JElement* JArray::operator[](size_t index)
{
	return m_Elements[index].get();
}

const JElement* JArray::operator[](size_t index) const
{
	return m_Elements[index].get();
}

JElement* JArray::at(size_t index)
{
	return m_Elements.at(index).get();
}

const JElement* JArray::at(size_t index) const
{
	return m_Elements.at(index).get();
}

std::string JArray::to_string(bool prettyPrint /*= false*/, int indentLevel /*= 0*/) const
{
	std::string indentation = utility::calculate_indentation(prettyPrint, indentLevel);
	std::string result = prettyPrint ? "[\n" : "[";
	for (size_t i{ 0 }; i < m_Elements.size(); ++i)
	{
		result += indentation + m_Elements[i]->to_string(prettyPrint, indentLevel + 1);
		if (i + 1 < m_Elements.size())
		{
			result += ", ";
		}

		if (prettyPrint)
		{
			result += "\n";
		}
	}

	return result + utility::calculate_indentation(prettyPrint, indentLevel - 1) + "] ";
}

JElementPtr JArray::copy() const
{
	auto c = std::make_unique<JArray>();
	for (const auto& p : m_Elements)
	{
		c->push_back(p->copy());
	}

	return c;
}
