#pragma once

#include <string>

#include "JElement.h"
#include "JValue.h"
#include "JArray.h"
#include "JObject.h"

namespace json {

	class JElement;
	class JSON {
	private:
		JElement* m_Root{nullptr};

	public:
		JSON() = default;
		JSON(JElement* root) : m_Root{ root } {}
		JSON(JSON&& m) noexcept;
		JSON(const JSON& m);
		virtual ~JSON();

		std::string to_string(bool prettyPrint = false) const;
		bool		is_valid() const;

		JElement* operator[](std::string key);
		JElement* at_path(const std::string& path, const std::string& separator = "///");
		const JElement* at_path(const std::string& path, const std::string& separator = "///") const;
		void take_ownership_of(JSON&& json);

		bool operator==(const JSON& other) const;
		bool operator!=(const JSON& other) const;
	};
}
