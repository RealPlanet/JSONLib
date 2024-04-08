#include "JSONParser.h"
#include "JSONUtility.h"
#include "JSON.h"

#include <fstream>
#include <cassert>
#include <locale>
#include <codecvt>

#include <cuchar>

using namespace json;

void Parser::DataIterator::skip_wspace()
{
	while (is_valid())
	{
		if (!iswspace(peek())) {
			return;
		}

		offset++;
	}

}

JSON json::Parser::from_text(const std::string& text)
{
	return from_text(text, nullptr);
}

JSON Parser::from_text(const std::string& text, std::vector<std::string>* errors)
{
	Parser p;
	DataIterator it{};
	it.data = text;
	it.offset = 0;

	auto element = p.parse_json_value(it);
	it.skip_wspace();

	if (it.offset != it.data.size() && p.m_Errors.size() == 0)
	{	
		p.build_error(Parser::LeftoverCharactersInData, it);
		delete element;
		element = nullptr;
	}

	if (errors)
	{
		*errors = p.m_Errors;
	}

#ifdef _DEBUG
	if (p.m_Errors.size() != 0)
	{
		assert(element == nullptr);
	}
#endif // DEBUG


	return element;
}

JSON Parser::from_file(const std::string& path)
{
	return from_file(path, nullptr);
}

JSON Parser::from_file(const std::string& path, std::vector<std::string>* errors)
{
	Parser p;

	std::ifstream file;
	file.open(path);

	file.ignore(std::numeric_limits<std::streamsize>::max());
	const size_t byte_count = static_cast<size_t>(file.gcount());
	file.clear();   //  Since ignore will have set eof.
	file.seekg(0, std::ios_base::beg);

	DataIterator it{};
	it.data.resize(byte_count);
	it.offset = 0;

	char* first = const_cast<char*>(it.data.c_str());
	file.read(&first[0], it.data.size());
	file.close();

	auto element = p.parse_json_value(it);
	it.skip_wspace();
	if (it.offset != it.data.size() && p.m_Errors.size() == 0)
	{
		p.build_error(Parser::LeftoverCharactersInData, it);
		delete element;
		element = nullptr;
	}

	if (errors)
	{
		*errors = p.m_Errors;
	}

#ifdef _DEBUG
	if (p.m_Errors.size() != 0)
	{
		assert(element == nullptr);
	}
#endif // DEBUG

	return std::move(element);
}

std::vector<std::string>& Parser::get_errors()
{
	return m_Errors;
}

void Parser::clear_errors()
{
	m_Errors.clear();
}

Element* Parser::parse_json_value(DataIterator& it)
{
	while (it.is_valid())
	{
		it.skip_wspace();
		switch (it.peek())
		{
		case '[':
		{
			// Parse array
			return parse_json_array(it);
		}
		case '{':
		{
			// Parse object
			return parse_json_object(it);
		}
		case '"':
		{
			// Parse string
			std::string s;
			int result = parse_string(it, s);
			if (result != 0)
				return nullptr;

			return json::LiteralValue::create_str_value(s);
		}
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '-':
		{
			// Parse number
			return parse_number(it);
		}
		case 't':
		case 'f':
		{
			return parse_boolean(it);
		}
		case 'n':
		{
			if (strncmp(&it.data[it.offset], "null", 4) == 0)
			{
				it.offset += 4;
				return json::LiteralValue::create_null_value();
			}
			build_error(ErrType::ExpectedNull, it);
			it.offset = it.data.size();
			break;
		}
		default:
			build_error(Parser::UnexpectedCharacterInFile, it);
			it.offset = it.data.size();
			break;
		}
	}

	if (it.offset == it.data.size() && m_Errors.size() == 0)
	{
		build_error(Parser::UnexpectedEOF, it);
	}

	return nullptr;
}

