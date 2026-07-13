#pragma once

#include <string>
#include <span>

#include "JSON.h"
#include "JSONValue.h"
#include "JSONArray.h"
#include "JSONObject.h"

namespace json {
	class Parser {
	private:
		enum ErrType {
			Unknown = 0,
			ExpectedNull,
			ExpectedBool,

			UnexpectedCharacterInFile,

			UnexpectedEOF,
			UnexpectedComma,
			UnexpectedCharTerminator,
			UnexpectedNewline,
			UnexpectedTabCharacter,

			ExpectedStartOfArray,
			ExpectedStartOfObject,
			ExpectedEndOfArray,
			ExpectedEndOfObject,
			ExpectedColonMarker,
			ExpectedUTFCharacter,
			UnexpectedEscapeCharacter,

			ExpectedQuotes,
			LeftoverCharactersInData,

			// Number errors
			NotANumber,
			NumberCannotStartWithZero,
			NumberCannotEndWithDecimalSeparator,
			NumberCannotEndWithExponentialCharacter,
			NumberCannotEndWithSign,

			MaxJSONDepthReached,

			NoError,
		};

		struct DataIterator
		{
			explicit DataIterator(std::span<const uint8_t> d) : data(d), offset{ 0 } {}

			char read1() { return static_cast<char>(data[offset++]); }
			bool is_valid() const { return offset < data.size(); }
			size_t remaining() const { return data.size() - offset; }
			void skip_wspace();

			char peek(size_t lookahead = 0) const
			{
				if (offset + lookahead >= data.size())
					return EOF;

				return static_cast<char>(data[offset + lookahead]);
			}

			size_t offset;
			std::span<const uint8_t> data;
		};

		// Since we stop at first error this can be a single string
		std::vector<std::string> m_Errors;
		size_t m_Depth{ 0 };

		Parser() = default;

		Element* parse_json_value(DataIterator& it);
		Element* parse_json_array(DataIterator& it);
		Element* parse_json_object(DataIterator& it);

		Element* parse_boolean(DataIterator& it);
		Element* parse_number(DataIterator& it);
		Parser::ErrType is_valid_json_number(const std::string& value);

		int parse_string(DataIterator& it, std::string& result);
		/// <returns>Tuple: 0 - Member Name; 1 - Json Element Pointer</returns>
		std::tuple<std::string, Element*> parse_member(DataIterator& it);
		void build_error(ErrType type, DataIterator& data);
		std::string get_err_template(ErrType type);
		bool get_escaped_character(DataIterator& it, std::string& escapedCharacters);
		bool add_if_hex(char*, int, char);
	public:
		static size_t MaxDepth;
		static JSON from_text(const std::string& text, std::vector<std::string>* errors);
		static JSON from_file(const std::string& path, std::vector<std::string>* errors);
		static JSON from_stream(std::istream& stream, std::vector<std::string>* errors);
		static JSON from_memory(std::vector<uint8_t>& stream, std::vector<std::string>* errors);
		static JSON from_iterator(DataIterator& stream, std::vector<std::string>* errors);

		static JSON from_text(const std::string& text);
		static JSON from_file(const std::string& path);
		static JSON from_stream(std::istream& stream);
		static JSON from_memory(std::vector<uint8_t>& stream);

		std::vector<std::string>& get_errors();
		void						clear_errors();
	};
}
