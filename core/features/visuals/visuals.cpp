#include "visuals.hpp"
#include "../../zgui/zmenu.hpp"

#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))

bool visuals::get_playerbox(player_t* ent, box& in) {
	vec3_t origin, min, max, flb, brt, blb, frt, frb, brb, blt, flt;
	int left, top, right, bottom;

	origin = ent->abs_origin();
	min = ent->collideable()->mins() + origin;
	max = ent->collideable()->maxs() + origin;

	vec3_t points[] = {
		vec3_t(min.x, min.y, min.z),
		vec3_t(min.x, max.y, min.z),
		vec3_t(max.x, max.y, min.z),
		vec3_t(max.x, min.y, min.z),
		vec3_t(max.x, max.y, max.z),
		vec3_t(min.x, max.y, max.z),
		vec3_t(min.x, min.y, max.z),
		vec3_t(max.x, min.y, max.z)
	};
	if (!interfaces::debug_overlay->world_to_screen(points[3], flb) || !interfaces::debug_overlay->world_to_screen(points[5], brt)
		|| !interfaces::debug_overlay->world_to_screen(points[0], blb) || !interfaces::debug_overlay->world_to_screen(points[4], frt)
		|| !interfaces::debug_overlay->world_to_screen(points[2], frb) || !interfaces::debug_overlay->world_to_screen(points[1], brb)
		|| !interfaces::debug_overlay->world_to_screen(points[6], blt) || !interfaces::debug_overlay->world_to_screen(points[7], flt))
		return false;

	vec3_t arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	for (int i = 1; i < 8; i++) {
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}
	in.x = (int)left;
	in.y = (int)top;
	in.w = int(right - left);
	in.h = int(bottom - top);


	return true;
}

void visuals::draw_box(player_t* entity, visuals::box bbox) {
	
	if (!variables::draw_box) 
		return;

	float alpha[65] = { 255 };

	static float lastDormantTime[65] = { 0 }, lastNonDormantTime[65] = { 0 }; // 64 maxplayers so 1-65 as entindex
	float fadespeed = 1.0; // adding this so you can modify it, fadeout speed in seconds 1.0 = 1s 0.5 = 0.5s etc
	int eid = entity->index(); // calling it once would be a better option
	if (entity->dormant())
	{
		lastDormantTime[eid] = interfaces::globals->realtime;
		alpha[eid] = clamp((lastNonDormantTime[eid] - interfaces::globals->realtime) * (255.f / fadespeed), 0, 255);
	}
	else // you don't need an if here since its either true or false
	{
		lastNonDormantTime[eid] = interfaces::globals->realtime + fadespeed;
		alpha[eid] = clamp((interfaces::globals->realtime - lastDormantTime[eid]) * (255.f / fadespeed), 0, 255);
	}

	render::draw_rect(bbox.x, bbox.y, bbox.w, bbox.h, color(255,255,255, alpha[eid]));
	render::draw_rect(bbox.x - 1, bbox.y - 1, bbox.w + 2, bbox.h + 2, color(0, 0, 0, alpha[eid]));
	render::draw_rect(bbox.x + 1, bbox.y + 1, bbox.w - 2, bbox.h - 2, color(0, 0, 0, alpha[eid]));
}

void visuals::draw_name(player_t* entity, visuals::box bbox) {

	if (!variables::draw_name) 
		return;


	float alpha[65] = { 255 };

	static float lastDormantTime[65] = { 0 }, lastNonDormantTime[65] = { 0 }; // 64 maxplayers so 1-65 as entindex
	float fadespeed = 1.0; // adding this so you can modify it, fadeout speed in seconds 1.0 = 1s 0.5 = 0.5s etc
	int eid = entity->index(); // calling it once would be a better option
	if (entity->dormant())
	{
		lastDormantTime[eid] = interfaces::globals->realtime;
		alpha[eid] = clamp((lastNonDormantTime[eid] - interfaces::globals->realtime) * (255.f / fadespeed), 0, 255);
	}
	else // you don't need an if here since its either true or false
	{
		lastNonDormantTime[eid] = interfaces::globals->realtime + fadespeed;
		alpha[eid] = clamp((interfaces::globals->realtime - lastDormantTime[eid]) * (255.f / fadespeed), 0, 255);
	}
	player_info_t info;

	interfaces::engine->get_player_info(entity->index(), &info);

	render::text(bbox.x + (bbox.w / 2), bbox.y - 15, render::fonts::watermark_font, info.name, true, color(255, 255, 255, alpha[eid]));
}