Element* Parser::parse_json_array(DataIterator& it)
{
	if (it.peek() != '[')
	{
		build_error(Parser::ExpectedStartOfArray, it);
		return nullptr;
	}

	it.read1();
	std::unique_ptr<Array> result = std::make_unique<Array>();
	while (it.is_valid() && it.peek() != ']')
	{
		it.skip_wspace();
		Element* val = parse_json_value(it);
		if (!val)
		{
			return nullptr;
		}

		result->push_back(val);

		it.skip_wspace();
		if (it.peek() != ',')
			break;

		it.read1();

		int last_comma_index = it.offset;
		it.skip_wspace();

		if (it.peek() == ']') {
			build_error(Parser::UnexpectedComma, it);
			return nullptr;
		}
	}

	it.skip_wspace();
	if (it.peek() != ']')
	{
		build_error(Parser::ExpectedEndOfArray, it);
		return nullptr;
	}
	it.read1();

	return result.release();
}

Element* Parser::parse_json_object(DataIterator& it)
{
	if (it.peek() != '{')
	{
		build_error(Parser::ExpectedStartOfObject, it);
		return nullptr;
	}

	it.read1();
	it.skip_wspace();

	std::unique_ptr<Object> result = std::make_unique<Object>();
	bool has_read_comma = false;
	int last_comma_index = -1;
	while (it.is_valid() && it.peek() != '}')
	{
		it.skip_wspace();
		auto tuple = parse_member(it);
		auto ptr = std::get<1>(tuple);
		if (!ptr)
		{
			return nullptr;
		}

		has_read_comma = false;
		result->insert(std::get<0>(tuple), ptr);
		it.skip_wspace();

		if (it.peek() != ',')
			break;

		last_comma_index = it.offset;
		has_read_comma = true;
		it.read1();
	}

	if (has_read_comma)
	{
		it.offset = last_comma_index;
		build_error(Parser::UnexpectedComma, it);
		return nullptr;
	}

	it.skip_wspace();
	if (it.peek() != '}')
	{
		build_error(Parser::ExpectedEndOfObject, it);
		return nullptr;
	}
	it.read1();

	return result.release();
}

Element* Parser::parse_boolean(DataIterator& it)
{
	std::string boolValue;

	it.skip_wspace();
	if (it.peek() == 't') {
		if (strncmp(&it.data[it.offset], "true", 4) == 0)
		{
			it.offset += 4;
			return json::LiteralValue::create_boolean_value(true);
		}
	}

	if (it.peek() == 'f') {
		if (strncmp(&it.data[it.offset], "false", 5) == 0)
		{
			it.offset += 5;
			return json::LiteralValue::create_boolean_value(false);
		}

	}

	build_error(Parser::ExpectedBool, it);
	return nullptr;
}
Element* Parser::parse_number(DataIterator& it)
{
	std::string number = "";

	bool isFractional = false;
	it.skip_wspace();
	while (it.is_valid()) {
		char c = it.peek();

		if (c < 0)
		{
			build_error(Parser::ErrType::UnexpectedCharacterInNumber, it);
			return nullptr;
		}

		bool isDigit = isdigit(c);

		if (!isDigit && c != '-' && c != '.' && c != 'e')
		{
			break;
		}

		it.read1();
		number += c;
		isFractional = isFractional || c == '.' || c == 'e';
	}

	Parser::ErrType error = is_valid_json_number(number);

	if (error != Parser::ErrType::NoError) {
		build_error(error, it);
		return nullptr;
	}

	if (isFractional)
	{
		return json::LiteralValue::create_number_value(std::stod(number));
	}

	return json::LiteralValue::create_number_value((int64_t)std::stoi(number));
}

Parser::ErrType json::Parser::is_valid_json_number(const std::string& value)
{
	int strLen = value.size();
	if (strLen == 0)
		return Parser::ErrType::NotANumber;

	if (strLen == 1)
	{
		// Number can only contain these characters but not start with these
		if (value[0] == '.' || value[0] == '-' || value[0] == 'e')
		{
			return Parser::ErrType::NotANumber;
		}
	}

	char lastChar = value[value.size() - 1];
	if(lastChar == 'e')
		return Parser::ErrType::NumberCannotEndWithExponentialCharacter;

	if (lastChar == '-')
		return Parser::ErrType::NumberCannotEndWithMinus;

	if (value[strLen - 1] == '.')
		return Parser::ErrType::NumberCannotEndWithDecimalSeparator;


	// Value cannot start with a zero
	int offset = value[0] == '-' ? 1 : 0;

	// If value is zero and there are more characters in the number
	// then it is an error
	if (value[offset] == '0' && offset + 1 < value.size())
		return Parser::ErrType::NumberCannotStartWithZero;

	if (value[offset] == '.')
		return Parser::ErrType::NumberHasNoRealPart;

	if (utility::contains_multiple(value, '.'))
	{
		return Parser::ErrType::MultipleDecimalSeparators;
	}

	if (utility::contains_multiple(value, '.'))
	{
		return Parser::ErrType::MultipleExponentialSymbols;
	}

	return Parser::ErrType::NoError;
}

