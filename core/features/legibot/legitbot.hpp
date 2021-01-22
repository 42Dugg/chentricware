#pragma once
#include "../../../dependencies/utilities/csgo.hpp"

namespace legitbot{
	player_t* get_best_target(c_usercmd* cmd);
	int get_bone_from_menu();
	int get_nearest_bone(player_t* entity);
	int closest_bone(player_t* Entity);
	void group_settings();
	void smoothing(vec3_t& viewangle, vec3_t& angle, float smooth_amount);
	void aimbot_chicken(c_usercmd* cmd);
	void aimbot(c_usercmd* cmd);
	void trigger(c_usercmd* cmd);
	static int trigger_delay = 0;
}