
#include "JSON.h"
#include "JSONValue.h"
#include "JSONArray.h"
#include "JSONObject.h"
#include "JSONUtility.h"

#include <fstream>
#include <cassert>
#include <sstream>
#include <iostream>

using namespace json;

JSON::JSON(JSON&& m) noexcept {
	m_Root = m.m_Root;
	m.m_Root = nullptr;
}

JSON::JSON(const JSON& m) {
	m_Root = m.m_Root->copy();
}

JSON::~JSON() {
	if (m_Root != nullptr) {
		delete m_Root;
	}
}

Element* JSON::operator[](std::string key) {
	Object* o = dynamic_cast<Object*>(m_Root);
	if (o != nullptr) {
		auto it = o->m_Elements.find(key);
		if (it != o->m_Elements.end()) {
			return it->second;
		}
	}

	return nullptr;
}

Element* JSON::at_path(const std::string& path, const std::string& separator) {
	std::vector<std::string> tokens;

	size_t index{ 0 };
	size_t prevIndex{ 0 };

	index = path.find(separator, prevIndex);
	while (index != std::string::npos) {
		tokens.push_back(path.substr(prevIndex, index - prevIndex));
		prevIndex += separator.size() + tokens[tokens.size() - 1].size();
		index = path.find(separator, prevIndex);
	}

	tokens.push_back(path.substr(prevIndex));

	Element* current{ m_Root };
	for (auto& a : tokens) {
		int arrIndex{ -1 };
		Array* arr = current->as_array();
		if (arr && utility::str2int(arrIndex, a.c_str())) {
			if (arrIndex < 0 || arr->size() <= arrIndex) {
				return nullptr;
			}

			current = (*arr)[arrIndex];
			continue;
		}

		Object* obj = current->as_object();
		if (obj && obj->contains(a)) {
			current = (*obj)[a];
			continue;
		}

		return nullptr;
	}

	return current;
}

bool JSON::operator!=(const JSON& other) const {
	return !(*this == other);
}

bool JSON::operator==(const JSON& other) const {
	std::string thisString = to_string(false);
	std::string otherString = other.to_string(false);
	return  thisString == otherString;
}

std::string JSON::to_string(bool prettyPrint) const {
	if (m_Root == nullptr) {
		return "";
	}

	return m_Root->to_string(prettyPrint, 1);
}

bool JSON::is_valid() const {
	return m_Root != nullptr;
}

