#include "features.h"
#include "../settings/config.h"
#include "../settings/globals.h"
#include "../settings/options.hpp"
#include "../helpers/http.h"
#include "../helpers/console.h"

namespace skins
{
	std::map<int, item_setting> m_items = { };
	std::map<int, statrack_setting> statrack_items = { };
	std::unordered_map<std::string, std::string> m_icon_overrides = { };

	std::vector<paint_kit_t> skin_kits = { };

	std::map<int, weapon_info_t> info =
	{
		{ WEAPON_KNIFE,{ "models/weapons/v_knife_default_ct.mdl", "knife_default_ct" } },
		{ WEAPON_KNIFE_T,{ "models/weapons/v_knife_default_t.mdl", "knife_t" } },
		{ WEAPON_KNIFE_BAYONET,{ "models/weapons/v_knife_bayonet.mdl", "bayonet" } },
		{ WEAPON_KNIFE_FLIP,{ "models/weapons/v_knife_flip.mdl", "knife_flip" } },
		{ WEAPON_KNIFE_GUT,{ "models/weapons/v_knife_gut.mdl", "knife_gut" } },
		{ WEAPON_KNIFE_KARAMBIT,{ "models/weapons/v_knife_karam.mdl", "knife_karambit" } },
		{ WEAPON_KNIFE_M9_BAYONET,{ "models/weapons/v_knife_m9_bay.mdl", "knife_m9_bayonet" } },
		{ WEAPON_KNIFE_TACTICAL,{ "models/weapons/v_knife_tactical.mdl", "knife_tactical" } },
		{ WEAPON_KNIFE_FALCHION,{ "models/weapons/v_knife_falchion_advanced.mdl", "knife_falchion" } },
		{ WEAPON_KNIFE_SURVIVAL_BOWIE,{ "models/weapons/v_knife_survival_bowie.mdl", "knife_survival_bowie" } },
		{ WEAPON_KNIFE_BUTTERFLY,{ "models/weapons/v_knife_butterfly.mdl", "knife_butterfly" } },
		{ WEAPON_KNIFE_PUSH,{ "models/weapons/v_knife_push.mdl", "knife_push" } },
		{ WEAPON_KNIFE_URSUS,{ "models/weapons/v_knife_ursus.mdl", "knife_ursus" } },
		{ WEAPON_KNIFE_GYPSY_JACKKNIFE,{ "models/weapons/v_knife_gypsy_jackknife.mdl", "knife_gypsy_jackknife" } },
		{ WEAPON_KNIFE_STILETTO,{ "models/weapons/v_knife_stiletto.mdl", "knife_stiletto" } },
		{ WEAPON_KNIFE_WIDOWMAKER,{ "models/weapons/v_knife_widowmaker.mdl", "knife_widowmaker" } },
		{ WEAPON_KNIFE_CSS,{ "models/weapons/v_knife_css.mdl", "knife_css" } },
		{ WEAPON_KNIFE_CORD,{ "models/weapons/v_knife_cord.mdl", "knife_cord" } },
		{ WEAPON_KNIFE_CANIS,{ "models/weapons/v_knife_canis.mdl", "knife_canis" } },
		{ WEAPON_KNIFE_OUTDOOR,{ "models/weapons/v_knife_outdoor.mdl", "knife_outdoor" } },
		{ WEAPON_KNIFE_SKELETON,{ "models/weapons/v_knife_skeleton.mdl", "knife_skeleton" } },
		{ GLOVE_STUDDED_BLOODHOUND,{ "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl" } },
		{ GLOVE_T_SIDE,{ "models/weapons/v_models/arms/glove_fingerless/v_glove_fingerless.mdl" } },
		{ GLOVE_CT_SIDE,{ "models/weapons/v_models/arms/glove_hardknuckle/v_glove_hardknuckle.mdl" } },
		{ GLOVE_SPORTY,{ "models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl" } },
		{ GLOVE_SLICK,{ "models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl" } },
		{ GLOVE_LEATHER_WRAP,{ "models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl" } },
		{ GLOVE_MOTORCYCLE,{ "models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl" } },
		{ GLOVE_SPECIALIST,{ "models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl" } },
		{ GLOVE_HYDRA,{ "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl" } }
	};

