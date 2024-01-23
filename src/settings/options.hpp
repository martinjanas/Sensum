#pragma once

#include <fstream>
#include <functional>

#include "settings.h"
#include "../jsoncpp/json.h"
#include "../helpers/math.h"

namespace Option
{
	void Save(Json::Value&, ImVec4);
	void Save(Json::Value&, Color);
	void Save(Json::Value&, float);

	void Load(Json::Value, ImVec4&, const ImVec4 & = ImVec4::White);
	void Load(Json::Value, Color&, const Color & = Color::White);
	void Load(Json::Value, int&, const int& = 0);
	void Load(Json::Value, bool&, const bool& = false);
	void Load(Json::Value, float&, const float& = 0.f);
}