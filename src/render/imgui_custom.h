#pragma once
#include "render.h"
#include <vector>

namespace imgui_custom
{
	//void selector(const char* label, const char** items, int items_count, int* v);
	void selector(const std::vector<std::string>& items, int* v);
}

namespace imc = imgui_custom;