	std::vector<weapon_name_t> knife_names =
	{
		{ 0, "Default" },
		{ WEAPON_KNIFE_BAYONET, "Bayonet" },
		{ WEAPON_KNIFE_FLIP, "Flip Knife" },
		{ WEAPON_KNIFE_GUT, "Gut Knife" },
		{ WEAPON_KNIFE_KARAMBIT, "Karambit" },
		{ WEAPON_KNIFE_M9_BAYONET, "M9 Bayonet" },
		{ WEAPON_KNIFE_TACTICAL, "Huntsman Knife" },
		{ WEAPON_KNIFE_FALCHION, "Falchion Knife" },
		{ WEAPON_KNIFE_SURVIVAL_BOWIE, "Bowie Knife" },
		{ WEAPON_KNIFE_BUTTERFLY, "Butterfly Knife" },
		{ WEAPON_KNIFE_PUSH, "Shadow Daggers" },
		{ WEAPON_KNIFE_URSUS, "Ursus Knife" },
		{ WEAPON_KNIFE_GYPSY_JACKKNIFE, "Navaja Knife" },
		{ WEAPON_KNIFE_STILETTO, "Stiletto Knife" },
		{ WEAPON_KNIFE_WIDOWMAKER, "Talon Knife" },
		{ WEAPON_KNIFE_CSS, "Classic Knife" },
		{ WEAPON_KNIFE_CORD, "Paracord Knife" },
		{ WEAPON_KNIFE_CANIS, "Survival Knife" },
		{ WEAPON_KNIFE_OUTDOOR, "Nomad Knife" },
		{ WEAPON_KNIFE_SKELETON, "Skeleton Knife" }
	};

	std::vector<weapon_name_t> glove_names =
	{
		{ 0, "Default" },
		{ GLOVE_T_SIDE, "Default (Terrorists)" },
		{ GLOVE_CT_SIDE, "Default (Counter-Terrorists)" },
		{ GLOVE_STUDDED_BLOODHOUND, "Bloodhound" },
		{ GLOVE_SPORTY, "Sporty" },
		{ GLOVE_SLICK, "Slick" },
		{ GLOVE_LEATHER_WRAP, "Handwrap" },
		{ GLOVE_MOTORCYCLE, "Motorcycle" },
		{ GLOVE_SPECIALIST, "Specialist" },
		{ GLOVE_HYDRA, "Hydra" },
	};

	const weapon_info_t* get_weapon_info(const int& defindex)
	{
		const auto entry = info.find(defindex);

		return entry == end(info) ? nullptr : &entry->second;
	}

	bool is_knife(const int& i)
	{
		return (i >= WEAPON_KNIFE_BAYONET && i < GLOVE_STUDDED_BLOODHOUND) || i == WEAPON_KNIFE_T || i == WEAPON_KNIFE;
	}

	const char* get_icon_override(const std::string& original)
	{
		return m_icon_overrides.count(original) > 0 ? m_icon_overrides.at(original).data() : nullptr;
	}

	void erase_override_if_exists_by_index(const int definition_index)
	{
		if (const auto original_item = get_weapon_info(definition_index))
		{
			if (!original_item->icon)
				return;

			const auto override_entry = m_icon_overrides.find(original_item->icon);
			if (override_entry != end(m_icon_overrides))
				m_icon_overrides.erase(override_entry);
		}
	}

	void apply_config_on_attributable_item(c_base_attributable_item* item, const item_setting config, const unsigned xuid_low)
	{
		auto _item = reinterpret_cast<c_base_combat_weapon*>(item);

		if (_item)
			_item->m_iWorldModelIndex() = -1;

		item->m_iItemIDHigh() = -1;

		item->m_iAccountID() = xuid_low;

		if (config.custom_name[0])
			strcpy_s(item->m_iCustomName(), config.custom_name);

		if (config.paint_kit_index)
			item->m_nFallbackPaintKit() = config.paint_kit_index;

		if (config.seed)
			item->m_nFallbackSeed() = config.seed;

		if (config.stat_track.enabled)
		{
			//item->m_nFallbackStatTrak() = config.stat_track.counter;
			item->m_iEntityQuality() = 9;
		}
		else
			item->m_iEntityQuality() = is_knife(config.definition_index) ? 3 : 0;

		item->m_flFallbackWear() = config.wear;

		auto& definition_index = item->m_iItemDefinitionIndex();
		if (!config.definition_override_index || config.definition_override_index == definition_index)
		{
			erase_override_if_exists_by_index(definition_index);
			return;
		}

		const auto replacement_item = get_weapon_info(config.definition_override_index);
		if (!replacement_item)
			return;

		const auto old_definition_index = definition_index;
		definition_index = config.definition_override_index;

		// Set the weapon model index -- required for paint kits to work on replacement items after the 29/11/2016 update.
		item->m_nModelIndex() = g::mdl_info->GetModelIndex(replacement_item->model);
		item->SetModelIndex(g::mdl_info->GetModelIndex(replacement_item->model));
		item->GetClientNetworkable()->PreDataUpdate(0);

		// We didn't override 0, but some actual weapon, that we have data for
		if (!old_definition_index)
			return;

		const auto original_item = get_weapon_info(old_definition_index);
		if (original_item && original_item->icon && replacement_item->icon)
			m_icon_overrides[original_item->icon] = replacement_item->icon;
	}

