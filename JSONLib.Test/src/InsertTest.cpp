#include "pch.h"
#include "JSON.h"
#include "JSONParser.h"
#define COUT_OFFSET "[ INFO     ] "

TEST(JSONInsert, JsonAddItemObject) {
	std::string sample = R"({
		"a" : true,
		"b" : [ "c", "d" ]
 })";

	std::string finalStruct = R"({
		"a" : true,
		"b" : [ "c", "d" , ["added", true	] ]
 })";

	json::JSON a = json::Parser::from_text(sample);
	json::JSON originalJson = json::Parser::from_text(sample);
	json::JSON b = json::Parser::from_text(finalStruct);

	auto newArray = new json::Array();
	a["b"]->as_array()->push_back(newArray);

	newArray->push_back(json::LiteralValue::create_str_value("added"));
	newArray->push_back(json::LiteralValue::create_boolean_value(true));
	std::cout << COUT_OFFSET << a.to_string() << "\n";
	std::cout << COUT_OFFSET << originalJson.to_string() << "\n";

	ASSERT_TRUE(a == b);
	ASSERT_TRUE(a != originalJson);
}