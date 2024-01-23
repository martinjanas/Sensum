#pragma once

#define NOMINMAX
#include <Windows.h>
#include <cstdint>
#include <functional>

enum class KeyState
{
	None = 1,
	Down,
	Up,
	Pressed
};

DEFINE_ENUM_FLAG_OPERATORS(KeyState);

namespace input_system
{
	void initialize();
	void destroy();

	HWND get_main_window();
	KeyState get_key_state(uint32_t vk);

	bool is_key_down(uint32_t vk);
	bool was_key_pressed(uint32_t vk);

	void register_hotkey(uint32_t vk, std::function<void(void)> f);
	void remove_hotkey(uint32_t vk);
}