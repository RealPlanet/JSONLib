#pragma once

#ifndef _H_JVALUE_
#define _H_JVALUE_

#include <memory>
#include <string>

#include "JElement.h"

namespace json
{
	class JValue;

	using JValuePtr = std::unique_ptr<JValue>;

	class JValue : public JElement
	{

	public:
		enum class ValueType
		{
			String,
			Number,
			Boolean,
			Null
		};

	public:
		JValue(JValue&&) = delete;
		JValue(const JValue&) = delete;
		virtual ~JValue() override = default;

		static JValuePtr create_str_value(const std::string& s);
		static JValuePtr create_number_value(double d);
		static JValuePtr create_number_value(int64_t i);
		static JValuePtr create_boolean_value(bool b);
		static JValuePtr create_null_value();

		bool is_string() const;
		bool is_number() const;
		bool is_integer_number() const;
		bool is_boolean() const;
		bool is_null() const;

		int64_t as_integer() const;
		double as_fractional() const;
		bool as_bool() const;
		const std::string& as_string() const;

		void operator=(const char*);
		void operator=(bool);
		void operator=(int64_t);
		void operator=(double);

		JElementPtr copy() const override;
		std::string to_string(bool prettyPrint = false, int indentLevel = 0) const override;

	private:
		std::string m_Value;
		ValueType m_Type;
		JValue(ValueType type, const std::string& source);
	};
} // namespace json

#endif // !_H_JVALUE_
