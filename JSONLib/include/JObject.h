#pragma once

#ifndef _H_JOBJECT_
#define _H_JOBJECT_

#include "JElement.h"
#include <unordered_map>
#include <string>

namespace json
{
	class JObject;
	using JObjectPtr = std::unique_ptr<JObject>;

	class JObject : public JElement
	{
		friend class JSON;

	public:
		JObject() = default;
		JObject(JObject&&) = default;
		JObject(const JObject&) = delete;
		~JObject() override = default;

		void insert(const std::string& name, JElementPtr value);
		bool contains(const std::string& key) const;
		bool contains_object(const std::string& key) const;
		bool contains_array(const std::string& key) const;

		size_t size() const
		{
			return m_Elements.size();
		}

		std::string to_string(bool prettyPrint = false, int indentLevel = 0) const override;
		JElementPtr copy() const override;

		// Operators
		JElementPtr& operator[](const std::string& key);
		const JElement* at(const std::string& key) const;
		const JObject* object_at(const std::string& key) const;

	private:
		std::unordered_map<std::string, JElementPtr> m_Elements;
	};
} // namespace json

#endif // !_H_JOBJECT_
