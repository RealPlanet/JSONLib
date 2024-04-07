#include "pch.h"
#include "JSON.h"
#include "JSONParser.h"
#include "JSONUtility.h"

void print_errors(std::vector<std::string>& e, int expectedCount)
{
	ASSERT_TRUE(e.size() == expectedCount);
	for (auto& s : e)
		std::cout << s << "\n";
}

TEST(JSONErrors, JsonExpectedNull) {
	std::string sample = R"({"a" : notnull})";

	std::vector<std::string> report;
	auto json = json::Parser::from_text(sample, &report);


	ASSERT_EQ(report.size(), 1);

	std::stringstream ss(report[0]);

	std::string err(999, '\0');
	std::string preview(999, '\0');
	ss.getline(&err[0], 999);
	ss.getline(&preview[0], 999);
	err.resize(strlen(err.c_str()));
	preview.resize(strlen(preview.c_str()));

	print_errors(report, 1);
	// Error index is at 7
	ASSERT_TRUE(err[err.size() - 3] == '7');
	ASSERT_TRUE(preview == sample);
}

TEST(JSONErrors, JsonStopAtFirstErr)
{
	std::string sample = R"({"a" : notnull, "b" : [1 , ]})";

	std::vector<std::string> report;
	auto json = json::Parser::from_text(sample, &report);
	print_errors(report, 1);
}

TEST(JSONErrors, JsonBadArray)
{
	std::string sample = R"({"a" : null, "b" : [1 , ]})";

	std::vector<std::string> report;
	auto json = json::Parser::from_text(sample, &report);

	print_errors(report, 1);
}