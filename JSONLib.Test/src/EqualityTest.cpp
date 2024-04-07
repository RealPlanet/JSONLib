#include "pch.h"
#include "JSON.h"
#include "JSONParser.h"

#define COUT_OFFSET "[ INFO     ] "

TEST(JSONComparison, ValueJSON) {
	std::string sample = "1";
	json::JSON a = json::Parser::from_text(sample);
	json::JSON b = json::Parser::from_text(sample);
	std::cout << COUT_OFFSET << a.to_string() << "\n";
	ASSERT_TRUE(a == b);
}

TEST(JSONComparison, EmptyObject) {
	std::string sample = "{ }";
	json::JSON a = json::Parser::from_text(sample);
	json::JSON b = json::Parser::from_text(sample);
	std::cout << COUT_OFFSET << a.to_string() << "\n";

	ASSERT_TRUE(a == b);
}

TEST(JSONComparison, EmptyObjectCopy) {
	std::string sample = "{ }";
	json::JSON a = json::Parser::from_text(sample);
	json::JSON b = a;
	std::cout << COUT_OFFSET << a.to_string() << "\n";

	ASSERT_TRUE(a == b);
}

TEST(JSONComparison, EmptyArray) {
	std::string sample = "[ ]";
	json::JSON a = json::Parser::from_text(sample);
	json::JSON b = json::Parser::from_text(sample);
	std::cout << COUT_OFFSET << a.to_string() << "\n";

	ASSERT_TRUE(a == b);
}

TEST(JSONComparison, EmptyArrayCopy) {
	std::string sample = "[]";
	json::JSON a = json::Parser::from_text(sample);
	json::JSON b = a;
	std::cout << COUT_OFFSET << a.to_string() << "\n";

	ASSERT_TRUE(a == b);
}