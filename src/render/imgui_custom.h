#pragma once
#include "render.h"
#include <vector>

namespace imgui_custom
{
	void selector(const char* label, const char** items, int items_count, int* v);
	void separator(const char* label);
	bool checkbox(const char* label, bool* v);
}

namespace imc = imgui_custom;



