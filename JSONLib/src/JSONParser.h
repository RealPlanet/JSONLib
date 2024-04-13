#pragma once

#include <string>

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

		struct DataIterator {
			std::string data;
			size_t offset;

			char read1() { return data[offset++]; }
			char peek(int _offset = 0)
			{ 
				if (offset < data.size())
					return data[offset + _offset];
				return EOF;
			}

			bool is_valid() { return peek() != EOF; }
			void skip_wspace();
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
		int hex2int(char c);
	public:
		static size_t MaxDepth;
		static JSON from_text(const std::string& text, std::vector<std::string>* errors);
		static JSON from_file(const std::string& path, std::vector<std::string>* errors);
		static JSON from_text(const std::string& text);
		static JSON from_file(const std::string& path);

		std::vector<std::string>&	get_errors();
		void						clear_errors();
	};
}
