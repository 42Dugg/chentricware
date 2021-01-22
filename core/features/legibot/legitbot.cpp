#include "legitbot.hpp"
#include "../../zgui/zmenu.hpp"
#include "../../zgui/zgui.hh"

int legitbot::get_bone_from_menu() {

	static int bone;

	switch (variables::bone) {
	case 0:
		bone = 8;
		break;
	case 1:
		bone = 6;
		break;
	}

	return bone;
}

player_t* legitbot::get_best_target(c_usercmd* cmd)
{
	float ofov = variables::aimbot_fov;
	float nfov = 0;
	player_t* player = nullptr;

	for (int iPlayer = 0; iPlayer < interfaces::globals->max_clients; iPlayer++)
	{
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(iPlayer));
		if (!entity
			|| !csgo::local_player
			|| !entity->is_alive()
			|| entity == csgo::local_player
			|| entity->team() == csgo::local_player->team()
			|| entity->dormant())
			continue;

		vec3_t eye_pos = csgo::local_player->get_eye_pos();
		vec3_t enemy_head_pos = entity->get_bone_position(get_bone_from_menu());
		vec3_t nearest = entity->get_bone_position(get_nearest_bone(entity));
		vec3_t angleTo = math::calculate_angle(eye_pos, enemy_head_pos);
		angleTo.clamp();
		nfov = cmd->viewangles.distance_to(variables::aimbot_rcs ? angleTo - csgo::local_player->aim_punch_angle() * 2.f : angleTo);
		//nfov = cmd->viewangles.distance_to(angleTo);

		if (nfov < ofov)
		{
			ofov = nfov;
			player = entity;
		}
	}
	return player;
}

void legitbot::group_settings() {
	if (!variables::aimbot_check)
		return;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;
	if (!csgo::local_player)
		return;
	if (!csgo::local_player->is_alive())
		return;

	weapon_t* weapon = (weapon_t*)csgo::local_player->active_weapon();

	switch (weapon->get_weapon_data()->weapon_type)
	{
	case WEAPONTYPE_RIFLE:
		variables::aimbot_fov = variables::r_fov;
		variables::aimbot_smoothing = variables::r_smooth;
		break;
	case WEAPONTYPE_SNIPER_RIFLE:
		variables::aimbot_fov = variables::sn_fov;
		variables::aimbot_smoothing = variables::sn_smooth;
		break;
	case WEAPONTYPE_SHOTGUN:
		variables::aimbot_fov = variables::sh_fov;
		variables::aimbot_smoothing = variables::sh_smooth;
		break;
	case WEAPONTYPE_SUBMACHINEGUN:
		variables::aimbot_fov = variables::sm_fov;
		variables::aimbot_smoothing = variables::sm_smooth;
		break;
	case WEAPONTYPE_PISTOL:
		variables::aimbot_fov = variables::p_fov;
		variables::aimbot_smoothing = variables::p_smooth;
		break;
	case WEAPONTYPE_MACHINEGUN:
		variables::aimbot_fov = variables::h_fov;
		variables::aimbot_smoothing = variables::h_smooth;
		break;
	}
}

void legitbot::smoothing(vec3_t& viewangle, vec3_t& angle, float smooth_amount)
{
	angle = (viewangle + (angle - viewangle).clamped() / smooth_amount).clamped();
}

void legitbot::aimbot_chicken(c_usercmd* cmd) {

	if (!variables::chicken_aimbot)
		return;

	if (!GetAsyncKeyState(0x58))
		return;

	for (int i = 1; i <= interfaces::entity_list->get_highest_index(); ++i) {
		entity_t* chicken = static_cast<entity_t*>(interfaces::entity_list->get_client_entity(i));

		if (!chicken
			||!csgo::local_player
			|| chicken == csgo::local_player
			|| chicken->dormant())
			continue;
		
		if (chicken->client_class()->class_id == cchicken) {

				vec3_t eyepos = csgo::local_player->get_eye_pos();
				vec3_t origin = chicken->origin();
				origin.y -= 10;

				vec3_t math = math::calculate_angle(eyepos, origin);
				math.clamp();

				vec3_t local_angle;
				interfaces::engine->get_view_angles(local_angle);
				float super_ok = math::get_fov(local_angle, math);

				if (super_ok <= 25) {
					cmd->viewangles = math;
				}
		}
		else
			continue;
	}
}

