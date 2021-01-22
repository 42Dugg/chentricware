#include "../../dependencies/utilities/csgo.hpp"
#include "../features/features.hpp"
#include "../features/misc/engine_prediction.hpp"
#include "../features/visuals/visuals.hpp"
#include "../features/legibot/legitbot.hpp"
#include "../zgui/zmenu.hpp"
#include "../zgui/zgui.hh"
#include "../../core/features/visuals/chams.hpp"

hooks::create_move::fn create_move_original = nullptr;
hooks::paint_traverse::fn paint_traverse_original = nullptr;
hooks::draw_model_execute::fn draw_model_execute_original = nullptr;
hooks::do_effect::fn do_effect_original = nullptr;

extern Chams chams;

void line(int x, int y, int x2, int y2, zgui::color c) noexcept { color cc = color(c.r, c.g, c.b, c.a); render::draw_line(x, y, x2, y2, cc); }

void rect(int x, int y, int x2, int y2, zgui::color c) noexcept { color cc = color(c.r, c.g, c.b, c.a); render::draw_rect(x, y, x2, y2, cc); }

void filled_rect(int x, int y, int x2, int y2, zgui::color c) noexcept { color cc = color(c.r, c.g, c.b, c.a); render::draw_filled_rect(x, y, x2, y2, cc); }

void text(int x, int y, zgui::color c, int font, bool center, const char* text) noexcept { color cc = color(c.r, c.g, c.b, c.a); render::text(x, y, font, text, center, cc); }

void get_text_size(unsigned long font, const char* text, int& wide, int& tall) noexcept
{

	std::string str = text;
	std::wstring wstr(str.begin(), str.end());

	interfaces::surface->get_text_size(font, wstr.c_str(), wide, tall);
}

float get_frametime() noexcept { return interfaces::globals->frame_time; }

bool hooks::initialize() {

	zgui::functions.draw_line = line;
	zgui::functions.draw_rect = rect;
	zgui::functions.draw_filled_rect = filled_rect;
	zgui::functions.draw_text = text;
	zgui::functions.get_text_size = get_text_size;
	zgui::functions.get_frametime = get_frametime;

	const auto create_move_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 24));
	const auto paint_traverse_target = reinterpret_cast<void*>(get_virtual(interfaces::panel, 41));
	const auto draw_model_target = reinterpret_cast<void*>(get_virtual(interfaces::model_render, 21));
	const auto do_effect_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 44));
	
	if (MH_Initialize() != MH_OK)
		throw std::runtime_error("failed to initialize MH_Initialize.");

	if (MH_CreateHook(create_move_target, &create_move::hook, reinterpret_cast<void**>(&create_move_original)) != MH_OK)
		throw std::runtime_error("failed to initialize create_move. (outdated index?)");

	if (MH_CreateHook(paint_traverse_target, &paint_traverse::hook, reinterpret_cast<void**>(&paint_traverse_original)) != MH_OK)
		throw std::runtime_error("failed to initialize paint_traverse. (outdated index?)");

	if (MH_CreateHook(draw_model_target, &draw_model_execute::hook, reinterpret_cast<void**>(&draw_model_execute_original)) != MH_OK)
		throw std::runtime_error("failed to initialize draw_model. (outdated index?)");

	if (MH_CreateHook(do_effect_target, &do_effect::hook, reinterpret_cast<void**>(&do_effect_original)) != MH_OK)
		throw std::runtime_error("failed to initialize do_effect. (outdated index?)");


	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		throw std::runtime_error("failed to enable hooks.");

	console::log("[setup] hooks initialized!\n");
	return true;
}

void hooks::release() {
	MH_Uninitialize();

	MH_DisableHook(MH_ALL_HOOKS);
}

bool __stdcall hooks::create_move::hook(float input_sample_frametime, c_usercmd* cmd) {
	create_move_original(input_sample_frametime, cmd);

	if (!cmd || !cmd->command_number)
		return false;

	csgo::local_player = static_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	uintptr_t* frame_pointer;
	__asm mov frame_pointer, ebp;
	bool& send_packet = *reinterpret_cast<bool*>(*frame_pointer - 0x1C);

	if (variables::backtrack && interfaces::engine->is_in_game() && interfaces::engine->is_connected() && csgo::local_player->is_alive())
	{
		if (cmd->command_number)
		{
			c_backtracking::RegisterTick(cmd);
		}
		c_backtracking::Begin(cmd);
		c_backtracking::End();
	}

	misc::movement::movement_related(cmd);
	misc::spam::spam_related();

//	legitbot::aimbot(cmd);
	legitbot::trigger(cmd);
	legitbot::group_settings();
	prediction::start(cmd); {
		legitbot::aimbot(cmd);
		misc::movement::edge_jump(cmd);

	} prediction::end();

	auto old_viewangles = cmd->viewangles;
	auto old_forwardmove = cmd->forwardmove;
	auto old_sidemove = cmd->sidemove;

	math::correct_movement(old_viewangles, cmd, old_forwardmove, old_sidemove);

	cmd->forwardmove = std::clamp(cmd->forwardmove, -450.0f, 450.0f);
	cmd->sidemove = std::clamp(cmd->sidemove, -450.0f, 450.0f);
	cmd->upmove = std::clamp(cmd->upmove, -320.0f, 320.0f);

	cmd->viewangles.normalize();
	cmd->viewangles.x = std::clamp(cmd->viewangles.x, -89.0f, 89.0f);
	cmd->viewangles.y = std::clamp(cmd->viewangles.y, -180.0f, 180.0f);
	cmd->viewangles.z = 0.0f;

	return false;
}

void __stdcall hooks::paint_traverse::hook(unsigned int panel, bool force_repaint, bool allow_force) {
	auto panel_to_draw = fnv::hash(interfaces::panel->get_panel_name(panel));	

	switch (panel_to_draw) {
	case fnv::hash("MatSystemTopPanel"):

		visuals::world_loop();
		visuals::player_loop();
		visuals::watermark();
		visuals::no_flash();
		visuals::aspecratio();
		visuals::spectator_list_draw();
		visuals::sniper_crosshair();
		visuals::skin_changer();
		zmenu::menu();

		break;

	case fnv::hash("FocusOverlayPanel"):
		//interfaces::panel->set_keyboard_input_enabled(panel, zgui::globals::window_ctx.opened);
		interfaces::panel->set_mouse_input_enabled(panel, zgui::globals::window_ctx.opened);
		break;
	}

	paint_traverse_original(interfaces::panel, panel, force_repaint, allow_force);
}

void __fastcall hooks::draw_model_execute::hook(void* _this, int edx, i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* matrix) {

	if (interfaces::engine->is_in_game() && interfaces::engine->is_connected() && csgo::local_player) {

		const auto mdl = info.model;

		if (!mdl)
			return;

		bool is_player = strstr(mdl->name, "models/player") != nullptr;


		if (!interfaces::model_render->is_forced() && is_player) {
			chams.OnDrawModelExecute(ctx, state, info, matrix);
			draw_model_execute_original(_this, edx, ctx, state, info, matrix);
			interfaces::model_render->override_material(nullptr);
		}
		else
			draw_model_execute_original(_this, edx, ctx, state, info, matrix);
	}
}


/*const char* material_name() {

	switch (variables::chams_group) {
	case 0:
		return "debug/debugambientcube";
		break;
	case 1:
		return "models/props_farm/chicken_bday_hat";
		break;
	}
}*/

int __fastcall hooks::do_effect::hook(void* ecx, void* edx, int a1) {
	visuals::glowesp();
	return do_effect_original(ecx, edx, a1);
}
