#include "patterns.h"

#include "../helpers/utils.h"

namespace patterns
{
	void initialize()
	{
		set_sky_addr = utils::pattern_scan(SET_SKY);
		find_hud_addr = utils::pattern_scan(FIND_HUD);
		set_clan_tag_addr = utils::pattern_scan(SET_CLAN_TAG);
		update_visibility_entities_addr = utils::pattern_scan(UPDATE_VISIBILITY_ENTITIES);
		set_abs_origin_addr = utils::pattern_scan(SET_ABS_ORIGIN);
		set_abs_angle_addr = utils::pattern_scan(SET_ABS_ANGLE);
		invalidate_bonecache_addr = utils::pattern_scan(INVALIDATE_BONE_CACHE);
		in_reload_addr = utils::pattern_scan(INRELOAD);
		current_command_addr = utils::pattern_scan(CURRENT_COMMAND);
		has_c4_addr = utils::pattern_scan(HAS_C4);
		get_sequence_activity_addr = utils::pattern_scan(GET_SEQUENCE_ACTIVITY);
		is_line_goes_through_smoke_addr = utils::pattern_scan(IS_LINE_GOES_THROUGH_SMOKE);
		is_line_goes_through_smoke_fn_addr = utils::pattern_scan(IS_LINE_GOES_THROUGH_SMOKE_FN);
		reveal_ranks_addr = utils::pattern_scan(REVEAL_RANKS);
	}
}