void legitbot::aimbot(c_usercmd* cmd)
{

	if (!variables::aimbot_check)
		return;

	auto weapon = csgo::local_player->active_weapon();

	if (!weapon)
		return;

	auto weapon_data = weapon->get_weapon_data();

	if (!weapon_data)
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player)
		return;

	if (!csgo::local_player->is_alive())
		return;

	if (weapon->clip1_count() == 0) 
		return;

	/*if (!(cmd->buttons & in_attack))
		return;*/

	if (!(GetAsyncKeyState(variables::aim_key)))
		return;

	if (variables::aimbot_flashed_check && csgo::local_player->not_flashed() > 75.0)
		return;

	if (zgui::globals::window_ctx.opened)
		return;

	if (weapon_data->weapon_type == WEAPONTYPE_KNIFE 
		|| weapon_data->weapon_type == WEAPONTYPE_C4
		|| weapon_data->weapon_type == WEAPONTYPE_GRENADE)
		return;

	auto target = get_best_target(cmd);

	vec3_t local_viewangles;
	interfaces::engine->get_view_angles(local_viewangles);

	if (target)
	{
		vec3_t eyepos = csgo::local_player->get_eye_pos();
		vec3_t targethead = target->get_bone_position(get_bone_from_menu());
		vec3_t nearest = target->get_bone_position(get_nearest_bone(target));
		vec3_t viewangles = math::calculate_angle(eyepos, targethead);
		viewangles.clamp();
		vec3_t delta = cmd->viewangles - viewangles;
		delta.clamp();	
		vec3_t finalAng = variables::aimbot_rcs ? viewangles - csgo::local_player->aim_punch_angle() * 2.f : viewangles;
		smoothing(local_viewangles, finalAng, variables::aimbot_smoothing);
		finalAng.clamp();

		if (variables::aimbot_is_visible_check && csgo::local_player->can_see_player_pos(target, target->get_bone_position(get_bone_from_menu()))) {
			//cmd->viewangles = finalAng;
			interfaces::engine->set_view_angles(finalAng);
		}
		else if (!variables::aimbot_is_visible_check) {
			//cmd->viewangles = finalAng;
			interfaces::engine->set_view_angles(finalAng);
		}
	}
}

void legitbot::trigger(c_usercmd* cmd) 
{
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!variables::trigger_check)
		return;

	if (GetAsyncKeyState(variables::t_key)) {

		if (zgui::globals::window_ctx.opened)
			return;

		auto weapon = csgo::local_player->active_weapon();
		if (!weapon)
			return;

		auto weapon_data = weapon->get_weapon_data();
		if (!weapon_data)
			return;

		if (weapon_data->weapon_type == WEAPONTYPE_KNIFE
			|| weapon_data->weapon_type == WEAPONTYPE_C4
			|| weapon_data->weapon_type == WEAPONTYPE_GRENADE)
			return;

		if (!csgo::local_player || !csgo::local_player->is_alive())
			return;

		if (!weapon->clip1_count())
			return;

		vec3_t src, dst, forward, crosshair_forward;
		trace_t tr;
		ray_t ray;
		trace_filter filter;

		math::angle_vectors(cmd->viewangles, forward);

		forward *= weapon_data->weapon_range;
		filter.skip = csgo::local_player;
		src = csgo::local_player->get_eye_pos();
		dst = src + forward;
		ray.initialize(src, dst);

		interfaces::trace_ray->trace_ray(ray, MASK_SHOT, &filter, &tr);

		if (variables::trigger_rcs)
			cmd->viewangles -= csgo::local_player->aim_punch_angle() * 2.f;

		math::angle_vectors(cmd->viewangles, crosshair_forward);
		crosshair_forward *= 8000.f;

		if (tr.did_hit_world() || !tr.did_hit_non_world_entity())
			return;

		if (csgo::local_player->team() == tr.entity->team())
			return;

		int hitgroup = tr.hit_group;
		bool didhit = false;

		if (variables::tr_head && hitgroup == hitgroups::hitgroup_head)
			didhit = true;

		if (variables::tr_chest && (hitgroup == hitgroups::hitgroup_chest || hitgroup == hitgroups::hitgroup_stomach))
			didhit = true;

		if (variables::tr_arms && (hitgroup == hitgroups::hitgroup_leftarm || hitgroup == hitgroups::hitgroup_rightarm))
			didhit = true;

		if (variables::tr_legs && (hitgroup == hitgroups::hitgroup_leftleg || hitgroup == hitgroups::hitgroup_rightleg))
			didhit = true;

		if (trigger_delay >= variables::trigger_delay && didhit) //recode  
		{
			trigger_delay = 0;
			cmd->buttons |= in_attack;
		}
		trigger_delay++;
	}
	else if (!(GetAsyncKeyState(variables::t_key)))
		trigger_delay = 0;
}
