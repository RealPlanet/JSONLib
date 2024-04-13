#include <iostream>
#include <vector>
#include <filesystem>

#include "Tests.h"

int main(int argc, char* argv[]) {
	size_t failedUnitTests{ 0 };
	for (FunctionTestPointer& ptr : json::test::gTests) {
		if (ptr()) {
			std::cout << "Test passed\n\n";
			continue;
		}

		failedUnitTests++;
		std::cout << "Test did not pass\n\n";
	}

	json::test::run_entire_test_suite();

	std::cout << "Of the '" << json::test::gTests.size() << "' unit tests a total of '" << failedUnitTests << "' tests FAILED!\n";

	std::cout << "Input anything to close\n";
	char c;
	std::cin >> c;

	return 0;
}