std::string Parser::get_escaped_character(DataIterator& it)
{
	it.read1(); // Consume escape char
	switch (it.peek())
	{
	case '\\':
		it.read1();
		return "\\";
	case '/':
		it.read1();
		return "/";
	case 'b':
		it.read1();
		return "\b";
	case 'f':
		it.read1();
		return "\f";
	case 'n':
		it.read1();
		return "\n";
	case 'r':
		it.read1();
		return "\r";
	case 't':
		it.read1();
		return "\t";
	case '"':
		it.read1();
		return "\"";
	case 'u':
	{
		it.offset++;
		int startingCodepointPosition = it.offset;

		int i{ 0 };
		char hex[5]{ 0 };

		for (int j{ 0 }; j < 4; j++)
		{
			if (!add_if_hex(hex, i++, it.peek())) {
				build_error(json::Parser::ExpectedUTFCharacter, it);
				return "";
			}

			it.offset++;
		}

		// Convert the char to utf8
		std::string utfChars = hex;
		char32_t c = std::stoul(utfChars, nullptr, 16);
		// Split the actual UTF8 Character into bytes
		std::mbstate_t state;
		auto len = std::c32rtomb(hex, c, &state);
		if (len == std::size_t(-1)) {
			//build_error(json::Parser::CodepointNotRecognized, it, startingCodepointPosition);
			return std::string(hex);
		}

		// Null terminate it and send it back up
		hex[len] = 0;
		return std::string(hex);
	}
	default:
		build_error(Parser::UnexpectedEscapeCharacter, it);
	}
	return "";
}

int json::Parser::hex2int(char ch)
{
	if (ch >= '0' && ch <= '9')
		return ch - '0';
	if (ch >= 'A' && ch <= 'F')
		return ch - 'A' + 10;
	if (ch >= 'a' && ch <= 'f')
		return ch - 'a' + 10;
	return -1;
}

bool json::Parser::add_if_hex(char* arr, int index, char c)
{
	if (c < 0)
	{
		// Invalid data
		return false;
	}

	if (c != 'A' &&
		c != 'B' &&
		c != 'C' &&
		c != 'D' &&
		c != 'E' &&
		c != 'F' &&
		!std::isdigit(c))
	{
		return false;
	}

	arr[index] = c;// hex2int(c);
	return true;
}

int Parser::parse_string(DataIterator& it, std::string& result)
{
	if (it.peek() != '"')
	{
		build_error(Parser::ExpectedQuotes, it);
		return -1;
	}
	it.read1();

	while (it.peek() != '"') {

		if (!it.is_valid())
		{
			build_error(json::Parser::UnexpectedEOF, it);
			return -1;
		}

		if (it.peek() == '\\')
		{
			std::string escapedCharacters = get_escaped_character(it);
			if (escapedCharacters == "")
				return -1;

			result += escapedCharacters;
			continue;
		}

		if (it.peek() == 0)
		{
			build_error(json::Parser::UnexpectedCharTerminator, it);
			return -1;
		}

		if (it.peek() == '\n' ||
			it.peek() == '\r')
		{
			build_error(json::Parser::UnexpectedNewline, it);
			return -1;
		}

		if (it.peek() == '\t')
		{
			build_error(json::Parser::UnexpectedTabCharacter, it);
			return -1;
		}

		result += it.read1();
	}

	if (it.peek() != '"')
	{
		build_error(Parser::ExpectedQuotes, it);
		return -1;
	}
	it.read1();
	return 0;
}

std::tuple<std::string, Element*> Parser::parse_member(DataIterator& it)
{

	std::string memberName;
	int result = parse_string(it, memberName);
	if (result != 0)
	{
		return std::make_tuple("", nullptr);
	}

	it.skip_wspace();
	if (it.peek() != ':')
	{
		build_error(Parser::ExpectedColonMarker, it);
		return std::make_tuple(memberName, nullptr);
	}

	it.read1();
	return std::make_tuple(memberName, parse_json_value(it));
}

