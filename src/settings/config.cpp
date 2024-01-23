#include "config.h"
#include "../helpers/console.h"

#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

namespace config
{
	const std::string path = "C:\\Sensum";
	std::map<std::string, std::map<std::string, Json::Value>> cached;

	std::string path_to_folder(const std::string& folder)
	{
		return path + (folder.empty() ? "" : "/" + folder);
	}

	std::string path_to_file(const std::string& name, const std::string& folder)
	{
		const auto next = path_to_folder(folder);

		return next + (name.empty() ? "" : "/" + name);
	}

	Json::Value get_source(const std::string& name, const std::string& folder)
	{
		Json::Value root;

		std::ifstream source(path_to_file(name, folder), std::ifstream::binary);
		if (!source.good())
		{
#ifdef _DEBUG
			console::print("File %s doesnt exists.", name.c_str());
#endif
			return root;
		}

		source >> root;

#ifdef _DEBUG
		console::print("File %s was succesfully loaded.", name.c_str());
#endif

		return root;
	}

	Json::Value get_and_cache_source(const std::string& name, const std::string& folder)
	{
		if (cached[folder].count(name) == 0)
			cached[folder][name] = get_source(name, folder);

		return cached[folder][name];
	}

	void cache(const std::string& folder)
	{
		const auto items = get_files(folder);
		for (auto& file : items)
			get_and_cache_source(file, folder);
	}

	std::vector<std::string> get_files(const std::string& folder)
	{
		std::vector<std::string> items;

		std::string path = path_to_folder(folder);
		if (!fs::is_directory(path))
			fs::create_directories(path);

		for (auto& p : fs::directory_iterator(path))
			items.emplace_back(p.path().string().substr(path.length() + 1));

		return items;
	}

	void load(const std::string& name, const std::string& folder, const bool& cache, const std::function<void(Json::Value)>& callback)
	{
		if (name.empty())
			return;

		const auto source = cache ? get_and_cache_source(name, folder) : get_source(name, folder);

		callback(source);
	}

	bool save(const std::string& name, const std::string& folder, const bool& cache, const std::function<Json::Value(void)>& callback)
	{
		if (name.empty())
			return false;

		const auto root = callback();
		if (cache)
			cached[folder][name] = root;

		Json::StyledWriter styledWriter;

		try
		{
			std::ofstream(path_to_file(name, folder)) << styledWriter.write(root);

#ifdef _DEBUG
			console::print("%s successfully saved", name.c_str());
#endif // _DEBUG

			return true;
		}
		catch (const std::exception& ex)
		{
#ifdef _DEBUG
			console::print("failed to save %s: %s", name.c_str(), ex.what());
#endif // _DEBUG
		}

		return false;
	}

	bool remove(const std::string& name, const std::string& folder)
	{
		const auto state = fs::remove(path_to_file(name, folder));
		if (state)
		{
			cached[folder].clear();
			cache(folder);
		}

		return state;
	}
}