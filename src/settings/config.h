#pragma once

#include <map>
#include <vector>
#include <string>
#include <functional>

#include "../jsoncpp/json.h"

namespace config
{
	extern std::map<std::string, std::map<std::string, Json::Value>> cached;

	void load(const std::string& name, const std::string& folder, const bool& cache, const std::function<void(Json::Value)>& callback);
	bool save(const std::string& name, const std::string& folder, const bool& cache, const std::function<Json::Value(void)>& callback);
	bool remove(const std::string& name, const std::string& folder);

	void cache(const std::string& folder);
	std::vector<std::string> get_files(const std::string& folder);
}