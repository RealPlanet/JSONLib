#pragma once

#include "JElement.h"

#include <string>

namespace json {
	class JValue
		: public JElement
	{
	public:
		enum class ValueType {
			String,
			Number,
			Boolean,
			Null,
		};

	private:
		std::string m_Value;
		ValueType m_Type;
		JValue(ValueType type, const std::string& source);
	public:
		JValue(JValue&&) = delete;
		JValue(const JValue&) = delete;
		virtual ~JValue() = default;

		static JValue* create_str_value(const std::string& s);
		static JValue* create_number_value(const double d);
		static JValue* create_number_value(const int64_t i);
		static JValue* create_boolean_value(const bool b);
		static JValue* create_null_value();

		bool is_string() const;
		bool is_number() const;
		bool is_integer_number() const;
		bool is_boolean() const;
		bool is_null() const;

		int64_t					as_integer() const;
		double					as_fractional() const;
		bool					as_bool() const;
		const std::string& as_string() const;

		void operator=(const char* newVal);
		void operator=(bool newVal);
		void operator=(int64_t newVal);
		void operator=(double newVal);

		// Inherited via JValue
		virtual JElement* copy() const override;
		virtual std::string to_string(bool prettyPrint = false, int indentLevel = 0) const override;
	};
}