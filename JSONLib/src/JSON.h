#pragma once

#include <string>
#include "JSONValue.h"
#include "JSONArray.h"
#include "JSONObject.h"

#define JSON_EXPECTED_NULL_ERR "Expected 'null' value but instead found %s at character %d"

namespace json {

	class Element;
	class JSON {
	private:
		Element* m_Root{nullptr};

	public:
		JSON() = default;
		JSON(Element* root) : m_Root{ root } {}
		JSON(JSON&& m) noexcept;
		JSON(const JSON& m);
		virtual ~JSON();

		std::string to_string(bool prettyPrint = false) const;
		bool		is_valid() const;

		Element* operator[](std::string key);
		Element* at_path(const std::string& path, const std::string& separator = "///");

		//JSONValue* operator[](size_t key);
		bool operator==(const JSON& other) const;
		bool operator!=(const JSON& other) const;
	};
}
