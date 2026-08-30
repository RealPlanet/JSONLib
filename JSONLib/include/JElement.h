#pragma once

#ifndef _H_JELEMENT_
#define _H_JELEMENT_

#include <memory>
#include <string>

namespace json
{
	class JValue;
	class JObject;
	class JArray;
	class JElement;

	using JElementPtr = std::unique_ptr<JElement>;

	class JElement
	{
	public:
		virtual ~JElement() = default;
		virtual std::string to_string(bool prettyPrint = false, int indentLevel = 0) const = 0;
		virtual JElementPtr copy() const = 0;

		JArray* as_array();
		const JArray* as_array() const;
		JObject* as_object();
		const JObject* as_object() const;
		JValue* as_literal();
		const JValue* as_literal() const;

		operator const std::string& () const;
		operator JObject* ();
		operator const JObject* () const;
		operator JArray* ();
		operator const JArray* () const;
		operator JValue* ();
		operator const JValue* () const;
	};
} // namespace json

#endif // !_H_JELEMENT_