	void ApplyStatrack(c_base_attributable_item* item, const item_setting config, const statrack_setting cfg)
	{
		if (config.stat_track.enabled)
		{
			//item->m_nFallbackStatTrak() = config.stat_track.counter;
			item->m_nFallbackStatTrak() = cfg.statrack_new.counter;
			item->m_iEntityQuality() = 9;
		}
	}

	template <typename T>
	T* get_entity_from_handle(CBaseHandle h)
	{
		if (h == INVALID_EHANDLE_INDEX)
			return nullptr;

		return static_cast<T*>(g::entity_list->GetClientEntityFromHandle(h));
	}

	CreateClientClassFn get_wearable_create_fn()
	{
		auto clazz = g::base_client->GetAllClasses();
		while (fnv::hash_runtime(clazz->m_pNetworkName) != FNV("CEconWearable"))
			clazz = clazz->m_pNext;

		return clazz->m_pCreateFn;
	}

	auto make_glove(int entry, int serial) -> c_base_attributable_item*
	{
		static auto create_wearable_fn = get_wearable_create_fn();

		create_wearable_fn(entry, serial);

		const auto glove = static_cast<c_base_attributable_item*>(g::entity_list->GetClientEntity(entry));
		assert(glove);

		static auto set_abs_origin_addr = utils::pattern_scan(SET_ABS_ORIGIN);
		static constexpr std::array<float, 3> new_pos = { 10000.f, 10000.f, 10000.f };

		const auto set_abs_origin_fn = reinterpret_cast<void(__thiscall*)(void*, const std::array<float, 3>&)>(set_abs_origin_addr);
		set_abs_origin_fn(glove, new_pos);

		return glove;
	}

	bool handle_glove_config(player_info_t player_info)
	{
		const auto wearables = g::local_player->m_hMyWearables();
		const auto glove_config = m_items[g::local_player->m_iTeamNum() == team::team_ct ? GLOVE_CT_SIDE : GLOVE_T_SIDE];
		static auto glove_handle = CBaseHandle(0);

		auto glove = get_entity_from_handle<c_base_attributable_item>(wearables[0]);
		if (!glove) // There is no glove
		{
			// Try to get our last created glove
			const auto our_glove = get_entity_from_handle<c_base_attributable_item>(glove_handle);
			if (our_glove) // Our glove still exists
			{
				wearables[0] = glove_handle;
				glove = our_glove;
			}
		}

		if (!g::local_player->IsAlive())
		{
			// We are dead but we have a glove, destroy it
			if (glove)
			{
				glove->GetClientNetworkable()->SetDestroyedOnRecreateEntities();
				glove->GetClientNetworkable()->Release();
			}

			return false;
		}

		if (glove_config.definition_override_index)
		{
			// We don't have a glove, but we should
			if (!glove)
			{
				const auto entry = g::entity_list->GetHighestEntityIndex() + 1;
				const auto serial = rand() % 0x1000;

				glove = make_glove(entry, serial);

				wearables[0] = entry | serial << 16;

				// Let's store it in case we somehow lose it.
				glove_handle = wearables[0];
			}

			// Thanks, Beakers
			glove->SetModelIndex(-1);

			apply_config_on_attributable_item(glove, glove_config, player_info.xuid_low);
		}

		return true;
	}