void visuals::draw_hp(player_t* entity, visuals::box bbox) {

	if (!variables::draw_hp) 
		return;

	float alpha[65] = { 255 };

	static float lastDormantTime[65] = { 0 }, lastNonDormantTime[65] = { 0 }; // 64 maxplayers so 1-65 as entindex
	float fadespeed = 1.0; // adding this so you can modify it, fadeout speed in seconds 1.0 = 1s 0.5 = 0.5s etc
	int eid = entity->index(); // calling it once would be a better option
	if (entity->dormant())
	{
		lastDormantTime[eid] = interfaces::globals->realtime;
		alpha[eid] = clamp((lastNonDormantTime[eid] - interfaces::globals->realtime) * (255.f / fadespeed), 0, 255);
	}
	else // you don't need an if here since its either true or false
	{
		lastNonDormantTime[eid] = interfaces::globals->realtime + fadespeed;
		alpha[eid] = clamp((interfaces::globals->realtime - lastDormantTime[eid]) * (255.f / fadespeed), 0, 255);
	}

	int entity_health = entity->health();

	int hp_bar = bbox.h * (entity_health / 100.f);


	color hp_color = color(0, 0, 0, alpha[eid]);

	if (entity_health >= 51)
		hp_color = color(0, 255, 0, alpha[eid]);
	else if (entity_health <= 50 && entity_health > 10)
		hp_color = color(255, 205, 1, alpha[eid]);
	else if (entity_health <= 10)
		hp_color = color(255, 0, 0, alpha[eid]);

	render::draw_filled_rect(bbox.x - 7, bbox.y - 1, 4, bbox.h + 2, color(35, 35, 35, alpha[eid]));
	render::draw_filled_rect(bbox.x - 6, bbox.y + bbox.h - hp_bar, 2, hp_bar, hp_color);

	if (entity_health < 100)
		render::text(bbox.x - 8, bbox.y + bbox.h - hp_bar - 12, render::fonts::watermark_font, std::to_string(entity->health()), false, color(255, 255, 255, alpha[eid]));
}

void visuals::draw_weapon(player_t* entity, visuals::box bbox) {

	if (!variables::draw_weapon) 
		return;

	float alpha[65] = { 255 };

	static float lastDormantTime[65] = { 0 }, lastNonDormantTime[65] = { 0 }; // 64 maxplayers so 1-65 as entindex
	float fadespeed = 1.0; // adding this so you can modify it, fadeout speed in seconds 1.0 = 1s 0.5 = 0.5s etc
	int eid = entity->index(); // calling it once would be a better option
	if (entity->dormant())
	{
		lastDormantTime[eid] = interfaces::globals->realtime;
		alpha[eid] = clamp((lastNonDormantTime[eid] - interfaces::globals->realtime) * (255.f / fadespeed), 0, 255);
	}
	else // you don't need an if here since its either true or false
	{
		lastNonDormantTime[eid] = interfaces::globals->realtime + fadespeed;
		alpha[eid] = clamp((interfaces::globals->realtime - lastDormantTime[eid]) * (255.f / fadespeed), 0, 255);
	}


	weapon_t* weapon = reinterpret_cast<weapon_t*>(entity->active_weapon());

	if (!weapon) 
		return;

	std::string weapon_name = weapon->get_weapon_data()->weapon_name;

	weapon_name.erase(0, 7);
	std::transform(weapon_name.begin(), weapon_name.end(), weapon_name.begin(), tolower);

	if (variables::draw_ammo) {

		if (weapon->clip1_count() >= 0)
			render::text(bbox.x + (bbox.w / 2), bbox.y + (bbox.h + 6), render::fonts::watermark_font, weapon_name, true, color(255,255,255,alpha[eid]));
		else
			render::text(bbox.x + (bbox.w / 2), bbox.y + (bbox.h), render::fonts::watermark_font, weapon_name, true, color(255,255,255,alpha[eid]));
	}
	else
		render::text(bbox.x + (bbox.w / 2), bbox.y + (bbox.h), render::fonts::watermark_font, weapon_name, true, color(255,255,255, alpha[eid]));

}

