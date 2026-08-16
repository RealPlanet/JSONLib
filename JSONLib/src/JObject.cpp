#include "JObject.h"
#include "JUtility.h"

using namespace json;

JObject::~JObject() {
	for (auto& ptr : m_Elements) {
		delete ptr.second;
	}

	m_Elements.clear();
}

void JObject::insert(const std::string& name, JElement* value) {
	m_Elements.insert(std::make_pair(name, value));
}

bool JObject::contains(const std::string& key){
	return m_Elements.find(key) != m_Elements.end();
}

std::string JObject::to_string(bool prettyPrint /*= false*/, int indentLevel /*= 0*/) const {
	std::string indentation = utility::calculate_indentation(prettyPrint, indentLevel);
	std::string objString = prettyPrint ? "{\n" : "{";
	for (auto i{ m_Elements.begin() }; i != m_Elements.end(); i++) {
		objString += indentation + "\"" + i->first + "\" : " + i->second->to_string(prettyPrint, indentLevel + 1);

		if (std::next(i) != m_Elements.end()) {
			objString += ",";
		}


		if (prettyPrint) {
			objString += "\n";
		}
	}

	objString += utility::calculate_indentation(prettyPrint, indentLevel - 1) + "}";
	return objString;
}

JElement* JObject::copy() const {
	JObject* c = new JObject();
	for (auto p : m_Elements) {
		c->insert(p.first, p.second->copy());
	}

	return c;
}