#include "JObject.h"
#include "JUtility.h"
using namespace json;

void JObject::insert(const std::string& name, JElementPtr value)
{
	m_Elements[name] = std::move(value);
}

bool JObject::contains(const std::string& key) const
{
	return m_Elements.find(key) != m_Elements.end();
}

bool JObject::contains_object(const std::string& key) const
{
	auto it = m_Elements.find(key);
	if (it == m_Elements.end())
	{
		return false;
	}

	return it->second->as_object() != nullptr;
}

bool json::JObject::contains_array(const std::string& key) const
{
	auto it = m_Elements.find(key);
	if (it == m_Elements.end())
	{
		return false;
	}

	return it->second->as_array() != nullptr;
}

std::string JObject::to_string(bool prettyPrint, int indentLevel) const
{
	std::string indentation = utility::calculate_indentation(prettyPrint, indentLevel);
	std::string objString = prettyPrint ? "{\n" : "{";
	for (auto i = m_Elements.begin(); i != m_Elements.end(); ++i)
	{
		objString += indentation + "\"" + i->first + "\" : " + i->second->to_string(prettyPrint, indentLevel + 1);
		if (std::next(i) != m_Elements.end())
		{
			objString += ",";
		}

		if (prettyPrint)
		{
			objString += "\n";
		}
	}

	return objString + utility::calculate_indentation(prettyPrint, indentLevel - 1) + "}";
}

JElementPtr JObject::copy() const
{
	auto c = std::make_unique<JObject>();
	for (const auto& p : m_Elements)
	{
		c->insert(p.first, p.second->copy());
	}

	return c;
}

JElementPtr& JObject::operator[](const std::string& key)
{
	return m_Elements[key];
}

const JElement* JObject::at(const std::string& key) const
{
	return m_Elements.at(key).get();
}

const JObject* JObject::object_at(const std::string& key) const
{
	return m_Elements.at(key)->as_object();
}
