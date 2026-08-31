#pragma once

#ifndef _H_JSERIALIZATION_
#define _H_JSERIALIZATION_

#include "json.h"
#include <vector>
#include <map>

namespace json
{
	json::JElementPtr Serialize(int64_t source);

	json::JElementPtr Serialize(bool source);

	json::JElementPtr Serialize(double source);

	json::JElementPtr Serialize(float source);

	json::JElementPtr Serialize(const std::string& source);

	template<typename T>
	inline json::JElementPtr Serialize(const std::vector<T>& source)
	{
		auto array = new json::JArray();
		auto result = json::JElementPtr(array);

		for (const auto& element : source)
		{
			json::JElementPtr elementJson = Serialize(element);
			array->push_back(std::move(elementJson));
		}

		return std::move(result);
	}

	template<typename T>
	inline json::JElementPtr Serialize(const std::map<std::string, T>& source)
	{
		auto map = new json::JObject();

		auto it = source.begin();
		while (it != source.end())
		{
			map[it->first] = Serialize(it->second);
		}

		return map;
	}
}



#endif // !_H_JSERIALIZATION_