	void handle()
	{
		if (!g::local_player)
			return;

		const auto player_info = g::local_player->GetPlayerInfo();
		if (!handle_glove_config(player_info))
			return;

		const auto weapons = g::local_player->m_hMyWeapons();
		for (size_t i = 0; weapons[i].IsValid(); i++)
		{
			auto* weapon = (c_base_attributable_item*)g::entity_list->GetClientEntityFromHandle(weapons[i]);
			if (!weapon)
				continue;

			const auto knife_index = g::local_player->m_iTeamNum() == team::team_ct ? WEAPON_KNIFE : WEAPON_KNIFE_T;

			auto& definition_index = weapon->m_iItemDefinitionIndex();
			const auto defined_index = is_knife(definition_index) ? knife_index : definition_index;
			const auto active_conf = m_items[defined_index];
			const auto sec_conf = statrack_items[definition_index];
			if (active_conf.enabled)
			{
				apply_config_on_attributable_item(weapon, active_conf, player_info.xuid_low);
				ApplyStatrack(weapon, active_conf, sec_conf);
			}
			else
				erase_override_if_exists_by_index(definition_index);
		}

		const auto view_model = get_entity_from_handle<c_base_view_model>(g::local_player->m_hViewModel());
		if (!view_model)
			return;

		const auto view_model_weapon = get_entity_from_handle<c_base_combat_weapon>(view_model->m_hWeapon());
		if (!view_model_weapon)
			return;

		const auto override_info = get_weapon_info(view_model_weapon->m_iItemDefinitionIndex());
		if (!override_info)
			return;

		const auto override_model_index = g::mdl_info->GetModelIndex(override_info->model);
		view_model->m_nModelIndex() = override_model_index;

		const auto world_model = get_entity_from_handle<c_base_weapon_world_model>(view_model_weapon->m_hWeaponWorldModel());
		if (!world_model)
			return;

		world_model->m_nModelIndex() = override_model_index + 1;
	}

	void load()
	{
		config::load("statrack.json", "", false, [](Json::Value root)
			{
				statrack_items.clear();

				Json::Value it = root["statrack"];
				if (it.isNull())
					return;

				for (auto& item : it)
				{
					auto skin_data = &statrack_items[item["definition_index"].asInt()];
					Option::Load(item["definition_index"], skin_data->definition_index);
					Option::Load(item["stat_track.counter_new"], skin_data->statrack_new.counter);
				}
			});

		config::load("skins.json", "", false, [](Json::Value root)
			{
				m_items.clear();

				Json::Value items = root["skins"];
				if (items.isNull())
					return;

				for (auto& item : items)
				{
					auto skin_data = &m_items[item["definition_index"].asInt()];
					strcpy(skin_data->custom_name, item["custom_name"].asString().c_str());

					Option::Load(item["enabled"], skin_data->enabled);
					Option::Load(item["definition_index"], skin_data->definition_index);
					Option::Load(item["definition_override_index"], skin_data->definition_override_index);
					Option::Load(item["paint_kit_index"], skin_data->paint_kit_index);
					Option::Load(item["wear"], skin_data->wear);
					Option::Load(item["stat_track.enabled"], skin_data->stat_track.enabled);
					Option::Load(item["seed"], skin_data->seed);
				}
			});
	}

	void save()
	{
		config::save("skins.json", "", false, []()
			{
				Json::Value config;

				Json::Value items;
				for (auto& m_item : m_items)
				{
					Json::Value item;

					item["enabled"] = m_item.second.enabled;
					item["custom_name"] = std::string(m_item.second.custom_name);
					item["definition_index"] = m_item.first;
					item["definition_override_index"] = m_item.second.definition_override_index;
					item["paint_kit_index"] = m_item.second.paint_kit_index;
					item["seed"] = m_item.second.seed;
					item["stat_track.enabled"] = m_item.second.stat_track.enabled; //item["stat_track.counter"] = m_item.second.stat_track.counter;
					item["wear"] = m_item.second.wear;

					items.append(item);
				}

				config["skins"] = items;

				return config;
			});
	}

	void save_statrack()
	{
		config::save("statrack.json", "", false, []()
			{
				Json::Value config;

				Json::Value it;
				for (auto& m_item : statrack_items)
				{
					Json::Value item;
					item["definition_index"] = m_item.first;
					item["stat_track.counter_new"] = m_item.second.statrack_new.counter;
					it.append(item);
				}

				config["statrack"] = it;

				return config;
			});
	}
}