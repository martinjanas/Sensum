#include "console.h"
#include "../security/importer.h"

#include <cstdio>
#include <Windows.h>

namespace console
{
	HANDLE out, old_out;

	void attach()
	{
		old_out = LI_FN(GetStdHandle)(STD_OUTPUT_HANDLE);

		LI_FN(AllocConsole)();
		LI_FN(AttachConsole)(LI_FN(GetCurrentProcessId)());

		out = LI_FN(GetStdHandle)(STD_OUTPUT_HANDLE);

		LI_FN(SetConsoleMode)(out, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);
	}

	void detach()
	{
		if (out) {
			LI_FN(FreeConsole)();

			if (old_out)
				LI_FN(SetStdHandle)(STD_OUTPUT_HANDLE, old_out);
		}
	}

	void print(Vector pos)
	{
		print("%.2f; %.2f; %.2f", pos.x, pos.y, pos.z);
	}

	void print(QAngle angles)
	{
		print("%.2f; %.2f; %.2f", angles.pitch, angles.yaw, angles.roll);
	}

	void print(const char* format, ...)
	{
//#ifdef _DEBUG
		if (!out)
			return;
		char buffer[1024 * 16];

		va_list argptr;
		va_start(argptr, format);
		vsnprintf(buffer, sizeof(buffer), format, argptr);
		va_end(argptr);

		strcat_s(buffer, sizeof(buffer), "\n");

		WriteConsoleA(out, buffer, strlen(buffer), nullptr, nullptr);
//#endif // _DEBUG
	}
}