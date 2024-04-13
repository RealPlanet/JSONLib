#include "Tests.h"

#define PRINT_TEST_NAME std::cout << "Running test:" << __FUNCTION__ << "\n"

bool json::test::are_values_equal() {
	PRINT_TEST_NAME;

	std::string sample = "1";
	json::JSON a = json::Parser::from_text(sample);
	json::JSON b = json::Parser::from_text(sample);
	std::cout << a.to_string() << "\n";
	return(a == b);
}

bool json::test::are_empty_objects_equal() {
	PRINT_TEST_NAME;

	std::string sample = "{ }";
	json::JSON a = json::Parser::from_text(sample);
	json::JSON b = json::Parser::from_text(sample);
	std::cout << a.to_string() << "\n";

	return(a == b);
}

bool json::test::are_empty_copied_objects_equal()
{
	PRINT_TEST_NAME;

	std::string sample = "{ }";
	json::JSON a = json::Parser::from_text(sample);
	json::JSON b = a;
	std::cout << a.to_string() << "\n";

	return(a == b);
}

bool json::test::are_empty_arrays_equal() {
	PRINT_TEST_NAME;

	std::string sample = "[ ]";
	json::JSON a = json::Parser::from_text(sample);
	json::JSON b = json::Parser::from_text(sample);
	std::cout << a.to_string() << "\n";

	return(a == b);
}

bool json::test::are_empty_copied_arrays_equal() {
	PRINT_TEST_NAME;

	std::string sample = "[]";
	json::JSON a = json::Parser::from_text(sample);
	json::JSON b = a;
	std::cout << a.to_string() << "\n";

	return(a == b);
}

bool json::test::can_mixed_objs_and_arrays_be_accessed() {
	PRINT_TEST_NAME;
	std::string sample = R"({
		"a" : true,
		"b" : [ "c", "d" ]
 })";

	json::JSON a = json::Parser::from_text(sample);

	if (!a["a"]->as_literal()->as_bool()) {
		return false;
	}

	if (a["b"]->as_array()->size() != 2) {
		return false;
	}

	if (a["b"]->as_array()->at(0)->as_literal()->as_str() != "c") {
		return false;
	}

	if (a["b"]->as_array()->at(1)->as_literal()->as_str() != "d") {
		return false;
	}

	return true;
}

bool json::test::can_access_edited_field_and_is_updated() {
	PRINT_TEST_NAME;
	std::string sample = R"({
		"a" : true,
		"b" : [ "c", "d" ]
 })";

	json::JSON a = json::Parser::from_text(sample);
	json::JSON original = json::Parser::from_text(sample);

	if (!a["a"]->as_literal()->as_bool()) {
		return false;
	}

	if (a["b"]->as_array()->size() != 2) {
		return false;
	}

	if (a["b"]->as_array()->at(0)->as_literal()->as_str() != "c") {
		return false;
	}

	if (a["b"]->as_array()->at(1)->as_literal()->as_str() != "d") {
		return false;
	}

	*a["b"]->as_array()->at(0)->as_literal() = "EDITED";

	if (a == original) {
		return false;
	}

	if (a["b"]->as_array()->at(0)->as_literal()->as_str() != "EDITED") {
		return false;
	}

	return true;
}

bool json::test::can_add_to_json()
{
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
	std::cout << a.to_string() << "\n";
	std::cout << originalJson.to_string() << "\n";

	if (a != b) {
		return false;
	}

	return(a != originalJson);
}

bool json::test::json_path_search_works() {
	PRINT_TEST_NAME;

	std::string sample = R"({ "a" : true, "b" : [ {	"bb" : "CIAO" }, 1 ] })";

	json::JSON a = json::Parser::from_text(sample);

	json::Element* ciaoValue = a.at_path("b///0///bb");

	if (!ciaoValue) {
		return false;
	}

	return ciaoValue->as_literal()->as_str() == "CIAO";
}

void json::test::print_report(const std::vector<std::string>& err)
{
	for (auto& s : err)
		std::cout << s << "\n";
}

bool json::test::run_parse_test_from_file(std::filesystem::directory_entry p)

{
	std::cout << "> " << p.path().filename().string() << "...";

	std::vector<std::string> report;

	auto json = json::Parser::from_file(p.path().string(), &report);
	std::string fileName = p.path().filename().string();

	bool shouldFail = p.path().filename().c_str()[0] == 'n';
	bool anyResult = p.path().filename().c_str()[0] == 'i';

	if (anyResult) {
		std::cout << "Ok.\n";
		print_report(report);
		return true;
	}

	if (shouldFail) {
		if (report.size() != 0) {
			std::cout << "Ok.\n";
			return true;
		}
		std::cout << "Fail.\n";
		return false;
	}

	if (report.size() != 0) {
		std::cout << "Fail.\n";
		print_report(report);
		return false;
	}

	std::cout << "Ok.\n";
	return true;
}

void json::test::run_entire_test_suite()
{

	std::string testDirectory = fs::current_path().string() + "\\JSONTestSuite";

	std::vector<std::filesystem::directory_entry> testToFail;
	std::vector<std::filesystem::directory_entry> testToPass;
	std::vector<std::filesystem::directory_entry> testToEh;

	// print all files from the current directory and below
	for (auto& p : fs::recursive_directory_iterator(testDirectory)) {
		bool anyResult = p.path().filename().c_str()[0] == 'i';
		if (anyResult) {
			testToEh.push_back(p);
			continue;
		}

		bool shouldFail = p.path().filename().c_str()[0] == 'n';
		if (shouldFail) {
			testToFail.push_back(p);
			continue;
		}

		testToPass.push_back(p);
	}


	std::cout << "Running tests with JSON that can be have any result\n";
	for (auto& p : testToEh) {
		run_parse_test_from_file(p);
	}

	size_t testToPassOKCount{ 0 }, testToFailOKCount{ 0 };
	std::cout << "-------------------------------\n\n";
	std::cout << "Running tests with malformed JSON\n";

	for (auto& p : testToFail) {
		if (run_parse_test_from_file(p)) {
			testToFailOKCount++;
		}
	}

	std::cout << "-------------------------------\n\n";
	std::cout << "Running tests with correct JSON\n";

	for (auto& p : testToPass) {
		if (run_parse_test_from_file(p)) {
			testToPassOKCount++;
		}
	}

	std::cout << "\n\n";
	std::cout << "Out of the '" << testToFail.size() << "' test that need to fail '" << testToFailOKCount << "' failed as expected!\n";
	std::cout << "Out of the '" << testToPass.size() << "' test that need to pass '" << testToPassOKCount << "' passed as expected!\n";
}


