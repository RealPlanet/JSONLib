#include "JSON.h"
#include "JSONParser.h"
#include "JSONUtility.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <iostream>
#include <Windows.h>
#include <cstdio>
namespace fs = std::filesystem;

void print_report(const std::vector<std::string>& err){
	for (auto& s : err)
		std::cout << s << "\n";
}

void run_test(std::filesystem::directory_entry p) {
	std::cout << "> " << p.path().filename().string() << "...";

	std::vector<std::string> report;

	auto json = json::Parser::from_file(p.path().string(), &report);
	std::string fileName = p.path().filename().string();

	bool shouldFail = p.path().filename().c_str()[0] == 'n';
	bool anyResult = p.path().filename().c_str()[0] == 'i';

	if (anyResult) {
		std::cout << "Ok.\n";
		print_report(report);
		return;
	}

	if (shouldFail) {
		if (report.size() != 0)
			std::cout << "Ok.\n";
		else
			std::cout << "Fail.\n";
		return;
	}

	if (report.size() != 0){
		std::cout << "Fail.\n";
		print_report(report);
	}
	else {
		std::cout << "Ok.\n";
	}		
}

void run_test_suite() {

	std::string testDirectory = fs::current_path().string() + "\\JSONTestSuite";

	std::vector<std::filesystem::directory_entry> testToFail;
	std::vector<std::filesystem::directory_entry> testToPass;
	std::vector<std::filesystem::directory_entry> testToEh;

	// print all files from the current directory and below
	for (auto& p : fs::recursive_directory_iterator(testDirectory))
	{
		// TODO - Optimize stack
		if (p.path().filename() == "n_structure_100000_opening_arrays.json" ||
			p.path().filename() == "n_structure_open_array_object.json")
			continue;

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

	std::cout << "Running tests with malformed JSON\n";
	for (auto& p : testToFail) {
		run_test(p);
	}

	std::cout << "-------------------------------\n\n";
	std::cout << "Running tests with correct JSON\n";
	for (auto& p : testToPass) {
		run_test(p);
	}

	std::cout << "-------------------------------\n\n";
	std::cout << "Running tests with JSON that can be have any result\n";
	for (auto& p : testToEh) {
		run_test(p);
	}
}

int main(int argc, char* argv[]) {

	if (argc > 1)
	{
		std::cout << "Single test mode -> ON\n";
		return 0;
	}

	run_test_suite();

	std::cout << "Input anything to close\n";
	char c;
	std::cin >> c;

	return 0;
}