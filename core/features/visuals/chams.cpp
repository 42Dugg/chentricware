#include "chams.hpp"
#include "../../zgui/zmenu.hpp"

//models/props_farm/chicken_bday_hat material for esoterik clown chams

extern hooks::draw_model_execute::fn draw_model_execute_original;

void Chams::override_material(bool ignoreZ, const color& rgba) {

	i_material* material = nullptr;

	material = materialFlat = interfaces::material_system->find_material("debug/debugambientcube",TEXTURE_GROUP_OTHER);

	if (material == nullptr)
		material->increment_reference_count();

	material->set_material_var_flag(material_var_ignorez, ignoreZ);
	material->alpha_modulate(rgba.a / 255.0f);
	material->color_modulate(rgba.r / 255.0f,rgba.g / 255.0f,rgba.b / 255.0f);
	interfaces::model_render->override_material(material);
}

void Chams::on_draw_model_execute(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* matrix)
{

	const auto mdl = info.model;

	bool is_arm = strstr(mdl->name, "arms") != nullptr;
	bool is_player = strstr(mdl->name, "models/player") != nullptr;
	bool is_sleeve = strstr(mdl->name, "sleeve") != nullptr;
	bool is_weapon = strstr(mdl->name, "weapons/v_") != nullptr;

	const auto clr_front = color(100.f, 200.f,75.f);
	const auto clr_back = color(0.f, 150.f, 255.f);

	if (is_player && variables::chams) {

		player_t* player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(info.entity_index));;

		if (player && csgo::local_player && player->is_alive()) {
			if (player->team() == csgo::local_player->team())
				return;

			if (variables::chams_ignore_z) {
				OverrideMaterial(true, clr_back);
				draw_model_execute_original(interfaces::model_render, 0, ctx, state, info, matrix);
				OverrideMaterial(false, clr_front);
			}
			else
				OverrideMaterial(false, clr_front);
		}
	}
} Chams chams; 
