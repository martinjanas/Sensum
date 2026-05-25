#include "Modules2.h"

namespace modules2
{
	void InitModules()
	{
		client2 = Module("client.dll");
		engine2 = Module("engine2.dll");
		schema2 = Module("schemasystem.dll");
		tier02 = Module("tier0.dll");
		directx112 = Module("rendersystemdx11.dll");
		input_sys2 = Module("inputsystem.dll");
		matchmaking2 = Module("matchmaking.dll");
		gameoverlay2 = Module("GameOverlayRenderer64.dll");
		materialsys2 = Module("materialsystem2.dll");
		scenesys2 = Module("scenesystem.dll");
		nav_system2 = Module("navsystem.dll");
	}
}
