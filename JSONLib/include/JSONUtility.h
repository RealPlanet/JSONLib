#pragma once
#include <istream>
#include <memory>
#include <string>
#include <stdexcept>
#include <sstream>

namespace json {
	namespace utility {
		inline std::string calculate_indentation(bool prettyPrint, int indentLevel)
		{
			if (!prettyPrint)
				return std::move(std::string(""));

			return std::move(std::string((size_t)4 * (size_t)indentLevel, ' '));
		}

		template<typename ... Args>
		inline std::string string_format(const std::string& format, Args ... args)
		{
			int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
			if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
			auto size = static_cast<size_t>(size_s);
			std::unique_ptr<char[]> buf(new char[size]);
			std::snprintf(buf.get(), size, format.c_str(), args ...);
			return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
		}

		inline bool isdigit(const char c) {
			if (c < 0 || c > 127)
				return false;

			return ::isdigit(c);
		}

		inline bool str2int(int& i, char const* s)
		{
			std::stringstream ss(s);
			ss >> i;
			if (ss.fail()) {
				// not an integer
				return false;
			}
			return true;
		}
	}
}