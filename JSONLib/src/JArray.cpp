#include "JArray.h"
#include "JUtility.h"

using namespace json;

JArray::~JArray() {
	clear();
}

void JArray::push_back(JElement* value) {
	m_Elements.push_back(value);
}

void JArray::clear() {
	for (auto ptr : m_Elements) {
		delete ptr;
	}

	m_Elements.clear();
}

JElement* JArray::operator[](size_t index) {
	return m_Elements[index];
}

JElement* JArray::at(size_t index) {
	return operator[](index);
}

std::string JArray::to_string(bool prettyPrint /*= false*/, int indentLevel /*= 0*/) const {
	std::string indentation = ::utility::calculate_indentation(prettyPrint, indentLevel);

	std::string result = prettyPrint ? "[\n" : "[";
	for (size_t i{ 0 }; i < m_Elements.size(); i++) {
		result += indentation + m_Elements[i]->to_string(prettyPrint, indentLevel + 1);
		if (i + 1 < m_Elements.size()) {
			result += ", ";
		}

		if (prettyPrint) {
			result += "\n";
		}
	}

	result += ::utility::calculate_indentation(prettyPrint, indentLevel - 1) + "] ";
	return result;
}

JElement* JArray::copy() const {
	JArray* c = new JArray();
	for (JElement* p : m_Elements) {
		c->push_back(p->copy());
	}

	return c;
}