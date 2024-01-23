#include "options.hpp"
#include "../helpers/console.h"
#include <filesystem>

#pragma warning( disable : 4101 )

namespace fs = std::filesystem;

namespace Config
{
	std::string GetPath(const std::string& name, const std::string& folder)
	{
		return "C:\\Sensum/" + (folder.empty() ? "" : folder + "/") + name;
	}

	std::vector<std::string> Configs(const std::string& folder)
	{
		std::vector<std::string> items = { };

		std::string path("C:\\Sensum/" + folder);
		if (!fs::is_directory(path))
			fs::create_directories(path);

		for (auto& p : fs::directory_iterator(path))
			items.emplace_back(p.path().string().substr(path.length() + 1));

		return items;
	}

	bool Delete(const std::string& name, const std::string& folder)
	{
		return fs::remove(GetPath(name, folder));
	}

	void Load(const std::string& name, const std::string& folder, std::function<void(Json::Value)> f)
	{
		if (name.empty())
			return;

		Json::Value root;

		try
		{
			std::ifstream f(GetPath(name, folder), std::ifstream::binary);
			if (!f.good())
			{
#ifdef _DEBUG
				console::print("file doesnt exists");
#endif
				return;
			}

			f >> root;
		}
		catch (const std::exception& ex)
		{
#ifdef _DEBUG
			console::print("Failed to load %s: %s", name.c_str(), ex.what());
#endif
			return;
		}

		f(root);

#ifdef _DEBUG
		console::print("%s successfully loaded", name.c_str());
#endif
	}

	void Save(const std::string& name, const std::string& folder, std::function<Json::Value(void)> f)
	{
		if (name.empty())
			return;

		const auto root = f();

		Json::StyledWriter styledWriter;

		try
		{
			std::ofstream(GetPath(name, folder)) << styledWriter.write(root);

#ifdef _DEBUG
			console::print("%s successfully saved", name.c_str());
#endif
		}
		catch (const std::exception& ex)
		{
#ifdef _DEBUG
			console::print("Failed to save %s: %s", name.c_str(), ex.what());
#endif
		}
	}
};

namespace Option
{
	void Save(Json::Value& config, ImVec4 setting)
	{
		config["x"] = setting.x;
		config["y"] = setting.y;
		config["z"] = setting.z;
		config["w"] = setting.w;
	}

	void Save(Json::Value& config, float setting)
	{
		config["r"] = config.asFloat();
		config["g"] = config.asFloat();
		config["b"] = config.asFloat();
		config["a"] = config.asFloat();
	}

	void Save(Json::Value& config, Color setting)
	{
		config["r"] = setting.r();
		config["g"] = setting.g();
		config["b"] = setting.b();
		config["a"] = setting.a();
	}

	void Load(Json::Value config, ImVec4& setting, const ImVec4& def)
	{
		if (config.isNull())
			setting = def;
		else
			setting = ImVec4(config["x"].asFloat(), config["y"].asFloat(), config["z"].asFloat(), config["w"].asFloat());
	}

	void Load(Json::Value config, Color& setting, const Color& def)
	{
		if (config.isNull())
			setting = def;
		else
			setting.SetColor(config["r"].asInt(), config["g"].asInt(), config["b"].asInt(), config["a"].asInt());
	}

	void Load(Json::Value config, int& setting, const int& def)
	{
		setting = config.isNull() ? def : config.asInt();
	}

	void Load(Json::Value config, bool& setting, const bool& def)
	{
		setting = config.isNull() ? def : config.asBool();
	}

	void Load(Json::Value config, float& setting, const float& def)
	{
		setting = config.isNull() ? def : config.asFloat();
	}
}