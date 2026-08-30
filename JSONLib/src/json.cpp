#include "JSON.h"
#include "JUtility.h"
using namespace json;

JSON::JSON(const JSON& m)
{
	if (m.m_Root)
	{
		m_Root = m.m_Root->copy();
	}
}

JSON& JSON::operator=(const JSON& m)
{
	if (this != &m)
	{
		m_Root = m.m_Root ? m.m_Root->copy() : nullptr;
	}

	return *this;
}

JElement* JSON::operator[](std::string key)
{
	auto* o = m_Root ? m_Root->as_object() : nullptr;
	if (!o)
	{
		return nullptr;
	}
	auto it = o->m_Elements.find(key);

	return it != o->m_Elements.end() ? it->second.get() : nullptr;
}

static JElement* get_from_node_path(JElement* element, const std::string& path, const std::string& separator)
{
	if (!element)
	{
		return nullptr;
	}

	std::vector<std::string> tokens;
	size_t prev = 0, index = path.find(separator);
	while (index != std::string::npos)
	{
		tokens.push_back(path.substr(prev, index - prev));
		prev = index + separator.size();
		index = path.find(separator, prev);
	}

	tokens.push_back(path.substr(prev));
	JElement* current = element;
	for (auto& a : tokens)
	{
		int arrIndex = -1;
		if (auto* arr = current->as_array(); arr && utility::str2int(arrIndex, a.c_str()))
		{
			if (arrIndex < 0 || arr->size() <= static_cast<size_t>(arrIndex))
				return nullptr;
			current = (*arr)[arrIndex];
			continue;
		}

		if (auto* obj = current->as_object(); obj && obj->contains(a))
		{
			current = (*obj)[a].get();
			continue;
		}
		return nullptr;
	}
	return current;
}

JElement* JSON::at_path(const std::string& path, const std::string& separator)
{
	return get_from_node_path(m_Root.get(), path, separator);
}

const JElement* JSON::at_path(const std::string& path, const std::string& separator) const
{
	return get_from_node_path(m_Root.get(), path, separator);
}

bool JSON::operator!=(const JSON& other) const
{
	return !(*this == other);
}

bool JSON::operator==(const JSON& other) const
{
	return to_string(false) == other.to_string(false);
}

std::string JSON::to_string(bool prettyPrint) const
{
	return m_Root ? m_Root->to_string(prettyPrint, 1) : "";
}

bool JSON::is_valid() const
{
	return m_Root != nullptr;
}

bool json::JSON::is_object() const
{
	return is_valid() && m_Root->as_object() != nullptr;
}

bool json::JSON::is_array() const
{
	return is_valid() && m_Root->as_array() != nullptr;
}

JObject& json::JSON::as_object()
{
	if (!is_object())
	{
		throw std::runtime_error("JSON is not object");
	}

	return dynamic_cast<JObject&>(*m_Root);
}

const JObject& json::JSON::as_object() const
{
	if (!is_object())
	{
		throw std::runtime_error("JSON is not object");
	}

	return dynamic_cast<JObject&>(*m_Root);
}

JArray& json::JSON::as_array()
{
	if (!is_array())
	{
		throw std::runtime_error("JSON is not array");
	}

	return dynamic_cast<JArray&>(*m_Root);
}

const JArray& json::JSON::as_array() const
{
	if (!is_array())
	{
		throw std::runtime_error("JSON is not array");
	}

	return dynamic_cast<JArray&>(*m_Root);
}
