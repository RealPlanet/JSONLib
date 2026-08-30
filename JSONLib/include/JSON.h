#pragma once
#include "JArray.h"
#include "JElement.h"
#include "JObject.h"
#include "JValue.h"
#include <memory>
#include <string>

namespace json
{
	class JSON
	{
	private:
		std::unique_ptr<JElement> m_Root;

	public:
		JSON() = default;
		explicit JSON(std::unique_ptr<JElement> root)
			: m_Root(std::move(root))
		{
		}

		JSON(JSON&&) noexcept = default;
		JSON& operator=(JSON&&) noexcept = default;
		JSON(const JSON& m);
		JSON& operator=(const JSON& m);
		~JSON() = default;

		bool is_valid() const;
		bool is_object() const;
		bool is_array() const;

		JObject& as_object();
		const JObject& as_object() const;
		JArray& as_array();
		const JArray& as_array() const;

		std::string to_string(bool prettyPrint = false) const;

		JElement* at_path(const std::string& path, const std::string& separator = "///");
		const JElement* at_path(const std::string& path, const std::string& separator = "///") const;

		// Operators
		bool operator==(const JSON& other) const;
		bool operator!=(const JSON& other) const;
		JElement* operator[](std::string key);
	};

} // namespace json
