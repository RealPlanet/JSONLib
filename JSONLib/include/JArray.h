#pragma once

#ifndef _H_JARRAY_
#define _H_JARRAY_

#include "JElement.h"
#include <memory>
#include <vector>

namespace json
{
	class JArray : public JElement
	{
		friend class JSON;

	private:
		std::vector<JElementPtr> m_Elements;

	public:
		JArray() = default;
		JArray(JArray&&) = delete;
		JArray(const JArray&) = delete;
		virtual ~JArray() override = default;

		void push_back(JElementPtr value);
		void clear();
		JElement* at(size_t index);
		const JElement* at(size_t index) const;
		size_t size() const
		{
			return m_Elements.size();
		}

		auto begin()
		{
			return m_Elements.begin();
		}
		auto end()
		{
			return m_Elements.end();
		}
		auto begin() const
		{
			return m_Elements.cbegin();
		}
		auto end() const
		{
			return m_Elements.cend();
		}

		std::string to_string(bool prettyPrint = false, int indentLevel = 0) const override;
		JElementPtr copy() const override;

		// Operators
		JElement* operator[](size_t index);
		const JElement* operator[](size_t index) const;
	};
} // namespace json

#endif // !_H_JARRAY_