void Parser::build_error(ErrType type, DataIterator& data, int charIndex)
{
	std::string templ = get_err_template(type);

	if (charIndex < 0)
		charIndex = data.offset;

	size_t startIndex = charIndex;
	size_t endIndex = charIndex;

	while (
		(startIndex > 0 && startIndex < data.data.size() &&
			(data.data[startIndex] != '\n' || data.data[startIndex] != '\r' || startIndex > charIndex - 15)))
	{
		startIndex--;
	}

	while (endIndex < data.data.size() &&
		(data.data[endIndex] != '\n' || data.data[endIndex] != '\r' || endIndex < charIndex + 15))
	{
		endIndex++;
	}

	auto cString = data.data;
	std::string preview(&cString[startIndex], &cString[endIndex]);
	std::string errMarker = '\n' + std::string(charIndex - startIndex, ' ') + '^';
	preview += errMarker;
	templ = json::utility::string_format(templ, charIndex);
	m_Errors.push_back(templ + preview);
}

std::string json::Parser::get_err_template(ErrType type)
{
	switch (type) {
	default:
	case ErrType::Unknown:
		return "Unexpected error at '%d':\n";
	case ErrType::ExpectedNull:
		return "Expected 'null' keyword at '%d':\n";
	case ErrType::ExpectedBool:
		return "Expected 'true' or 'false' keyword while parsing boolean at '%d':\n";
	case ErrType::UnexpectedCharacterInNumber:
		return "Unexpected character in number at '%d':\n";
	case ErrType::UnexpectedCharacterInFile:
		return "Unexpected character at '%d':\n";
	case ErrType::UnexpectedEOF:
		return "Unexpected EOF at '%d':\n";
	case ErrType::UnexpectedCharTerminator:
		return "Unexpected 0 Terminator at '%d':\n";
	case ErrType::UnexpectedNewline:
		return "Unexpected unexcaped newline at '%d':\n";
	case ErrType::UnexpectedTabCharacter:
		return "Unexpected unexcaped TAB character at '%d':\n";
	case ErrType::UnexpectedComma:
		return "Unexpected trailing comma (',') at '%d' (Did you forget a value?):\n";
	case ErrType::ExpectedStartOfArray:
		return "Unexpected start of Array at '%d':\n";
	case ErrType::ExpectedStartOfObject:
		return "Unexpected start of Object at '%d':\n";
	case ErrType::ExpectedEndOfArray:
		return "Expected end of Array at '%d', did you forget a comma?:\n";
	case ErrType::ExpectedEndOfObject:
		return "Unexpected end of Object at '%d':\n";
	case ErrType::ExpectedColonMarker:
		return "Expected colon (':') at '%d':\n";
	case ErrType::ExpectedQuotes:
		return "Expected double quotes ('\"') at '%d':\n";
	case ErrType::LeftoverCharactersInData:
		return "Leftover characters at '%d' after parsing JSON:\n";
	case ErrType::NotANumber:
		return "Value is not a valid number at '%d':\n";
	case ErrType::NumberCannotStartWithZero:
		return "Number at '%d' starts with 0, leading 0s are not allowed:\n";
	case ErrType::MultipleDecimalSeparators:
		return "Number contains multiple decimal separators '.', first occurence at '%d':\n";
	case ErrType::MultipleExponentialSymbols:
		return "Number contains multiple exponential symbols 'e', first occurence at '%d':\n";
	case ErrType::NumberCannotEndWithMinus:
		return "Number cannot end with '-', error at '%d':\n";
	case ErrType::NumberCannotEndWithExponentialCharacter:
		return "Number cannot end with 'e', error at '%d':\n";
	case ErrType::NumberCannotEndWithDecimalSeparator:
		return "Number ends with decimal separator '.' at '%d':\n";
	case ErrType::NumberHasNoRealPart:
		return "Number must have a real part, unable to parse number at ''%d'':\n";
	case ErrType::ExpectedUTFCharacter:
		return "Character at '%d' is not a HEX digit!\n";
	case ErrType::UnexpectedEscapeCharacter:
		return "Unexpected escape character at '%d', it is not a valid escape character!\n";
	}
}


