#include "../features.hpp"
#include "backtrack.h"

void c_backtracking::RegisterTick(c_usercmd* cmd)
{
	ticks.insert(ticks.begin(), backtrack_tick{ cmd->tick_count });
	auto& cur = ticks[0];

	while (ticks.size() > (1.0f / interfaces::globals->interval_per_tick) * 0.2f)
		ticks.pop_back();

	for (int i = 1; i < interfaces::globals->max_clients; i++)
	{
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!entity 
			|| !entity->is_alive()
			|| entity->dormant()
			|| entity->team() == csgo::local_player->team() 
			|| entity->has_gun_game_immunity())
			continue;

		cur.records.emplace_back(backtrack_record{ entity, entity->get_bone_position(8), entity->origin() });
	}
}

void c_backtracking::Begin(c_usercmd* cmd)
{
	entity = nullptr;

	float serverTime = csgo::local_player->get_tick_base() * interfaces::globals->interval_per_tick;
	auto weapon = csgo::local_player->active_weapon();

	if (!weapon)
		return;

	if (cmd->buttons & in_attack && weapon->next_primary_attack() < serverTime + 0.001)
	{
		float fov = 5.f;
		int tickcount = 0;
		bool hasTarget = false;
		vec3_t orig;

		for (auto& tick : ticks)
		{
			for (auto& record : tick.records)
			{
				vec3_t eye_pos = csgo::local_player->get_eye_pos();
				vec3_t angle = math::calculate_angle(eye_pos, record.head);
				float temp_fov = math::get_fov(cmd->viewangles, angle);

				if (temp_fov < fov)
				{
					fov = temp_fov;
					tickcount = tick.tickcount;
					hasTarget = true;
					entity = record.entity;
					orig = record.origin;
				}
			}
		}

		if (entity && hasTarget)
		{
			cmd->tick_count = tickcount;
			prevOrig = entity->origin();
			entity->origin() = orig;
		}
	}
}

void c_backtracking::End()
{
	if (entity)
		entity->origin() = prevOrig;

	entity = nullptr;
}