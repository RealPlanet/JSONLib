#pragma once
#include "JSONValue.h"
#include <map>

namespace json {
	class Object
		: public Element
	{
		friend class JSON;
	private:
		std::map<std::string, Element*> _Elements;

	public:
		Object() = default;
		Object(Object&&) = delete;
		Object(const Object&) = delete;

		virtual ~Object() {
			for (auto ptr : _Elements)
				delete ptr.second;

			_Elements.clear();
		}

		void insert(const std::string& name, Element* value);

		Element* operator[](std::string key) { return this->_Elements[key]; }

		virtual std::string to_string(bool prettyPrint = false, int indentLevel = 0) override;
		virtual Element* copy() const override;
	};
}