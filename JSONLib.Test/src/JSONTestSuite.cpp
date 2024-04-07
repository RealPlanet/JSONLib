#include "pch.h"
#include "JSON.h"
#include "JSONParser.h"
#include "JSONUtility.h"

#include <iostream>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

#include <string>
#include <iostream>
#include <Windows.h>
#include <cstdio>

void print_report(const std::vector<std::string>& err)
{
	for (auto& s : err)
		std::cout << s << "\n";
}

TEST(JSONTestSuite, JSONPrintEmoj)
{
	std::string json = R"({ "EMOJ" : "\u9080" })";

	std::vector<std::string> errors;
	auto jsonStructure = json::Parser::from_text(json, &errors);

	ASSERT_TRUE(errors.size() == 0);

	// Set console code page to UTF-8 so console known how to interpret string data
	SetConsoleOutputCP(CP_UTF8);

	// Enable buffering to prevent VS from chopping up UTF-8 byte sequences
	setvbuf(stdout, nullptr, _IONBF, 0);

	auto str = jsonStructure.to_string();
	std::cout << str << "\n";
}

TEST(JSONTestSuite, JSONTestSuite)
{
	std::string testDirectory = fs::current_path().string() + "\\JSONTestSuite";

	// print all files from the current directory and below
	for (auto& p : fs::recursive_directory_iterator(testDirectory))
	{
		if (p.path().filename().c_str()[0] == 'i')
			continue;

		std::cout << "Testing file: " << p.path() << '\n';

		std::vector<std::string> report;

		//if(p.path() == "E:\\Programming\\JSONLib\\JSONLib.Test\\JSONTestSuite\\n_object_several_trailing_commas.json")
		//	__debugbreak();

		auto json = json::Parser::from_file(p.path().string(), &report);

		bool shouldFail = p.path().filename().c_str()[0] == 'n';
		if (shouldFail)
		{
			print_report(report);
			if (report.size() == 0)
			{
				__debugbreak();
			}

			ASSERT_TRUE(report.size() != 0);
			std::cout << "Passed!" << '\n';
			continue;
		}

		print_report(report);

		if (report.size() != 0)
		{
			__debugbreak();
		}
		ASSERT_TRUE(report.size() == 0);
		std::cout << "Passed!" << '\n';
	}
}