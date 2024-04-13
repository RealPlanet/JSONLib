#pragma once

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

typedef bool(*FunctionTestPointer)();

namespace json {
	namespace test {
		bool are_values_equal();
		bool are_empty_objects_equal();
		bool are_empty_copied_objects_equal();
		bool are_empty_arrays_equal();
		bool are_empty_copied_arrays_equal();

		// JSON ACCESS
		bool can_mixed_objs_and_arrays_be_accessed();
		bool can_access_edited_field_and_is_updated();

		// JSON INSERT
		bool can_add_to_json();

		// JSON SEARCH
		bool json_path_search_works();

		static std::vector< FunctionTestPointer> gTests = {
			are_values_equal,
			are_empty_objects_equal,
			are_empty_copied_objects_equal,
			are_empty_arrays_equal,
			are_empty_copied_arrays_equal,

			// JSON ACCESS
			can_mixed_objs_and_arrays_be_accessed,
			can_access_edited_field_and_is_updated,

			// JSON INSERT
			can_add_to_json,

			// JSON SEARCH
			json_path_search_works,
		};

		// TEST SUITE
		void print_report(const std::vector<std::string>& err);
		bool run_parse_test_from_file(std::filesystem::directory_entry p);
		void run_entire_test_suite();
	}
}