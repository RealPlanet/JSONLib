#pragma once
#include "JElement.h"

#include <map>
#include <string>

namespace json {
	class JObject
		: public JElement
	{
		friend class JSON;
	private:
		std::map<std::string, JElement*> m_Elements;

	public:
		JObject() = default;
		JObject(JObject&&) = delete;
		JObject(const JObject&) = delete;
		virtual ~JObject();

		void insert(const std::string& name, JElement* value);
		bool contains(const std::string& key);
		size_t size() { return m_Elements.size(); }

		JElement* operator[](const std::string& key) { return m_Elements.at(key); }
		const JElement* operator[](const std::string& key) const { return m_Elements.at(key); }

		virtual std::string to_string(bool prettyPrint = false, int indentLevel = 0) const override;
		virtual JElement* copy() const override;
	};
}