#include "../features.hpp"
#include "engine_prediction.hpp"
#include "../../zgui/zmenu.hpp"
#include <windows.h>
#include <shellapi.h>

void misc::movement::bunny_hop(c_usercmd* cmd) {

	if (!variables::bhop) return;

	auto move_type = csgo::local_player->move_type();

	if (move_type == movetype_ladder) return;

	if (!(csgo::local_player->flags() & fl_onground))
		cmd->buttons &= ~in_jump;
};

void misc::movement::edge_jump(c_usercmd* cmd) {

	if (!GetAsyncKeyState(0x58)) return;
	if (!variables::edge) return;

	auto move_type = csgo::local_player->move_type();

	if (move_type == movetype_ladder) return;

	if ((prediction::get_flags() & 1) && !(csgo::local_player->flags() & 1))
		cmd->buttons |= in_jump;

}

template<class T, class U>
inline T clamp_value(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}

static vec_t normalize_yaw(vec_t ang)
{
	while (ang < -180.0f)
		ang += 360.0f;
	while (ang > 180.0f)
		ang -= 360.0f;
	return ang;
}

void misc::movement::strafer(c_usercmd* user_cmd) {

	if (!variables::strafer) return;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	if (!interfaces::inputsystem->is_button_down(button_code_t::KEY_SPACE))
		return;

	if (interfaces::inputsystem->is_button_down(button_code_t::KEY_S) || interfaces::inputsystem->is_button_down(button_code_t::KEY_D) || interfaces::inputsystem->is_button_down(button_code_t::KEY_A) || interfaces::inputsystem->is_button_down(button_code_t::KEY_W))
		return;

	if (csgo::local_player->move_type() == movetype_noclip || csgo::local_player->move_type() == movetype_ladder)
		return;

	if (!(csgo::local_player->flags() & fl_onground))
	{
		if (user_cmd->mousedx > 1 || user_cmd->mousedx < -1)
			user_cmd->sidemove = user_cmd->mousedx < 0.f ? -450.0f : 450.0f;
		else
		{
			user_cmd->forwardmove = std::clamp(10000.f / csgo::local_player->velocity().length_2d(), -450.f, 450.f);
			user_cmd->sidemove = user_cmd->command_number % 2 == 0 ? -450.f : 450.f;
		}
	}
}

void misc::movement::skate(c_usercmd* cmd) {

	if (!variables::skate) return;

	if (csgo::local_player && csgo::local_player->move_type() != movetype_ladder)
		cmd->buttons ^= in_forward | in_back | in_moveleft | in_moveright;
}

void SendClanTag(const char* szClanTag, const char* szIdentifier = "")
{
	using SendClanTagFn = void(__fastcall*)(const char*, const char*);
	static auto oSendClanTag = reinterpret_cast<SendClanTagFn>(utilities::pattern_scan("engine.dll", "53 56 57 8B DA 8B F9 FF 15"));

	if (oSendClanTag != nullptr)
		oSendClanTag(szClanTag, szIdentifier);
}

void misc::spam::clan_tag()
{
	if (variables::clan_tag) {
		static int counter = 0;
		static std::string clantag = "  chnware  ";
		if (++counter > 30) {
			rotate(clantag.begin(), clantag.begin() + 1, clantag.end());
			SendClanTag(clantag.c_str());
			counter = 0;
		}
	}
}

void misc::spam::chat_spam() {
	if (!variables::chat_spam) return;

	static int counter = 0;
	if (++counter >= variables::chat_spam_delay) {
		std::string cSpam = "say " + variables::chat_spam_input_text;

		if (variables::chat_spam_input_text == "") 
			return;

		interfaces::engine->execute_cmd(cSpam.c_str());
		counter = 0;
	}
}

void misc::movement::movement_related(c_usercmd* cmd) {
	bunny_hop(cmd);
	strafer(cmd);
	skate(cmd);
}

void misc::spam::spam_related() {
	chat_spam();
	clan_tag();
}
