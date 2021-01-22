#pragma once

struct backtrack_record
{
	player_t* entity;
	vec3_t head;
	vec3_t origin;
};

struct backtrack_tick
{
	int tickcount;
	std::vector<backtrack_record> records;
};

std::vector<backtrack_tick> ticks;
player_t* entity;
vec3_t prevOrig;