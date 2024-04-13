#pragma once
#include "JSONValue.h"
#include <vector>

namespace json {
	class Array
		: public Element
	{
		friend class JSON;
	private:
		std::vector<Element*> m_Elements;

	public:
		Array() = default;
		Array(Array&&) = delete;
		Array(const Array&) = delete;
		virtual ~Array();

		void push_back(Element* value);
		void clear();

		Element* at(size_t index);
		size_t size() { return m_Elements.size(); }

		Element* operator[](size_t index);

		// Inherited from JSONValue
		virtual std::string to_string(bool prettyPrint = false, int indentLevel = 0) override;
		virtual Element* copy() const override;
	};
}