void visuals::draw_ammo(player_t* entity, visuals::box bbox) {

	if (!variables::draw_ammo) 
		return;


	float alpha[65] = { 255 };

	static float lastDormantTime[65] = { 0 }, lastNonDormantTime[65] = { 0 }; // 64 maxplayers so 1-65 as entindex
	float fadespeed = 1.0; // adding this so you can modify it, fadeout speed in seconds 1.0 = 1s 0.5 = 0.5s etc
	int eid = entity->index(); // calling it once would be a better option
	if (entity->dormant())
	{
		lastDormantTime[eid] = interfaces::globals->realtime;
		alpha[eid] = clamp((lastNonDormantTime[eid] - interfaces::globals->realtime) * (255.f / fadespeed), 0, 255);
	}
	else // you don't need an if here since its either true or false
	{
		lastNonDormantTime[eid] = interfaces::globals->realtime + fadespeed;
		alpha[eid] = clamp((interfaces::globals->realtime - lastDormantTime[eid]) * (255.f / fadespeed), 0, 255);
	}

	weapon_t* weapon = reinterpret_cast<weapon_t*>(entity->active_weapon());

	if (!weapon) 
		return;

	int weapon_ammo = weapon->clip1_count();

	if (weapon_ammo < 0)
		return;

	int weapon_ammo_bar = weapon_ammo * bbox.w / weapon->get_weapon_data()->weapon_max_clip;

	render::draw_filled_rect(bbox.x - 1, bbox.y + bbox.h + 3, bbox.w + 2, 4, color(0, 0, 0, alpha[eid]));
	render::draw_filled_rect(bbox.x, bbox.y + bbox.h + 4, weapon_ammo_bar, 2, color(255, 0, 0, alpha[eid]));

	if (!(weapon_ammo <= weapon->get_weapon_data()->weapon_max_clip / 4)) return;

	render::text((bbox.x + 10) + weapon_ammo_bar - 6, bbox.y + bbox.h - 2, render::fonts::watermark_font, std::to_string(weapon_ammo), true, color(255, 255, 255, alpha[eid]));

}

void visuals::draw_info(player_t* entity, visuals::box bbox) {

	static int offset;
	offset = 0;

	float alpha[65] = { 255 };

	static float lastDormantTime[65] = { 0 }, lastNonDormantTime[65] = { 0 }; // 64 maxplayers so 1-65 as entindex
	float fadespeed = 1.0; // adding this so you can modify it, fadeout speed in seconds 1.0 = 1s 0.5 = 0.5s etc
	int eid = entity->index(); // calling it once would be a better option
	if (entity->dormant())
	{
		lastDormantTime[eid] = interfaces::globals->realtime;
		alpha[eid] = clamp((lastNonDormantTime[eid] - interfaces::globals->realtime) * (255.f / fadespeed), 0, 255);
	}
	else // you don't need an if here since its either true or false
	{
		lastNonDormantTime[eid] = interfaces::globals->realtime + fadespeed;
		alpha[eid] = clamp((interfaces::globals->realtime - lastDormantTime[eid]) * (255.f / fadespeed), 0, 255);
	}


	player_info_t info;
	interfaces::engine->get_player_info(entity->index(), &info);

	if (variables::draw_bot_check && info.fakeplayer) {
		render::text(bbox.x + bbox.w + 5, bbox.y, render::fonts::watermark_font, "bot", false, color(255, 255, 255, alpha[eid]));
		offset += 11;
	}

	if (variables::draw_zoom_check) {
		if (entity->is_scoped()) {
			render::text(bbox.x + bbox.w + 5, bbox.y + offset, render::fonts::watermark_font, "zoom", false, color(255, 255, 255, alpha[eid]));
			offset += 11;
		}	
	}

	if (variables::draw_money) {	
		std::string money = std::to_string(entity->money()) + "$";
		render::text(bbox.x + bbox.w + 5, bbox.y + offset, render::fonts::watermark_font, money, false, color(255, 200, 0, alpha[eid]));
		offset += 11;
	}

	if (variables::draw_hk) {

		if (entity->armor() > 0 && !entity->has_helmet())
			render::text(bbox.x + bbox.w + 5, bbox.y + offset, render::fonts::watermark_font, "k", false, color(255, 255, 255, alpha[eid]));
		else if (entity->armor() > 0 && entity->has_helmet())
			render::text(bbox.x + bbox.w + 5, bbox.y + offset, render::fonts::watermark_font, "hk", false, color(255, 255, 255, alpha[eid]));
	}

	if (variables::draw_is_flashed && entity->not_flashed() > 75.0) {
		render::text(bbox.x + (bbox.w / 2), bbox.y - 25, render::fonts::watermark_font, "flashed" , true, color(255, 255, 255,alpha[eid]));
	}
}

