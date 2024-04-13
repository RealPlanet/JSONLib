#pragma once
#include "JSONValue.h"
#include <map>

namespace json {
	class Object
		: public Element
	{
		friend class JSON;
	private:
		std::map<std::string, Element*> m_Elements;

	public:
		Object() = default;
		Object(Object&&) = delete;
		Object(const Object&) = delete;
		virtual ~Object();

		void insert(const std::string& name, Element* value);
		bool contains(const std::string& key);
		size_t size() { return m_Elements.size(); }

		Element* operator[](std::string key) { return m_Elements[key]; }

		virtual std::string to_string(bool prettyPrint = false, int indentLevel = 0) override;
		virtual Element* copy() const override;
	};
}