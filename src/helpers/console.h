#pragma once

#include "../valve_sdk/csgostructs.hpp"

namespace console
{
	void attach();
	void detach();

	void print(Vector pos);
	void print(QAngle angles);
	void print(const char* fmt, ...);
}