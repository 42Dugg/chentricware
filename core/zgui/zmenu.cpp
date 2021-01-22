#include "zmenu.hpp"
#include "../features/visuals/visuals.hpp"


static int main_tab{ 0 };
static int tab{ 0 };
static int weapon_group_tab{ 0 };
static int visual_tab{ 0 };
static int misc_tab{ 0 };

std::vector<std::string> ok2{ "normal","clown" };
std::vector<std::string> ok{ "normal","clown" };

void zmenu::menu() {

	zgui::poll_input("Counter-Strike: Global Offensive");

	if (zgui::begin_window("chnware", { 500, 350 }, render::fonts::menu_font)) {
		
		/*if (zgui::button("aimbot", { 50,30 })) { tab = 0; }
		zgui::same_line();
		if (zgui::button("visuals", { 50,30 })) { tab = 1; }
		zgui::same_line();
		if (zgui::button("misc", { 50,30 })) { tab = 2; }
		zgui::same_line();
		if (zgui::button("beta", { 50,30 })) { tab = 3; }

		switch (tab)
		{
		case 0:
			zgui::dummy();
			zgui::backup_line();
			
					zgui::checkbox("enable", variables::aimbot_check);
					if (variables::aimbot_check) {
						zgui::backup_line();
						zgui::key_bind("#aimbotkey", variables::aim_key);
						zgui::checkbox("aimbot rcs", variables::aimbot_rcs);
					}
					zgui::backup_line();
					zgui::checkbox("visible check", variables::aimbot_is_visible_check);
					zgui::checkbox("backtrack", variables::backtrack);
					zgui::checkbox("trigger", variables::trigger_check);
					if (variables::trigger_check) {
						zgui::backup_line();
						zgui::checkbox("trigger rcs", variables::trigger_rcs);
						zgui::key_bind("#triggerkey", variables::t_key);
						zgui::slider_int("trigger delay", 0, 50, variables::trigger_delay);
					}
			
					zgui::backup_line();
					if (zgui::clickable_text("rifle")) { weapon_group_tab = 0; }
					zgui::same_line();
					if (zgui::clickable_text("pistol")) { weapon_group_tab = 1; }
					zgui::same_line();
					if (zgui::clickable_text("sniper")) { weapon_group_tab = 2; }
					zgui::same_line();
					if (zgui::clickable_text("smg")) { weapon_group_tab = 3; }
					zgui::same_line();
					if (zgui::clickable_text("heavy")) { weapon_group_tab = 4; }
					zgui::same_line();
					if (zgui::clickable_text("shotgun")) { weapon_group_tab = 5; }


					switch (weapon_group_tab)
					{
					case 0:
						zgui::backup_line();
						zgui::slider_float("rifle fov", 0.f, 10.f, variables::r_fov);
						zgui::slider_float("rifle smooth", 1.f, 10.f, variables::r_smooth);
						break;
					case 1:
						zgui::backup_line();
						zgui::slider_float("pistol fov", 0.f, 10.f, variables::p_fov);
						zgui::slider_float("pistol smooth", 1.f, 10.f, variables::p_smooth);
						break;
					case 2:
						zgui::backup_line();
						zgui::slider_float("sniper fov", 0.f, 10.f, variables::sn_fov);
						zgui::slider_float("sniper smooth", 1.f, 10.f, variables::sn_smooth);
						break;
					case 3:
						zgui::backup_line();
						zgui::slider_float("smg fov", 0.20f, 10.f, variables::sm_fov);
						zgui::slider_float("smg smooth", 1.f, 10.f, variables::sm_smooth);
						break;
					case 4:
						zgui::backup_line();
						zgui::slider_float("heavy fov", 0.f, 10.f, variables::h_fov);
						zgui::slider_float("heavy smooth", 1.f, 10.f, variables::h_smooth);
						break;
					case 5:
						zgui::backup_line();
						zgui::slider_float("shotgun fov", 0.f, 10.f, variables::sh_fov);
						zgui::slider_float("shotgun smooth", 1.f, 10.f, variables::sh_smooth);
						break;
					}
						
			break;
		case 1:
			zgui::dummy();
			zgui::backup_line();
		
				zgui::checkbox("visible", variables::visible_check);
				zgui::checkbox("radar", variables::radar_spotted);
				//zgui::checkbox("draw backtrack", variables::drawbacktrack);
				zgui::checkbox("box", variables::draw_box);
				//	zgui::colorpicker("box color",variables::box_color);
				zgui::checkbox("name", variables::draw_name);
				zgui::checkbox("bot check", variables::draw_bot_check);
				zgui::checkbox("weapon", variables::draw_weapon);
				//	zgui::colorpicker("weapon color", variables::weapon_color);
				zgui::checkbox("ammo", variables::draw_ammo);
				zgui::checkbox("hp", variables::draw_hp);
				zgui::checkbox("hk", variables::draw_hk);
				//	zgui::colorpicker("hk color", variables::hk_color);
				zgui::checkbox("zoom check", variables::draw_zoom_check);
				zgui::checkbox("money", variables::draw_money);
				//zgui::colorpicker("zoom color", variables::zoom_color);
				zgui::checkbox("chams", variables::chams);
				if(variables::chams)
					zgui::checkbox("ignorez", variables::chams_ignore_z);
				zgui::checkbox("glow", variables::glow);
				zgui::checkbox("grenade prediction", variables::grenade_prediction);
				zgui::checkbox("projectiles esp", variables::draw_projectile);
				zgui::checkbox("dropped weapons esp", variables::draw_dropped_weapons);
			break;
		case 2:
			zgui::dummy();
			zgui::backup_line();
			//zgui::checkbox("noite mode", variables::nm);
			zgui::checkbox("chiken esp", variables::draw_chicken);
			zgui::checkbox("spectator list", variables::draw_spectators);
			zgui::checkbox("skate movement effect", variables::skate);
			zgui::checkbox("bhop", variables::bhop);
			zgui::checkbox("manual strafer helper", variables::strafer);
			//zgui::checkbox("strafer", variables::strafer);
			zgui::checkbox("edge jump", variables::edge);
			zgui::checkbox("clan tag", variables::clan_tag);
			zgui::checkbox("chat pam", variables::chat_spam);
			if (variables::chat_spam) {
				zgui::text_input("#superid", variables::chat_spam_input_text);
				zgui::slider_int("delay", 60, 600, variables::chat_spam_delay);
			}
			zgui::slider_float("aspectio", 0, 100.f, variables::aspect_ratio);
			zgui::checkbox("no flash", variables::no_flash);
			zgui::checkbox("no smoke", variables::no_smoke);
		if (zgui::button("music", { 50,50 })) {
				misc::sixnine();
			}
			//zgui::slider_float("x menu", 0, 50, variables::x_m);
			//zgui::slider_int("y menu", 0, 1000, variables::y_m);
			break;
		case 3:
			zgui::dummy();
			zgui::backup_line();
			zgui::combobox("cham z type", ok2, variables::chams_group2);
			zgui::combobox("cham type", ok, variables::chams_group);
			break;
		}*/


		switch (main_tab) {
		case 0:
			zgui::dummy();
			zgui::push_line();
			if (zgui::button("aimbot", { 50,50 })) 
				main_tab = 1;
			zgui::same_line();
			 if (zgui::button("visuals", { 50,50 }))
				main_tab = 2;
			 zgui::same_line();
			 if (zgui::button("misc", { 50,50 }))
				main_tab = 3;
			 zgui::dummy();
			 zgui::dummy();
			 zgui::backup_line();
			 zgui::text("changelog - recode");
			break;
		case 1:
			switch (tab) {
			case 0:
				if (zgui::clickable_text("<-"))
					main_tab = 0;
				zgui::text("aimbot - main");
				if (zgui::clickable_text("aimbot"))
					tab = 1;
				if (zgui::clickable_text("triggerbot"))
					tab = 2;
				if (zgui::clickable_text("backtrack"))
					tab = 3;
				break;
			case 1:
				if (zgui::clickable_text("<-"))
					tab = 0;
				zgui::backup_line();
				zgui::text("aimbot");
				zgui::checkbox("enable", variables::aimbot_check);
				if (variables::aimbot_check) {
					zgui::backup_line();
					zgui::key_bind("#aimbotkey", variables::aim_key);
					zgui::checkbox("aimbot rcs", variables::aimbot_rcs);
					zgui::checkbox("visible check", variables::aimbot_is_visible_check);
					zgui::checkbox("flashed check (local player) (yourself)", variables::aimbot_flashed_check);
					zgui::combobox("hitboxes", std::vector<std::string>{ {"head"}, { "chest" }},variables::bone);

					zgui::backup_line();
					if (zgui::clickable_text("rifle")) { weapon_group_tab = 0; }
					zgui::same_line();
					if (zgui::clickable_text("pistol")) { weapon_group_tab = 1; }
					zgui::same_line();
					if (zgui::clickable_text("sniper")) { weapon_group_tab = 2; }
					zgui::same_line();
					if (zgui::clickable_text("smg")) { weapon_group_tab = 3; }
					zgui::same_line();
					if (zgui::clickable_text("heavy")) { weapon_group_tab = 4; }
					zgui::same_line();
					if (zgui::clickable_text("shotgun")) { weapon_group_tab = 5; }


					switch (weapon_group_tab)
					{
					case 0:
						zgui::backup_line();
						zgui::slider_float("rifle fov", 0.f, 10.f, variables::r_fov);
						zgui::slider_float("rifle smooth", 1.f, 10.f, variables::r_smooth);
						break;
					case 1:
						zgui::backup_line();
						zgui::slider_float("pistol fov", 0.f, 10.f, variables::p_fov);
						zgui::slider_float("pistol smooth", 1.f, 10.f, variables::p_smooth);
						break;
					case 2:
						zgui::backup_line();
						zgui::slider_float("sniper fov", 0.f, 10.f, variables::sn_fov);
						zgui::slider_float("sniper smooth", 1.f, 10.f, variables::sn_smooth);
						break;
					case 3:
						zgui::backup_line();
						zgui::slider_float("smg fov", 0.20f, 10.f, variables::sm_fov);
						zgui::slider_float("smg smooth", 1.f, 10.f, variables::sm_smooth);
						break;
					case 4:
						zgui::backup_line();
						zgui::slider_float("heavy fov", 0.f, 10.f, variables::h_fov);
						zgui::slider_float("heavy smooth", 1.f, 10.f, variables::h_smooth);
						break;
					case 5:
						zgui::backup_line();
						zgui::slider_float("shotgun fov", 0.f, 10.f, variables::sh_fov);
						zgui::slider_float("shotgun smooth", 1.f, 10.f, variables::sh_smooth);
						break;
					}

				}
				break;
			case 2:
				if (zgui::clickable_text("<-"))
					tab = 0;
				zgui::backup_line();
				zgui::text("triggerbot");
				zgui::checkbox("trigger", variables::trigger_check);
				if (variables::trigger_check) {
					zgui::backup_line();
					zgui::checkbox("trigger rcs", variables::trigger_rcs);
					zgui::key_bind("#triggerkey", variables::t_key);
					zgui::slider_int("trigger delay", 0, 50, variables::trigger_delay);
					zgui::multi_combobox("hitboxes", std::vector< zgui::multi_select_item >{ { "head", & variables::tr_head }, { "chest", &variables::tr_chest }, { "arms", &variables::tr_arms }, { "legs", &variables::tr_legs }});
				}
				break;
			case 3:
				if (zgui::clickable_text("<-"))
					tab = 0;
				zgui::text("backtrack");
				zgui::checkbox("backtrack", variables::backtrack);
				break;
			}

			break;
		case 2:
			switch (visual_tab) {
			case 0:
				if (zgui::clickable_text("<-"))
					main_tab = 0;
				zgui::text("visuals - main");
				if (zgui::clickable_text("players"))
					visual_tab = 1;
				if (zgui::clickable_text("world"))
					visual_tab = 2;
				break;
			case 1:
				if (zgui::clickable_text("<-"))
					visual_tab = 0;
				zgui::text("players");
				zgui::checkbox("visible", variables::visible_check);
				zgui::checkbox("radar", variables::radar_spotted);
				zgui::checkbox("box", variables::draw_box);
				zgui::checkbox("name", variables::draw_name);
				zgui::checkbox("weapon", variables::draw_weapon);
				zgui::checkbox("ammo", variables::draw_ammo);
				zgui::checkbox("hp", variables::draw_hp);
				zgui::multi_combobox("flags", std::vector< zgui::multi_select_item >{ { "bot", & variables::draw_bot_check }, { "zoom", &variables::draw_zoom_check }, { "money", &variables::draw_money }, { "armor", &variables::draw_hk }, { "flashed", &variables::draw_is_flashed }});
				zgui::checkbox("chams", variables::chams);
				if (variables::chams)
					zgui::checkbox("ignorez", variables::chams_ignore_z);
				zgui::checkbox("glow", variables::glow);
				break;
			case 2:
				if (zgui::clickable_text("<-"))
					visual_tab = 0;
				zgui::text("world");
				zgui::checkbox("c4 & defuser timer", variables::draw_boom);
				zgui::checkbox("projectiles esp", variables::draw_projectile);
				zgui::checkbox("dropped weapons esp", variables::draw_dropped_weapons);
				zgui::checkbox("grenade prediction", variables::grenade_prediction);
				break;
			}
			break;
		case 3:
			switch (misc_tab) {
			case 0:
				if (zgui::clickable_text("<-"))
					main_tab = 0;
				zgui::text("misc - main");
				if (zgui::clickable_text("movement"))
					misc_tab = 1;
				if (zgui::clickable_text("spam"))
					misc_tab = 2;
				if (zgui::clickable_text("visuals related"))
					misc_tab = 3;
				break;
			case 1:
				if (zgui::clickable_text("<-"))
					misc_tab = 0;
				zgui::text("movement");
				zgui::checkbox("bhop", variables::bhop);
				zgui::checkbox("strafer helper", variables::strafer);
				zgui::checkbox("skate movement effect", variables::skate);
				zgui::checkbox("edge jump", variables::edge);
				break;
			case 2:
				if (zgui::clickable_text("<-"))
					misc_tab = 0;
				zgui::text("spam");
				zgui::checkbox("clan tag", variables::clan_tag);
				zgui::checkbox("chat pam", variables::chat_spam);
				if (variables::chat_spam) {
					zgui::text_input("#superid", variables::chat_spam_input_text);
					zgui::slider_int("delay", 60, 600, variables::chat_spam_delay);
				}
				break;
			case 3:
				if (zgui::clickable_text("<-"))
					misc_tab = 0;
				zgui::text("visuals related");
				zgui::combobox("player skin changer", std::vector<std::string>{ {"default"}, { "black" }, { "mixed" }, { "white" }, { "tan" }}, variables::skin_changer);
				zgui::checkbox("draw chicken", variables::draw_chicken);
				zgui::checkbox("spectator list", variables::draw_spectators);
				zgui::checkbox("engine crosshair", variables::engine_crosshair);
				zgui::slider_float("aspectio", 0, 100.f, variables::aspect_ratio);
				zgui::checkbox("no flash", variables::no_flash);
				break;
			}
		}
	


		zgui::end_window();
	}
}
