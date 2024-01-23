#include "../hooked_functions/game_event_listener.h"

std::once_flag get_class_ids_flag;
void CGameEventListener::FireGameEvent(IGameEvent* context)
{
	const auto name = fnv::hash_runtime(context->GetName());

	if (name == FNV("game_newmap"))
	{
		std::call_once(get_class_ids_flag, []() { g::get_class_ids(); });

		color_modulation::event();
		globals::team_damage.clear();
	}
	else if (name == FNV("player_hurt"))
	{
		auto attacker = c_base_player::GetPlayerByUserId(context->GetInt("attacker"));
		auto target = c_base_player::GetPlayerByUserId(context->GetInt("userid"));

		if (!attacker || !target)
			return;

		if (attacker->m_iTeamNum() == target->m_iTeamNum())
			globals::team_damage[context->GetInt("attacker")] += context->GetInt("dmg_health");
	}
	else if (name == FNV("bomb_begindefuse"))
		features::on_bomb_begindefuse(context);
	else if (name == FNV("item_purchase"))
		features::buy_logger(context);
}