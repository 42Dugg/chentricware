#pragma once
#include "../features/features.hpp"
#include "../zgui/zgui.hh"

namespace zmenu {
	void menu();
}

namespace variables {
	//keybinds
	inline int aim_key = 1;
	inline int t_key = 9;


	//visuals (text)
	inline bool visible_check = false;
	inline bool draw_box = false;
	inline bool draw_name = false;
	inline bool draw_weapon = false;
	inline bool draw_ammo = false;
	inline bool draw_hp = false;
	inline bool draw_hk = false;
	inline bool draw_zoom_check = false;
	inline bool draw_spectators = false;
	inline bool draw_money = false;
	inline bool draw_bot_check = false;
	inline bool draw_projectile = false;
	inline bool draw_dropped_weapons = false;
	inline bool draw_chicken = false;
	inline bool draw_is_flashed = false;
	inline bool draw_boom = false;
	inline int skin_changer = 0;
	inline bool engine_crosshair = false;

	//visuals (chams , glow , etc)
	inline bool chams = false;
	inline bool chams_ignore_z = false;
	inline int chams_group = 0;
	inline int chams_group2 = 0;
	inline bool glow = false;
	inline bool no_flash = false;
	inline bool no_smoke = false;
	inline bool radar_spotted = false;
	inline bool grenade_prediction = false;
	inline float aspect_ratio = 0.f;

	//aimbot (checks)
	inline bool aimbot_check = false;
	inline bool aimbot_is_visible_check = true;
	inline bool aimbot_rcs = false;
	inline bool chicken_aimbot = false;
	inline bool aimbot_flashed_check = false;

	//aimbot (settings main)
	inline float aimbot_fov = 0.f;
	inline float aimbot_smoothing = 1.f;
	inline int bone = 0;

	//aimbot (settings for the group of weapons)
	inline float r_fov = 0.f;
	inline float r_smooth = 1.f;
	inline float sm_fov = 0.f;
	inline float sm_smooth = 1.f;
	inline float sn_fov = 0.f;
	inline float sn_smooth = 1.f;
	inline float sh_fov = 0.f;
	inline float sh_smooth = 1.f;
	inline float p_fov = 0.f;
	inline float p_smooth = 1.f;
	inline float h_fov = 0.f;
	inline float h_smooth = 1.f;

	//backtrack 
	inline bool backtrack = false;

	//triggerbot (checks)
	inline bool trigger_check = false;
	inline bool trigger_rcs = false;

	//triggerbot (settings)
	inline int trigger_delay = 0;
	inline bool tr_head = false;
	inline bool tr_legs = false;
	inline bool tr_arms = false;
	inline bool tr_chest = false;


	//misc (movement)
	inline bool bhop = false;
	inline bool skate = false;
	inline bool strafer = false;
	inline bool edge = false;

	//misc (spammers)
	inline bool clan_tag = false;
	inline bool chat_spam = false;
	inline int chat_spam_delay = 60;
	inline std::string chat_spam_input_text = "";

	//colorpicker 
	inline zgui::color box_color{ 255,255,255,255 };
	inline zgui::color weapon_color{ 255,255,255,255 };
	inline zgui::color hk_color{ 255,255,255,255 };
	inline zgui::color zoom_color{ 255,255,255,255 };

}