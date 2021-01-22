#pragma once
#include "../../../dependencies/utilities/csgo.hpp"


namespace visuals {

	struct box {
		int x, y, w, h;
		box() = default;
		box(int x, int y, int w, int h) {
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
		}
	};

	static bool is_checked = false;

	bool get_playerbox(player_t* ent, box& in);

	void draw_box(player_t* entity, visuals::box bbox);

	void draw_info(player_t* entity, visuals::box bbox);

	void draw_name(player_t* entity, visuals::box bbox);

	void draw_hp(player_t* entity, visuals::box bbox);

	void draw_ammo(player_t* entity, visuals::box bbox);

	void draw_weapon(player_t* entity, visuals::box bbox);

	void dropped_weapon_esp(weapon_t* weapon);

	void grenades_draw(entity_t* entity);

	void spectator_list_draw();

	void chicken_esp(entity_t* chicken);

	void watermark();

	void radar(player_t* entity);

	void sniper_crosshair();

	void skin_changer();

	void glowesp();

	void engine_grenade_prediction();

	void aspecratio();

	//void nightmode();

	void bomb_defuser_timer(entity_t* entity);

	void no_flash();

	void player_esp_render(player_t* entity);

	void player_loop();

	void world_loop();
}