std::string clean_item_name(std::string name) {
	std::string Name = name;

	auto weapon_start = Name.find("CWeapon");
	if (weapon_start != std::string::npos)
		Name.erase(Name.begin() + weapon_start, Name.begin() + weapon_start + 7);

	if (Name[0] == '_')
		Name.erase(Name.begin());

	if (Name[0] == 'C') //optional for dropped weapons - designer
		Name.erase(Name.begin());

	return Name;
}

void visuals::dropped_weapon_esp(weapon_t* weapon) {

	if (!variables::draw_dropped_weapons)
		return;

	if (!weapon)
		return;

	if (!weapon->is_weapon())
		return;

	vec3_t origin = weapon->origin(), w2s;

	if (origin.x == 0 && origin.y == 0 && origin.z == 0)
		return;

	if (!math::world_to_screen(origin, w2s))
		return;

	int ammo = weapon->clip1_count() * 80 / weapon->get_weapon_data()->weapon_max_clip;

	if (weapon->clip1_count() < 0)
		return;

	std::string weapon_name = clean_item_name(weapon->client_class()->network_name);
	std::transform(weapon_name.begin(), weapon_name.end(), weapon_name.begin(), tolower);

	render::draw_filled_rect(w2s.x - 40, w2s.y + 14, 82, 4, color(15, 15, 15));
	render::draw_filled_rect(w2s.x - 39, w2s.y + 15, ammo, 2, color(255, 255, 255));
	render::text(w2s.x, w2s.y, render::fonts::watermark_font, weapon_name, true, color(255, 255, 255));
}

void visuals::grenades_draw(entity_t* entity) {

	if (!variables::draw_projectile)
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player)
		return;

	if (!entity)
		return;

	vec3_t origin = entity->origin(), w2s;

	if (math::world_to_screen(origin, w2s)) {

		auto class_id = entity->client_class()->class_id;

		switch (class_id) {

		case cbasecsgrenadeprojectile: {

			const model_t* model = entity->model();

			if (!model)
				return;

			studio_hdr_t* hdr = interfaces::model_info->get_studio_model(model);

			if (!hdr)
				return;

			std::string name = hdr->name_char_array;

			if (name.find("incendiarygrenade") != std::string::npos || name.find("fraggrenade") != std::string::npos) {

				render::text(w2s.x, w2s.y, render::fonts::watermark_font, "FRAG", true, color(255, 255, 255));

				break;
			}

			render::text(w2s.x, w2s.y, render::fonts::watermark_font, "FLASH", true, color(255, 255, 255));

		}
		   break;
		case cmolotovprojectile:
		case cinferno: {

				render::text(w2s.x, w2s.y, render::fonts::watermark_font, "FIRE", true, color(255, 255, 255));

			}
					 break;
			case csmokegrenadeprojectile: {
				render::text(w2s.x, w2s.y, render::fonts::watermark_font, "SMOKE", true, color(255, 255, 255));
			}
			break;
			case cdecoyprojectile: {
				render::text(w2s.x, w2s.y, render::fonts::watermark_font, "DECOY", true, color(255, 255, 255));
			}
			break;
			default: break;
			}
		}
	
}

