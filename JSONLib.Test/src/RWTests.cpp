#include "pch.h"
#include "JSON.h"
#include "JSONParser.h"

TEST(JSONReadWrite, JsonAccess) {
	std::string sample = R"({
		"a" : true,
		"b" : [ "c", "d" ]
 })";

	json::JSON a = json::Parser::from_text(sample);

	ASSERT_TRUE(a["a"]->as_literal()->as_bool());
	ASSERT_TRUE(a["b"]->as_array()->size() == 2);

	ASSERT_TRUE(a["b"]->as_array()->at(0)->as_literal()->as_str() == "c");
	ASSERT_TRUE(a["b"]->as_array()->at(1)->as_literal()->as_str() == "d");
}

TEST(JSONReadWrite, JsonEdit) {
	std::string sample = R"({
		"a" : true,
		"b" : [ "c", "d" ]
 })";

	json::JSON a = json::Parser::from_text(sample);
	json::JSON original = json::Parser::from_text(sample);

	ASSERT_TRUE(a["a"]->as_literal()->as_bool());
	ASSERT_TRUE(a["b"]->as_array()->size() == 2);

	ASSERT_TRUE(a["b"]->as_array()->at(0)->as_literal()->as_str() == "c");
	ASSERT_TRUE(a["b"]->as_array()->at(1)->as_literal()->as_str() == "d");

	*a["b"]->as_array()->at(0)->as_literal() = "EDITED";

	ASSERT_FALSE(a == original);
	ASSERT_TRUE(a["b"]->as_array()->at(0)->as_literal()->as_str() == "EDITED");
}