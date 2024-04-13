#pragma once
#include <string>

namespace json {
	class LiteralValue;
	class JSONNullValue;
	class Object;
	class Array;

	class Element
	{
	public:
		Element() = default;
		virtual ~Element() = default;

		virtual std::string to_string(bool prettyPrint = false, int indentLevel = 0) = 0;
		virtual Element* copy() const = 0;

		Array*			as_array();
		Object*			as_object();
		LiteralValue*	as_literal();
	};

	class LiteralValue
		: public Element
	{
	public:
		enum class ValueType{
			String,
			Number,
			Boolean,
			Null,
		};

	private:
		std::string m_Value;
		ValueType m_Type;
		LiteralValue(ValueType type, const std::string& source);
	public:
		LiteralValue(LiteralValue&&) = delete;
		LiteralValue(const LiteralValue&) = delete;
		virtual ~LiteralValue() = default;

		static LiteralValue* create_str_value(const std::string& s);
		static LiteralValue* create_number_value(const double d);
		static LiteralValue* create_number_value(const int64_t i);
		static LiteralValue* create_boolean_value(const bool b);
		static LiteralValue* create_null_value();

		bool is_string();
		bool is_number();
		bool is_integer_number();
		bool is_boolean();
		bool is_null();

		int64_t					as_integer() const;
		double					as_fractional() const;
		bool					as_bool() const;
		const std::string&		as_str() const;

		void operator=(const char* newVal);
		void operator=(bool newVal);
		void operator=(int64_t newVal);
		void operator=(double newVal);

		// Inherited via JSONValue
		virtual Element* copy() const override;
		virtual std::string to_string(bool prettyPrint = false, int indentLevel = 0) override;
	};
}