void visuals::spectator_list_draw() {

	if(!variables::draw_spectators)
		return;

	if (!csgo::local_player)
		return;

	auto text_y_offset = 5; // yea
	int screen[2];

	interfaces::engine->get_screen_size(screen[0], screen[1]);

	player_t* spec_player = csgo::local_player->is_alive() ? csgo::local_player : reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity_handle(csgo::local_player->observer_target()));

	if (!spec_player)
		return;

	for (int i = 1; i <= interfaces::globals->max_clients; ++i) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity || entity->dormant() || entity->is_alive() || reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity_handle(entity->observer_target())) != spec_player || entity == csgo::local_player)
			continue;
		player_info_t player_info;

		interfaces::engine->get_player_info(i, &player_info); // bypasing ipv88 and connecting localling to the masterlloser mainfraim

		if (wchar_t name[128]; MultiByteToWideChar(CP_UTF8, 0, player_info.name, -1, name, 128)) {
			int text_width, text_height;
			interfaces::surface->get_text_size(render::fonts::watermark_font, name, text_width, text_height);
			render::text(screen[0] - text_height - 3, text_y_offset, render::fonts::watermark_font, name, false, color(255, 255, 255, 255));
			text_y_offset += text_height + 3;
		}
	}
}

void visuals::chicken_esp(entity_t* chicken) {

	if (!variables::draw_chicken)
		return;

	if (!chicken
		|| chicken == csgo::local_player
		|| chicken->dormant())
		return;

	if (chicken->client_class()->class_id == cchicken) {
		vec3_t origin = chicken->origin(), w2s;

		if (!math::world_to_screen(origin, w2s))
			return;

		render::text(w2s.x, w2s.y, render::fonts::watermark_font, "chicken", true, color(255, 255, 255));
	}
}

void visuals::watermark() {
	int x, y;
	interfaces::engine->get_screen_size(x, y);
	render::draw_filled_rect(x * 0.01, y * 0.01f, 42, 13, color(20, 20, 20, 170));
	render::draw_filled_rect(x * 0.01, y * 0.01f + 13, 42, 2, color(255, 255, 255, 255));
	render::text(x * 0.01f, y * 0.01f, render::fonts::menu_font, "chnware", false, color(255, 255, 255));
}

void visuals::glow_esp()
{
	if (!variables::glow)
		return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;
	if (!csgo::local_player)
		return;
	for (int i = 0; i < interfaces::glow_manager->size; ++i)
	{
		if (interfaces::glow_manager->objects[i].unused() || !interfaces::glow_manager->objects[i].entity)
			continue;

		auto& glowEnt = interfaces::glow_manager->objects[i];
		auto pCSPlayer = reinterpret_cast<player_t*>(glowEnt.entity);
		auto clientclass = reinterpret_cast<c_client_class*>(pCSPlayer->client_class());

		if (!pCSPlayer)
			continue;

		if (pCSPlayer == csgo::local_player)
			continue;

		if (clientclass->class_id == ccsplayer)
		{
			if (pCSPlayer->dormant())
				continue;
			if (!(pCSPlayer->is_alive() && pCSPlayer->health() > 0))
				continue;

			if (pCSPlayer->team() == csgo::local_player->team()) continue;

				glowEnt.set(0.8f, 0.0f, 0.0f, 0.6f);
		}
		if (strstr(clientclass->network_name, "CWeapon") || strstr(clientclass->network_name, "CAK47") || strstr(clientclass->network_name, "CDEAGLE")
			|| strstr(clientclass->network_name, "CFlashbang") || strstr(clientclass->network_name, "CInferno") ||
			strstr(clientclass->network_name, "CDecoyGrenade") || strstr(clientclass->network_name, "CIncendiaryGrenade") || strstr(clientclass->network_name, "CHEGrenade") ||
			strstr(clientclass->network_name, "CSmokeGrenade")) {
			glowEnt.set(0.8f, 0.0f, 0.0f, 1.f);
		}
		if (clientclass->class_id == cc4) {
			glowEnt.set(0.0f, 0.5f, 1.f, 1.f);
		}
		if (clientclass->class_id == cplantedc4) {
			glowEnt.set(0.0f, 0.0f, 1.f, 1.f);
		}
	}
}

