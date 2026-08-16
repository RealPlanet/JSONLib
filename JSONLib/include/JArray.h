#pragma once

#include "JElement.h"
#include <vector>

namespace json {
	class JArray
		: public JElement
	{
		friend class JSON;
	private:
		std::vector<JElement*> m_Elements;

	public:
		JArray() = default;
		JArray(JArray&&) = delete;
		JArray(const JArray&) = delete;
		virtual ~JArray();

		void push_back(JElement* value);
		void clear();

		JElement* at(size_t index);
		size_t size() { return m_Elements.size(); }

		JElement* operator[](size_t index);
		auto begin() { return m_Elements.begin(); }
		auto end() { return m_Elements.end(); }
		auto begin() const { return m_Elements.cbegin(); }
		auto end() const { return m_Elements.cend(); }

		// Inherited from JValue
		virtual std::string to_string(bool prettyPrint = false, int indentLevel = 0) const override;
		virtual JElement* copy() const override;
	};
}