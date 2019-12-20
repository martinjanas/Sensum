#include "hooks.h"
#include "../globals.h"
#include "../settings.h"
#include "../jsoncpp/json.h"
#include "../render/render.h"
#include "../helpers/http.h"
#include "../helpers/console.h"
#include "../helpers/notifies.h"
#include "../helpers/proto_parse.h"
#include "../features/features.h"

#include <intrin.h>
#include <inttypes.h>

#define k_EMsgGCCStrike15_v2_MatchmakingGC2ClientReserve 9107

#define CAST(cast, address, add) reinterpret_cast<cast>((uint32_t)address + (uint32_t)add)

make_struct(VoteStart, 8)
make_field(team, 1, TYPE_UINT32)
make_field(ent_idx, 2, TYPE_UINT32)
make_field(vote_type, 3, TYPE_UINT32)
make_field(disp_str, 4, TYPE_STRING)
make_field(details_str, 5, TYPE_STRING)
make_field(other_team_str, 6, TYPE_STRING)
make_field(is_yes_no_vote, 7, TYPE_BOOL)
make_field(entidx_target, 8, TYPE_UINT32)
};

make_struct(SayText2, 5)
make_field(ent_idx, 1, TYPE_UINT32)
make_field(chat, 2, TYPE_BOOL)
make_field(msg_name, 3, TYPE_STRING)
make_field(params, 4, TYPE_STRING)
make_field(textallchat, 5, TYPE_BOOL)
};

make_struct(ClientReserve, 7)
make_field(map, 6, TYPE_STRING)
};

make_struct(ClientUpdate, 11)
make_field(vacbanned_account_id_sessions, 11, TYPE_UINT32);
};

namespace hooks
{
	vfunc_hook retrieve_message::hook;
	blackbone::Detour<dispatch_user_message::fn> dispatch_user_message::hook;

	template <typename MessageType>
	void read_packet(const int& message_type, const int& target_type, void* msg_data, const uint32_t& msg_size, const std::function<void(const MessageType& message)>& fn)
	{
		if (message_type == target_type)
		{
			MessageType message((void*)((DWORD)msg_data + 8), msg_size - 8);

			fn(message);
		}
	}

	template <typename MessageType>
	void edit_packet(const int& message_type, const int& target_type, void* msg_data, uint32_t msg_size, const std::function<void(MessageType& message)>& fn)
	{
		if (message_type != target_type)
			return;

		MessageType message((void*)((DWORD)msg_data + 8), msg_size - 8);

		fn(message);

		const auto packet = message.serialize();
		memcpy((void*)((DWORD)msg_size + 8), (void*)packet.data(), packet.size());
	}

	template <typename MessageType>
	void read_message(const int& message_type, const int& target_type, const int& length, const void* msg_data, const std::function<void(MessageType& message)>& fn)
	{
		if (message_type == target_type)
		{
			MessageType message((void*)msg_data, length);

			fn(message);
		}
	}

	EGCResults __stdcall retrieve_message::hooked(uint32_t* punMsgType, void* pubDest, uint32_t cubDest, uint32_t* pcubMsgSize)
	{
		static auto original = hook.get_original<fn>(index);

		const auto result = original(interfaces::game_coordinator, punMsgType, pubDest, cubDest, pcubMsgSize);
		if (result != EGCResults::k_EGCResultOK)
			return result;

		const uint32_t MessageType = *punMsgType & 0x7FFFFFFF;
#ifdef _DEBUG
		console::print("[->] Message received from GC [%d]!", MessageType);
#endif

		if (MessageType == 9191)
			lobby_inviter::update();

		return result;
	}

	bool __stdcall dispatch_user_message::hooked(void*& thisptr, int& type, int& a3, int& length, void*& msg_data)
	{
		//static auto original = hook.CallOriginal(std::forward<void*>(thisptr), std::forward<int>(type), std::forward<int>(a3), std::forward<int>(length), std::forward<void*>(msg_data));

		uint32_t MessageType = type & 0x7FFFFFFF;
#ifdef _DEBUG
		console::print("[->] User Message [%d]!", MessageType);
#endif

		read_message<VoteStart>(MessageType, CS_UM_VoteStart, length, msg_data, [](VoteStart& message)
		{
			if (!interfaces::local_player)
				return;

			if (message.get_disp_str().String() != std::string("#SFUI_vote_kick_player_other"))
				return;

			if (message.get_details_str().String().empty())
				return;

			player_info_t local_info = interfaces::local_player->GetPlayerInfo();
			if (message.get_details_str().String().find(local_info.szName) != std::string::npos)
			{
				interfaces::engine_client->ClientCmd_Unrestricted("callvote swapteams");

				notifies::push(strings::antikick.c_str(), notify_state_s::warning_state);
			}
		});

		return 0;
		//return original = hook.CallOriginal(std::forward<void*>(thisptr), std::forward<int>(type), std::forward<int>(a3), std::forward<int>(length), std::forward<void*>(msg_data));
	}
}
