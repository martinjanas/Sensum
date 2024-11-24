#pragma once
#include <cstdint>

class CHudChat
{
public:
	enum ChatFilters
	{
		CHAT_FILTER_NONE = 0,
		CHAT_FILTER_JOINLEAVE = 0x000001,
		CHAT_FILTER_NAMECHANGE = 0x000002,
		CHAT_FILTER_PUBLICCHAT = 0x000004,
		CHAT_FILTER_SERVERMSG = 0x000008,
		CHAT_FILTER_TEAMCHANGE = 0x000010,
		CHAT_FILTER_ACHIEVEMENT = 0x000020,
	};

	//void ChatPrintf(const char* fmt, ...) //crashing
	//{
	//	using fn = void(__cdecl*)(void*, uint32_t, const char*, ...);
	//	static auto addr = modules::client.scan("E8 ?? ?? ?? ?? 49 8B 4E 20 BA ?? ?? ?? ??", "ChatPrintf").add(0x1).abs().as();
	//	if (!addr)
	//		return;

	//	auto chat_printf = reinterpret_cast<fn>(addr);
	//	if (chat_printf)
	//		chat_printf(this, -1, fmt);
	//}
};