void visuals::radar(player_t* entity) {

	if (!entity
		|| !csgo::local_player
		|| !entity->is_alive()
		|| entity == csgo::local_player
		|| entity->dormant())
		return;

	entity->spotted() = variables::radar_spotted ? 1 : 0;
}

void visuals::sniper_crosshair() {
	
	if (variables::engine_crosshair) {

		if (csgo::local_player->is_scoped()) {
			interfaces::console->get_convar("weapon_debug_spread_show")->set_value(0);
			interfaces::console->get_convar("weapon_debug_spread_gap")->set_value(0);
		}
		else {
			interfaces::console->get_convar("weapon_debug_spread_show")->set_value(1);
			interfaces::console->get_convar("weapon_debug_spread_gap")->set_value(2);
		}
	}
	else {
		interfaces::console->get_convar("weapon_debug_spread_show")->set_value(0);
		interfaces::console->get_convar("weapon_debug_spread_gap")->set_value(0);
	}
}

void visuals::skin_changer() {

	convar* skin = interfaces::console->get_convar("r_skin");

	switch (variables::skin_changer) {
	case 0:
		skin->set_value(0); //default
		break;
	case 1:
		skin->set_value(1); //black
		break;
	case 2:
		skin->set_value(2); //mistura
		break;
	case 3:
		skin->set_value(3); //bronze
		break;
	case 4:
		skin->set_value(4); //ariana grande
		break;
	}

}

void visuals::aspecratio() {
	interfaces::console->get_convar("r_aspectratio")->set_value((variables::aspect_ratio * 0.1f) / 2);
}

void visuals::engine_grenade_prediction() {
	interfaces::console->get_convar("cl_grenadepreview")->set_value(variables::grenade_prediction);
}

void visuals::no_flash() {

	if (!csgo::local_player)
		return;

	variables::no_flash ? csgo::local_player->flash_alpha() = 0.f : csgo::local_player->flash_alpha() = 255.f;
}

void visuals::player_esp_render(player_t* entity) {

	if (!entity
		|| !csgo::local_player
		|| !entity->is_alive()
		|| entity == csgo::local_player
		|| entity->team() == csgo::local_player->team())
		return;

	box bbox;
	if (!visuals::get_playerbox(entity, bbox))
		return;

	visuals::radar(entity);

	if (variables::visible_check && csgo::local_player->can_see_player_pos(entity, entity->get_hitbox_position(hitbox_chest))) {
		
		draw_name(entity, bbox);
		
		draw_info(entity, bbox);
		
		draw_box(entity, bbox);
		
		draw_ammo(entity, bbox);
		
		draw_weapon(entity, bbox);
		
		draw_hp(entity, bbox);

	}
	else if (!variables::visible_check) {
		
		draw_name(entity, bbox);
		
		draw_info(entity, bbox);
		
		draw_box(entity, bbox);
		
		draw_ammo(entity, bbox);
		
		draw_weapon(entity, bbox);
		
		draw_hp(entity, bbox);

	}
}

void visuals::player_loop()
{
	for (int i = 1; i <= interfaces::globals->max_clients; ++i) {

		player_t* entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		player_esp_render(entity);

		radar(entity);

		engine_grenade_prediction();

		aspecratio();
	}
}

void visuals::world_loop() {

	for (int i = 1; i <= interfaces::entity_list->get_highest_index(); ++i) {

		entity_t* entity = reinterpret_cast<entity_t*>(interfaces::entity_list->get_client_entity(i));

		dropped_weapon_esp(reinterpret_cast<weapon_t*>(entity));
		
		grenades_draw(entity);
		
		chicken_esp(entity);
	}
}
