

// #===================================================================================#
// +                                                                                   +
// +  _   _   ____  ________ _____                                        (_)          +
// + | \ | | / __ \|__   __||  ____|  DO NOT edit    ____   __ _   _   _   _           +
// + |  \| || |  | |  | |   | |__    this file, it  |_  /  / _` | | | | | | |          +
// + | . ` || |  | |  | |   |  __|   was created by: / /  | (_| | | |_| | | |          +
// + | |\  || |__| |  | |   | |____                 /___|  \__, |  \__,_| |_|          +
// + |_| \_| \____/   |_|   |______|                        __/ |                      +
// +                                                       |___/    zgui_packer        +
// +                                                             by danielkrupinski    + 
// +   (https://github.com/zxvnme/zgui/blob/master/packer.py)       and zxvnme         +      
// #===================================================================================#

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cctype>
#include <Windows.h>

#include "./zgui.hh"
#include <functional>


using namespace zgui::globals;
zgui::functions_t zgui::functions;
zgui::gui_window_context_t zgui::globals::window_ctx;

// ========================================================================
bool zgui::button(const char *id, const vec2 size) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    const vec2 draw_pos{window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y};

    const bool active = window_ctx.blocking == utils::hash::hash(id);

    bool result = false;
    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, size.x, size.y); !active && hovered && utils::input::key_pressed(VK_LBUTTON)) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (active && !utils::input::key_down(VK_LBUTTON)) {
        window_ctx.blocking = 0;
        result = hovered;
    }

    int text_width, text_height;
    functions.get_text_size(font, id_split[0].c_str(), text_width, text_height);

    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + size.x / 2 - text_width / 2, draw_pos.y + size.y / 2 - text_height / 2}, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0, 0}, font});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, active ? global_colors.control_active_or_clicked : global_colors.control_idle, "", {size.x - 2, size.y - 2}});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", size});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + size.x + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + size.y / 2 + global_config.item_spacing});

    utils::misc::push_font(font);

    return result;
}
// ========================================================================



// ========================================================================
void zgui::checkbox(const char *id, bool &value) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const int control_height = 8;
    const int control_width = 8;

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    const vec2 draw_pos{window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y};

    int text_width, text_height;
    functions.get_text_size(font, id_split[0].c_str(), text_width, text_height);

    const bool active = window_ctx.blocking == utils::hash::hash(id);

    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width + 6 + text_width, control_height); !active && hovered && utils::input::key_pressed(VK_LBUTTON)) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (active && !utils::input::key_down(VK_LBUTTON)) {
        window_ctx.blocking = 0;
        value = !value;
    }

    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 14, draw_pos.y - 2}, zgui_render_type::zgui_text, value ? global_colors.color_text : global_colors.color_text, id_split[0], vec2{0, 0}, font});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, value ? global_colors.control_active_or_clicked : global_colors.control_idle, "", {control_width - 2, control_height - 2}});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {control_width, control_height}});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + 14 + text_width + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + global_config.item_spacing});

    utils::misc::push_font(font);
}
// ========================================================================

// ========================================================================
void zgui::combobox(const char *id, std::vector<std::string> items, int &value) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const int control_width = 70;
    const int control_height = 20;

    value = std::clamp(value, 0, static_cast<int>(items.size()) - 1);

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 draw_pos{window_ctx.position.x + cursor_pos.x + 14, window_ctx.position.y + cursor_pos.y};

    const bool inlined = id_split[0].empty();

    if (!inlined) {
        int text_width, text_height;

        functions.get_text_size(font, id_split[0].c_str(), text_width, text_height);

        window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y - 4}, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0, 0}, font});

        draw_pos.y += text_height;
    }

    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + control_width - 10, draw_pos.y + 4}, zgui_render_type::zgui_text, global_colors.color_text, "+", vec2{0, 0}, font});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 4, draw_pos.y + 4}, zgui_render_type::zgui_text, global_colors.color_text, items.at(value), vec2{0, 0}, font});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, global_colors.control_idle, "", {control_width - 2, control_height - 2}});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {control_width, control_height}});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + control_width + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12)});


    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && utils::input::key_pressed(VK_LBUTTON) && window_ctx.blocking == 0) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (window_ctx.blocking == utils::hash::hash(id)) {
        for (int i = 1; i <= items.size(); i++) {
            bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y + (control_height - 1) * i, control_width, control_height);

            if (hovered && utils::input::key_pressed(VK_LBUTTON)) {
                window_ctx.blocking = 0;
                value = i - 1;
            }

            if (!hovered && utils::input::key_pressed(VK_LBUTTON)) {
                window_ctx.blocking = 0;
            }
            bool selected = value == i - 1;
            window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x + 4, draw_pos.y + (control_height - 1) * i + 4}, zgui_render_type::zgui_text, selected ? color{0,150,255,255} : global_colors.color_text, items.at(i - 1), vec2{0, 0}, font });
            window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 1, draw_pos.y + (19 * i) + 1}, zgui_render_type::zgui_filled_rect, hovered ? global_colors.color_combo_bg : global_colors.control_idle, "", {control_width - 2, control_height - 2}});
            window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y + 19 * i}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {control_width, control_height}});
        }
    }

    utils::misc::push_font(font);
}

// ========================================================================
void zgui::multi_combobox(const char *id, std::vector<multi_select_item> items) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const int control_width = 100;
    const int control_height = 20;

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 draw_pos{window_ctx.position.x + cursor_pos.x + 14, window_ctx.position.y + cursor_pos.y};

    const bool inlined = id_split[0].empty();

    if (!inlined) {
        int text_width, text_height;
        functions.get_text_size(font, id_split[0].c_str(), text_width, text_height);

        window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y - 4}, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0, 0}, font});

        draw_pos.y += text_height;
    }

    std::string value_str;
    int text_width, text_height;

    for (auto &item_t : items) {
        if (*item_t.value) {
            if (value_str.length() > 0)
                value_str += ", ";

            value_str += item_t.name;
        }
    }

    functions.get_text_size(font, value_str.c_str(), text_width, text_height);
    if (text_width > control_width - 18) {
        value_str.resize(control_width / 10);
        value_str += " ...";
    }
    if (!value_str.length())
        value_str += "None";

    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + control_width - 10, draw_pos.y + 4}, zgui_render_type::zgui_text, global_colors.color_text, "+", vec2{0, 0}, font});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 4, draw_pos.y + 4}, zgui_render_type::zgui_text, global_colors.color_text, value_str, vec2{0, 0}, font});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, global_colors.control_idle, "", {control_width - 2, control_height - 2}});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {control_width, control_height}});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + control_width + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12)});


    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && utils::input::key_pressed(VK_LBUTTON) && window_ctx.blocking == 0) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (window_ctx.blocking == utils::hash::hash(id)) {
        for (int i = 1; i <= items.size(); i++) {
            bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y + (control_height - 1) * i, control_width, control_height);
            const bool outofbounds = utils::input::mouse_in_region(draw_pos.x, draw_pos.y + (control_height - 1), control_width, control_height * i);
            if (hovered && utils::input::key_pressed(VK_LBUTTON)) {
                window_ctx.blocking = utils::hash::hash(id);
                *items[i - 1].value = !*items[i - 1].value;
            }
            if (!outofbounds && utils::input::key_pressed(VK_LBUTTON)) {
                window_ctx.blocking = 0;
            }
            bool selected = *items[i - 1].value >= 1;
            window_ctx.render.emplace_back(zgui_control_render_t{ {draw_pos.x + 4, draw_pos.y + (control_height - 1) * i + 4}, zgui_render_type::zgui_text, selected ? color{0,150,255,255} : global_colors.color_text, items[i - 1].name.data(), vec2{0, 0}, font });
            window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 1, draw_pos.y + (19 * i) + 1}, zgui_render_type::zgui_filled_rect, hovered ? global_colors.color_combo_bg : global_colors.control_idle, "", {control_width - 2, control_height - 2}});
            window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y + 19 * i}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {control_width, control_height}});
        }
    }

    utils::misc::push_font(font);
}
// ========================================================================


// ========================================================================
void zgui::begin_groupbox(std::string_view title, const vec2 size, const int flags) {
    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    const vec2 draw_pos{window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y};

    int text_width, text_height;

    if (!title.empty())
        functions.get_text_size(font, title.data(), text_width, text_height);

    const int header_height = title.empty() ? 16 : text_height + 3;

    if (flags & zgui_groupbox_flags_legacy_design) {
        functions.draw_rect(draw_pos.x - 1, draw_pos.y - 1, size.x + 2, size.y + 2, global_colors.control_outline);
        functions.draw_filled_rect(draw_pos.x, draw_pos.y, size.x, size.y, global_colors.color_groupbox_bg);

        if (!title.empty()) {
            functions.draw_text(draw_pos.x + 4, draw_pos.y - 8, global_colors.color_text, font, false, title.data());
        }
    }
    else {
        functions.draw_rect(draw_pos.x - 1, draw_pos.y - 1, size.x + 2, size.y + 2, global_colors.control_outline);
        functions.draw_filled_rect(draw_pos.x, draw_pos.y, size.x, size.y, global_colors.color_groupbox_bg);

        functions.draw_filled_rect(draw_pos.x, draw_pos.y, size.x, header_height, global_colors.color_groupbox_header);
        functions.draw_line(draw_pos.x, draw_pos.y + header_height, draw_pos.x + size.x, draw_pos.y + header_height, global_colors.control_outline);

        if (!title.empty()) {
            functions.draw_text(draw_pos.x + size.x / 2 - text_width / 2, draw_pos.y + 2, global_colors.color_text, font, false, title.data());
        }
    }

    window_ctx.next_cursor_pos = vec2{cursor_pos.x, cursor_pos.y + size.y + 10};

    utils::misc::push_cursor_pos(vec2{ cursor_pos.x + 8, cursor_pos.y + ((flags & zgui_groupbox_flags_legacy_design) ? 0 : header_height) + 8 });

    utils::misc::push_font(font);
}

// ========================================================================
void zgui::end_groupbox() {
    utils::misc::push_cursor_pos(window_ctx.next_cursor_pos);
    window_ctx.next_cursor_pos = {};
}
// ========================================================================


// ========================================================================
void zgui::key_bind(const char *id, int &value) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const int control_width = 80;
    const int control_height = 20;

    value = std::clamp(value, 0, 255);

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 draw_pos{window_ctx.position.x + cursor_pos.x + 14, window_ctx.position.y + cursor_pos.y};

    const bool inlined = id_split[0].empty();

    if (!inlined) {
        int text_width, text_height;
        functions.get_text_size(font, id_split[0].c_str(), text_width, text_height);

        window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y - 4}, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0, 0}, font});

        draw_pos.y += text_height;
    }

    const bool active = window_ctx.blocking == utils::hash::hash(id);

    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && utils::input::key_pressed(VK_LBUTTON) && window_ctx.blocking == 0) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (active) {
        for (int i = 0; i < 256; i++) {
            if (utils::input::key_pressed(i)) {
                if (keys_list[i] != "Error")
                    value = i;

                if (utils::input::key_pressed(VK_ESCAPE))
                    value = 0;

                window_ctx.blocking = 0;
            }
        }
    }

    std::string key_data;
    if (keys_list[value] == "Error")
        key_data = "No key";
    else
        key_data = keys_list[value].data();
        
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 4, draw_pos.y + 4}, zgui_render_type::zgui_text, global_colors.color_text, active ? "Press any key" : key_data, vec2{0, 0}, font});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, active ? global_colors.control_active_or_clicked : global_colors.control_idle, "", {control_width, control_height}});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {control_width + 2, control_height + 2}});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + control_width + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12)});

    utils::misc::push_font(font);
}
// ========================================================================



// ========================================================================
void zgui::listbox(const char *id, std::vector<multi_select_item> items) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const int control_width = 100;
    const int control_height = 20;

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 draw_pos{window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y};

    const bool inlined = id_split[0].empty();

    if (!inlined) {
        int text_width, text_height;
        functions.get_text_size(font, id_split[0].c_str(), text_width, text_height);

        window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y - 4}, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0, 0}, font});

        draw_pos.y += text_height;
    }

    for (int i = 1; i <= items.size(); i++) {
        const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y + (control_height - 1) * (i - 1), control_width, control_height);

        if (hovered && utils::input::key_pressed(VK_LBUTTON)) {
            window_ctx.blocking = 0;
            *items[i - 1].value = !*items[i - 1].value;
        }
        window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 4, draw_pos.y + (control_height - 1) * (i - 1) + 4}, zgui_render_type::zgui_text, *items[i - 1].value || hovered ? global_colors.control_active_or_clicked : global_colors.color_text, items[i - 1].name.data(), vec2{0, 0}, font});
    }


    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, global_colors.control_idle, "", {control_width - 2, static_cast<float>(control_height * items.size() - 2)}});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {control_width, static_cast<float>(control_height * items.size())}});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + control_width + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12) + control_height * (items.size() - 1)});

    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && utils::input::key_pressed(VK_LBUTTON) && window_ctx.blocking == 0) {
        window_ctx.blocking = utils::hash::hash(id);
    }

    utils::misc::push_font(font);
}
// ========================================================================

// ========================================================================
void zgui::slider_int(const char *id, const int min, const int max, int &value) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const unsigned long font = utils::misc::pop_font();

    const int control_width = 120;
    const int control_height = 10;

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 draw_pos{window_ctx.position.x + cursor_pos.x + 14, window_ctx.position.y + cursor_pos.y};

    const bool inlined = id_split[0].empty();

    if (!inlined) {
        int text_width, text_height;
        functions.get_text_size(font, id_split[0].c_str(), text_width, text_height);

        window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y - 4}, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0, 0}, font});

        draw_pos.y += text_height;
    }

    if (window_ctx.blocking == 0 && utils::input::mouse_in_region(draw_pos.x - (control_height - 2), draw_pos.y, 8, 10) && utils::input::key_pressed(VK_LBUTTON))
        value = std::clamp(value - 1, min, max);
    else if (window_ctx.blocking == 0 && utils::input::mouse_in_region(draw_pos.x + control_width, draw_pos.y, 8, 10) && utils::input::key_pressed(VK_LBUTTON))
        value = std::clamp(value + 1, min, max);

    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && utils::input::key_pressed(VK_LBUTTON) && window_ctx.blocking == 0) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (utils::input::key_down(VK_LBUTTON) && window_ctx.blocking == utils::hash::hash(id)) {
        float value_unmapped = std::clamp(mouse_pos.x - draw_pos.x, 0.0f, static_cast<float>(control_width));
        int value_mapped = static_cast<int>(value_unmapped / control_width * (max - min) + min);

        value = value_mapped;
    }
    else if (!utils::input::key_down(VK_LBUTTON) && window_ctx.blocking == utils::hash::hash(id)) {
        window_ctx.blocking = 0;
    }

    const int dynamic_width = (static_cast<float>(value) - min) / (max - min) * control_width - 2;

    int text_width, text_height;
    std::string value_str = std::to_string(value);
    functions.get_text_size(font, value_str.c_str(), text_width, text_height);

    int text_x = dynamic_width - text_width;

    if (text_x < 0)
        text_x = 0;

    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x - (control_height - 2), draw_pos.y - 2}, zgui_render_type::zgui_text, global_colors.color_text_dimmer, "-", vec2{0, 0}, font});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + (control_width + 4), draw_pos.y - 2}, zgui_render_type::zgui_text, global_colors.color_text_dimmer, "+", vec2{0, 0}, font});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + text_x, draw_pos.y}, zgui_render_type::zgui_text, global_colors.color_text, value_str, vec2{0, 0}, font});

    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, global_colors.color_slider, "", {static_cast<float>(dynamic_width), control_height - 2}});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, global_colors.control_idle, "", {control_width - 2, control_height - 2}});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {control_width, control_height}});


    utils::misc::push_cursor_pos(vec2{cursor_pos.x + control_width + 14 + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12)});

    utils::misc::push_font(font);
}

// ========================================================================
void zgui::slider_float(const char *id, const float min, const float max, float &value) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const int control_width = 120;
    const int control_height = 10;

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 draw_pos{window_ctx.position.x + cursor_pos.x + 14, window_ctx.position.y + cursor_pos.y};

    const bool inlined = id_split[0].empty();

    if (!inlined) {
        int text_width, text_height;
        functions.get_text_size(font, id_split[0].c_str(), text_width, text_height);

        window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y - 4}, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0, 0}, font});

        draw_pos.y += text_height;
    }

    if (window_ctx.blocking == 0 && utils::input::mouse_in_region(draw_pos.x - (control_height - 2), draw_pos.y, 8, 10) && utils::input::key_pressed(VK_LBUTTON))
        value = std::clamp(value - 1, min, max);
    else if (window_ctx.blocking == 0 && utils::input::mouse_in_region(draw_pos.x + control_width, draw_pos.y, 8, 10) && utils::input::key_pressed(VK_LBUTTON))
        value = std::clamp(value + 1, min, max);

    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && utils::input::key_pressed(VK_LBUTTON) && window_ctx.blocking == 0) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (utils::input::key_down(VK_LBUTTON) && window_ctx.blocking == utils::hash::hash(id)) {
        float value_unmapped = std::clamp(mouse_pos.x - draw_pos.x, 0.0f, static_cast<float>(control_width));
        float value_mapped = static_cast<float>((value_unmapped / static_cast<float>(control_width)) * (max - min) + min);

        value = value_mapped;
    }
    else if (!utils::input::key_down(VK_LBUTTON) && window_ctx.blocking == utils::hash::hash(id)) {
        window_ctx.blocking = 0;
    }

    const float dynamic_width = (static_cast<float>(value) - min) / (max - min) * control_width - 2;

    int text_width, text_height;
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << value;
    std::string value_str = ss.str();
    functions.get_text_size(font, value_str.c_str(), text_width, text_height);

    int text_x = dynamic_width - text_width;

    if (text_x < 0)
        text_x = 0;

    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x - (control_height - 2), draw_pos.y - 2}, zgui_render_type::zgui_text, global_colors.color_text_dimmer, "-", vec2{0, 0}, font});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + (control_width + 4), draw_pos.y - 2}, zgui_render_type::zgui_text, global_colors.color_text_dimmer, "+", vec2{0, 0}, font});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + text_x, draw_pos.y}, zgui_render_type::zgui_text, global_colors.color_text, value_str, vec2{0, 0}, font});

    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, global_colors.color_slider, "", {dynamic_width, control_height - 2}});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, global_colors.control_idle, "", {control_width - 2, control_height - 2}});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {control_width, control_height}});


    utils::misc::push_cursor_pos(vec2{cursor_pos.x + control_width + 14 + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12)});

    utils::misc::push_font(font);
}
// ========================================================================

// ========================================================================
bool zgui::clickable_text(const char *id) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    const vec2 draw_pos{window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y};

    int text_width, text_height;
    functions.get_text_size(font, id_split[0].c_str(), text_width, text_height);

    const bool active = window_ctx.blocking == utils::hash::hash(id);
    const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, text_width, text_height);

    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y}, zgui_render_type::zgui_text, (hovered || window_ctx.blocking == utils::hash::hash(id)) ? color{0, 150, 255, 255} : global_colors.color_text, id_split[0], vec2{0, 0}, font});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + text_width + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + text_height / 2 + global_config.item_spacing});

    bool result = false;

    if (!active && hovered && utils::input::key_pressed(VK_LBUTTON)) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (active && !utils::input::key_down(VK_LBUTTON)) {
        window_ctx.blocking = 0;
        result = hovered;
    }

    utils::misc::push_font(font);

    return result;
}

// ========================================================================
void zgui::text(const char *text) {
    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    const vec2 draw_pos{window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y};

    int text_width, text_height;
    functions.get_text_size(font, text, text_width, text_height);

    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y}, zgui_render_type::zgui_text, global_colors.color_text, text, vec2{0, 0}, font});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + text_width + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + text_height / 2 + global_config.item_spacing});

    utils::misc::push_font(font);
}
// ========================================================================

// ========================================================================
void zgui::text_input(const char *id, std::string &value, const int max_length, const int flags) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const int control_width = 80;
    const int control_height = 20;

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 draw_pos{window_ctx.position.x + cursor_pos.x + 14, window_ctx.position.y + cursor_pos.y};

    const bool inlined = id_split[0].empty();

    if (!inlined) {
        int text_width, text_height;
        functions.get_text_size(font, id_split[0].c_str(), text_width, text_height);

        window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y - 4}, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0, 0}, font});

        draw_pos.y += text_height;
    }

    const bool active = window_ctx.blocking == utils::hash::hash(id);
    const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height);

    if (hovered && utils::input::key_pressed(VK_LBUTTON) && !active) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (active) {
        if (utils::input::key_pressed(VK_ESCAPE) || utils::input::key_pressed(VK_RETURN) || (!hovered && utils::input::key_pressed(VK_LBUTTON))) {
            window_ctx.blocking = 0;
        }
        else if (utils::input::key_pressed(VK_BACK) && !value.empty()) {
            value.pop_back();
        }
        else if (value.length() < max_length) {
            for (int i = 32; i <= 222; i++) {
                if ((i > 32 && i < 48) || (i > 57 && i < 65) || (i > 90 && i < 186))
                    continue;

                if (i > 57 && i <= 90) {
                    if (utils::input::key_pressed(i))
                        value += utils::input::key_down(VK_SHIFT) ? static_cast<char>(i) : static_cast<char>(i + 32);
                }
                else {
                    if (utils::input::key_pressed(i)) {
                        for (int j = 0; j < sizeof(special_characters); j++) {
                            if (special_characters[j].vk == i)
                                value += utils::input::key_down(VK_SHIFT) ? special_characters[j].shift : special_characters[j].regular;
                        }
                    }
                }
            }
        }
    }

    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 4, draw_pos.y + 4}, zgui_render_type::zgui_text, global_colors.color_text, flags & zgui_text_input_flags_password ? std::string(value.length(), '*').c_str() : value.c_str(), vec2{0, 0}, font});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, active ? global_colors.control_active_or_clicked : global_colors.control_idle, "", {control_width - 2, control_height - 2}});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {control_width, control_height}});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + control_width + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12)});

    utils::misc::push_font(font);
}
// ========================================================================

// ========================================================================
void zgui::toggle_button(const char *id, const vec2 size, bool &value) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    const vec2 draw_pos{window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y};

    const bool active = window_ctx.blocking == utils::hash::hash(id);

    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, size.x, size.y); !active && hovered && utils::input::key_pressed(VK_LBUTTON)) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (active && !utils::input::key_down(VK_LBUTTON)) {
        window_ctx.blocking = 0;
        value = !value;
    }

    int text_width, text_height;
    functions.get_text_size(font, id_split[0].c_str(), text_width, text_height);

    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + size.x / 2 - text_width / 2, draw_pos.y + size.y / 2 - text_height / 2}, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0, 0}, font});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, value ? global_colors.control_active_or_clicked : global_colors.control_idle, "", size});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {size.x + 2, size.y + 2}});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + size.x + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + size.y / 2 + global_config.item_spacing});

    utils::misc::push_font(font);
}
// ========================================================================

// ========================================================================
bool zgui::begin_window(std::string_view title, const vec2 default_size, const unsigned long font, const int flags) {
    if (!input_loop_started)
        throw std::exception("Input loop didnt start or didnt start properly.");;

    if (!(flags & zgui_window_flags_always_open)) {
        if (utils::input::key_pressed(global_config.menu_toggle_key))
            window_ctx.opened = !window_ctx.opened;
    }
    else
        window_ctx.opened = true;

    if (const int prev_alpha = window_ctx.alpha; !(flags & zgui_window_flags_no_ontoggle_animation)) {
        const int fade_factor = static_cast<int>(1.0f / 0.15f * functions.get_frametime() * 255);
        window_ctx.alpha = std::clamp(window_ctx.alpha + (window_ctx.opened ? fade_factor : -fade_factor), 0, 255);

        if (window_ctx.alpha != prev_alpha) {
            global_colors.window_border_inner_fill.a = window_ctx.alpha;
            global_colors.window_border_fill.a = window_ctx.alpha;
            global_colors.window_border_color.a = window_ctx.alpha;
            global_colors.window_background.a = window_ctx.alpha;

            global_colors.control_outline.a = window_ctx.alpha;
            global_colors.control_active_or_clicked.a = window_ctx.alpha;
            global_colors.control_idle.a = window_ctx.alpha;

            global_colors.color_groupbox_bg.a = window_ctx.alpha;
            global_colors.color_text.a = window_ctx.alpha;
            global_colors.color_text_dimmer.a = window_ctx.alpha;
            global_colors.color_slider.a = window_ctx.alpha;
        }
    }

    if (window_ctx.opened || window_ctx.alpha > 0) {
        if (!(flags & zgui_window_flags_no_move)) {
            if ((flags & zgui_window_flags_no_border ? utils::input::mouse_in_region(window_ctx.position.x + 9, window_ctx.position.y + 14, window_ctx.size.x - 18, 14)
                                                     : utils::input::mouse_in_region(window_ctx.position.x - 6, window_ctx.position.y - 10, window_ctx.size.x + 12, 16))
                && utils::input::key_pressed(VK_LBUTTON) && !window_ctx.dragging) {
                window_ctx.dragging = true;
            }
            else if (utils::input::key_down(VK_LBUTTON) && window_ctx.dragging) {
                const vec2 mouse_delta{mouse_pos.x - previous_mouse_pos.x, mouse_pos.y - previous_mouse_pos.y};
                const vec2 new_position{window_ctx.position.x + mouse_delta.x, window_ctx.position.y + mouse_delta.y};

                window_ctx.position = new_position;
            }
            else if (!utils::input::key_down(VK_LBUTTON) && window_ctx.dragging) {
                window_ctx.dragging = false;
            }
        }

        if (window_ctx.size.x < 1 && window_ctx.size.y < 1)
            window_ctx.size = default_size;

        if (!(flags & zgui_window_flags_no_border)) {
            functions.draw_filled_rect(window_ctx.position.x - 6, window_ctx.position.y - 10, window_ctx.size.x + 12, window_ctx.size.y + 16, global_colors.window_border_inner_fill);
            functions.draw_filled_rect(window_ctx.position.x - 5, window_ctx.position.y - 9, window_ctx.size.x + 10, window_ctx.size.y + 14, global_colors.window_border_color);
            functions.draw_filled_rect(window_ctx.position.x - 4, window_ctx.position.y - 8, window_ctx.size.x + 8, window_ctx.size.y + 12, global_colors.window_border_fill);
            functions.draw_filled_rect(window_ctx.position.x, window_ctx.position.y + 7, window_ctx.size.x, window_ctx.size.y - 7, global_colors.window_border_color);
            functions.draw_filled_rect(window_ctx.position.x + 1, window_ctx.position.y + 8, window_ctx.size.x - 2, window_ctx.size.y - 9, global_colors.window_border_inner_fill);
            functions.draw_filled_rect(window_ctx.position.x + 8, window_ctx.position.y + 15, window_ctx.size.x - 16, window_ctx.size.y - 23, global_colors.window_border_color);
        }

        if (!(flags & zgui_window_flags_no_titlebar))
            functions.draw_text(window_ctx.position.x + window_ctx.size.x * 0.5, window_ctx.position.y + (window_ctx.size.y * 0.010) - 10, global_colors.color_text, font, true, title.data());

        functions.draw_filled_rect(window_ctx.position.x + 9, window_ctx.position.y + 16, window_ctx.size.x - 18, window_ctx.size.y - 25, global_colors.window_background);


        utils::misc::push_font(font);
        utils::misc::push_cursor_pos(global_config.base_pos);
    }

    return window_ctx.opened || window_ctx.alpha > 0;
}

// ========================================================================
void zgui::end_window() {
    for (int i = window_ctx.render.size() - 1; i >= 0; i--) {
        switch (window_ctx.render[i].render_type) {
            case zgui_render_type::zgui_line:
                functions.draw_line(window_ctx.render[i].draw_position.x, window_ctx.render[i].draw_position.y, window_ctx.render[i].size.x, window_ctx.render[i].size.y, window_ctx.render[i].color);
                break;
            case zgui_render_type::zgui_rect:
                functions.draw_rect(window_ctx.render[i].draw_position.x, window_ctx.render[i].draw_position.y, window_ctx.render[i].size.x, window_ctx.render[i].size.y, window_ctx.render[i].color);
                break;
            case zgui_render_type::zgui_filled_rect:
                functions.draw_filled_rect(window_ctx.render[i].draw_position.x, window_ctx.render[i].draw_position.y, window_ctx.render[i].size.x, window_ctx.render[i].size.y, window_ctx.render[i].color);
                break;
            case zgui_render_type::zgui_text:
                functions.draw_text(window_ctx.render[i].draw_position.x, window_ctx.render[i].draw_position.y, window_ctx.render[i].color, window_ctx.render[i].font, false, window_ctx.render[i].text.c_str());
                break;
        }
    }

    window_ctx.render.clear();

    while (!window_ctx.cursor_pos.empty())
        window_ctx.cursor_pos.pop();
}
// ========================================================================

// ========================================================================
void zgui::dummy() {
    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + global_config.item_spacing});
}

// ========================================================================
void zgui::next_column(const int pusher_x, const int pusher_y) {
    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 new_cursor_pos{cursor_pos.x + pusher_x, global_config.base_pos.y + pusher_y};

    if (window_ctx.next_cursor_pos.y != 0)
        new_cursor_pos.y += 14;

    utils::misc::push_cursor_pos(new_cursor_pos);
}

// ========================================================================
void zgui::same_line(const float x_axis) {
    const vec2 cursor_pos = utils::misc::pop_cursor_pos();

    if (x_axis != -1)
        utils::misc::push_cursor_pos(vec2{global_config.base_pos.x + x_axis, cursor_pos.x});
}

// ========================================================================
void zgui::backup_line() {
    const vec2 cursor_pos = utils::misc::pop_cursor_pos();

    utils::misc::push_cursor_pos(vec2{window_ctx.next_cursor_pos.x + 20, cursor_pos.y});
}
// ========================================================================
void zgui::push_line() {
    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    utils::misc::push_cursor_pos(vec2{ window_ctx.next_cursor_pos.x + 155, cursor_pos.y });
}
// ========================================================================
std::vector<std::string> zgui::utils::hash::split_str(const char *str, const char separator) {
    std::vector<std::string> output;
    std::string substring;
    std::istringstream stream{str};

    while (std::getline(stream, substring, separator))
        output.push_back(substring);

    return output;
}

// ========================================================================
uint32_t zgui::utils::hash::hash(const char *str, const uint32_t value) {
    return *str ? hash(str + 1, (value ^ *str) * 0x1000193ull) : value;
}
// ========================================================================

// ========================================================================
// Function for starting our input loop.
void zgui::poll_input(std::string_view window_name) {
    if (window_name.empty())
        throw std::exception("No window from where input should be read from specified in function parameter.");

    for (int i = 0; i < 256; i++) {
        prev_key_state[i] = key_state[i];
        key_state[i] = GetAsyncKeyState(i);
    }

    POINT p_mouse_pos;
    GetCursorPos(&p_mouse_pos);
    ScreenToClient(FindWindow(nullptr, window_name.data()), &p_mouse_pos);
    previous_mouse_pos = mouse_pos;
    mouse_pos = vec2{static_cast<float>(p_mouse_pos.x), static_cast<float>(p_mouse_pos.y)};

    if (!input_loop_started)
        input_loop_started = true;
}

// ========================================================================
// Function for starting our input loop.
void zgui::poll_input(HWND hwnd) {
    if (!hwnd)
        throw std::exception("No window from where input should be read from specified in function parameter.");

    for (int i = 0; i < 256; i++) {
        prev_key_state[i] = key_state[i];
        key_state[i] = GetAsyncKeyState(i);
    }

    POINT p_mouse_pos;
    GetCursorPos(&p_mouse_pos);
    ScreenToClient(hwnd, &p_mouse_pos);
    previous_mouse_pos = mouse_pos;

    mouse_pos = vec2{static_cast<float>(p_mouse_pos.x), static_cast<float>(p_mouse_pos.y)};

    if (!input_loop_started)
        input_loop_started = true;
}

// ========================================================================
bool zgui::utils::input::key_pressed(const int key) {
    return key_state[key] && !prev_key_state[key];
}

// ========================================================================
bool zgui::utils::input::key_down(const int key) {
    return key_state[key];
}

// ========================================================================
bool zgui::utils::input::key_released(const int key) {
    return !key_state[key] && prev_key_state[key];
}

// ========================================================================
// Check if mouse is hovered over specified region.
bool zgui::utils::input::mouse_in_region(const int x, const int y, const int w, const int h) {
    return mouse_pos.x > x && mouse_pos.y > y && mouse_pos.x < w + x && mouse_pos.y < h + y;
}
// ========================================================================

// ========================================================================
// Push cursor position to the stack defined in window context.
void zgui::utils::misc::push_cursor_pos(const vec2 pos) {
    window_ctx.cursor_pos.push(pos);
}

// ========================================================================
// Pop cursor position from the stack defined in window context.
zgui::vec2 zgui::utils::misc::pop_cursor_pos() {
    const vec2 pos = window_ctx.cursor_pos.top();
    window_ctx.cursor_pos.pop();
    return pos;
}

// ========================================================================
// Pop font from the stack defined in window context.
void zgui::utils::misc::push_font(const unsigned long font) {
    window_ctx.fonts.push(font);
}

// ========================================================================
// Push font to the stack defined in window context.
unsigned long zgui::utils::misc::pop_font() {
    const unsigned long font = window_ctx.fonts.top();
    window_ctx.fonts.pop();
    return font;
}

int getHue(int red, int green, int blue) {

    float min = min(min(red, green), blue);
    float max = max(max(red, green), blue);

    if (min == max) {
        return 0;
    }

    float hue = 0.f;
    if (max == red) {
        hue = (green - blue) / (max - min);

    }
    else if (max == green) {
        hue = 2.f + (blue - red) / (max - min);

    }
    else {
        hue = 4.f + (red - green) / (max - min);
    }

    hue = hue * 60;
    if (hue < 0) hue = hue + 360;

    return round(hue);
}
static bool rightclicked;

HWND hwndclip = GetDesktopWindow();
void sendToClipboard(HWND hwnd, const std::string& s) {
    OpenClipboard(hwnd);
    EmptyClipboard();
    HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
    if (!hg) {
        CloseClipboard();
        return;
    }
    memcpy(GlobalLock(hg), s.c_str(), s.size() + 1);
    GlobalUnlock(hg);
    SetClipboardData(CF_TEXT, hg);
    CloseClipboard();
    GlobalFree(hg);
}

void zgui::colorpicker(const char* id, color& item) noexcept
{
    std::vector<std::string> id_split = zgui::utils::hash::split_str(id, '#');

    const int control_height = 8;
    const int control_width = 8;

    const int slider_height = 10;
    const int slider_width = 152.5f;

    const unsigned long font = utils::misc::pop_font();
    const vec2 cursor_pos = zgui::utils::misc::pop_cursor_pos();
    const vec2 draw_pos{ zgui::globals::window_ctx.position.x + cursor_pos.x + 175, zgui::globals::window_ctx.position.y + cursor_pos.y - 16 };

    color rainbow;
    color grey;

    int text_wide, text_tall;
    functions.get_text_size(font, id_split[0].c_str(), text_wide, text_tall);

    int CtrXoffset = 0;
    /*float yoffset = draw_pos.y + 10;
    float slideryoffset = draw_pos.y;
    float xoffset = draw_pos.x + 300;
    float sliderxoffset = draw_pos.x + 400; */
    const float x_offset = 75;
    const float y_offset = 0;
    float yoffset = draw_pos.y + y_offset;
    float slideryoffset = draw_pos.y + 173;
    float xoffset = draw_pos.x + x_offset;
    float sliderxoffset = draw_pos.x + (x_offset / 2) - 3.5f;

    const int min = 0;
    const int max = 255;

    const bool active = zgui::globals::window_ctx.blocking == zgui::utils::hash::hash(id);

    std::string itemstring = std::to_string(item.r);
    itemstring += std::to_string(item.g);
    itemstring += std::to_string(item.b);
    itemstring += std::to_string(item.a);

    if (const bool hovered = zgui::utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width + 6 + text_wide, control_height); !active && hovered && zgui::utils::input::key_pressed(VK_RBUTTON)) {
        rightclicked = true;
    }
    else {
        rightclicked = false;
    }

    if (rightclicked)
    {
        sendToClipboard(hwndclip, itemstring);
    }

    if (const bool hovered = zgui::utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width * 2, control_height); !active && hovered && zgui::utils::input::key_pressed(VK_LBUTTON))
    {
        zgui::globals::window_ctx.blocking = zgui::utils::hash::hash(id);
    }
    else if (active)
    {
        if (zgui::utils::input::mouse_in_region(draw_pos.x, draw_pos.y, 2 * control_width, control_height) && zgui::utils::input::key_pressed(VK_LBUTTON))
            zgui::globals::window_ctx.blocking = 0;

        if (zgui::utils::input::mouse_in_region(draw_pos.x + 20, draw_pos.y + 5, 200, 180) && zgui::utils::input::key_pressed(VK_LBUTTON))
            zgui::globals::window_ctx.blocking = zgui::utils::hash::hash(id);

        if (!zgui::utils::input::mouse_in_region(draw_pos.x + 15, draw_pos.y + 5, 200, 190) && zgui::utils::input::key_pressed(VK_LBUTTON))
            zgui::globals::window_ctx.blocking = 0;

        for (int i = 0; i < 990; i++)
        {
            if (zgui::utils::input::mouse_in_region(xoffset + 100, yoffset, 5, 5))
            {
                if (zgui::utils::input::key_down(VK_LBUTTON)) {
                    item.r = rainbow.r;
                    item.g = rainbow.g;
                    item.b = rainbow.b;
                }
                //if (key_released(VK_LBUTTON))
                //	context.window.blocking = 0;
            }

            if (xoffset >= draw_pos.x + x_offset)
            {
                xoffset -= 150;
                yoffset += 5;
            }

            float hue = (i * .001f);

            rainbow.FromHSV(hue, 1.f, 1.f);

            zgui::globals::window_ctx.render.emplace_back(zgui_control_render_t{ { xoffset + 110, yoffset, }, zgui_render_type::zgui_filled_rect,color { rainbow.r, rainbow.g, rainbow.b, global_colors.control_outline.a },"", { 5,5 } });

            if (GetAsyncKeyState(VK_LBUTTON) && zgui::utils::input::mouse_in_region(xoffset + 125, yoffset, 5, 5))
            {
                item.r = rainbow.r;
                item.g = rainbow.g;
                item.b = rainbow.b;
            }

            xoffset += 5;

        }

        for (int i = 0; i < 40; i++)
        {
            if (zgui::utils::input::mouse_in_region(draw_pos.x + 20, (yoffset + 5) - 170, 10, 4))
            {
                if (zgui::utils::input::key_down(VK_LBUTTON)) {
                    item.r = grey.r;
                    item.g = grey.g;
                    item.b = grey.b;
                }
                //if (key_released(VK_LBUTTON))
                //	context.window.blocking = 0;
            }

            if (xoffset >= draw_pos.x + x_offset)
            {
                xoffset -= 4;
                yoffset += 4;
            }

            float hue = (i * .025f);

            grey.FromHSV(0.f, 0.f, hue);

            zgui::globals::window_ctx.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 20, (yoffset + 5) - 170, }, zgui_render_type::zgui_filled_rect,color { grey.r, grey.g, grey.b, global_colors.control_outline.a },"", { 10,4 } });

            if (GetAsyncKeyState(VK_LBUTTON) && zgui::utils::input::mouse_in_region(draw_pos.x + 20, (yoffset + 5) - 170, 10, 4))
            {
                item.r = grey.r;
                item.g = grey.g;
                item.b = grey.b;
            }

            xoffset += 4;

        }

        if (zgui::globals::window_ctx.blocking == 0 && zgui::utils::input::mouse_in_region(draw_pos.x - (control_height - 2), draw_pos.y, 8, 10) && zgui::utils::input::key_pressed(VK_LBUTTON))
            item.a = std::clamp(item.a - 1, min, max);
        else if (zgui::globals::window_ctx.blocking == 0 && zgui::utils::input::mouse_in_region(draw_pos.x + control_width, draw_pos.y, 8, 10) && zgui::utils::input::key_pressed(VK_LBUTTON))
            item.a = std::clamp(item.a + 1, min, max);

        if (zgui::utils::input::key_down(VK_LBUTTON) && zgui::utils::input::mouse_in_region(sliderxoffset, slideryoffset, slider_width + 5, slider_height) && zgui::globals::window_ctx.blocking == zgui::utils::hash::hash(id)) {
            zgui::globals::window_ctx.blocking = zgui::utils::hash::hash(id);
            float value_unmapped = std::clamp(mouse_pos.x - sliderxoffset, 0.0f, static_cast<float>(slider_width));
            int value_mapped = static_cast<int>(value_unmapped / slider_width * (max - min) + min);
            item.a = value_mapped;
        }

        const int dynamic_width = (static_cast<float>(item.a) - min) / (max - min) * slider_width - 2;

        int text_x = dynamic_width - text_wide;

        if (text_x < 0)
            text_x = 0;

        rainbow.r = item.r;
        rainbow.g = item.g;
        rainbow.b = item.b;
        rainbow.a = item.a;

        zgui::globals::window_ctx.render.emplace_back(zgui_control_render_t{ { sliderxoffset + 1 , slideryoffset + 1  }, zgui_render_type::zgui_filled_rect, color{item.r,item.g,item.b,item.a}, "", { static_cast<float>(dynamic_width), control_height - 2 } });
        zgui::globals::window_ctx.render.emplace_back(zgui_control_render_t{ { sliderxoffset + 1 , slideryoffset + 1 }, zgui_render_type::zgui_filled_rect, global_colors.control_idle,"", { slider_width - 2, control_height - 2 } });
        zgui::globals::window_ctx.render.emplace_back(zgui_control_render_t{ { sliderxoffset , slideryoffset  }, zgui_render_type::zgui_filled_rect, global_colors.control_outline,"", { slider_width, control_height } });

    }

    if (global_colors.control_outline.a >= 250)
        zgui::globals::window_ctx.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y }, zgui_render_type::zgui_filled_rect, color{item.r,item.g,item.b,item.a},"", { 2 * control_width, control_height } });
    else
        zgui::globals::window_ctx.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y }, zgui_render_type::zgui_filled_rect, color{item.r,item.g,item.b,global_colors.control_outline.a},"", { 2 * control_width, control_height } });

    zgui::globals::window_ctx.render.emplace_back(zgui_control_render_t{ { draw_pos.x - 1, draw_pos.y - 1 }, zgui_render_type::zgui_rect, color{0,0,0,global_colors.control_outline.a},"", { 2 * control_width + 2, control_height + 2 } });

    zgui::utils::misc::push_cursor_pos(vec2{ cursor_pos.x + 14 + global_config.item_spacing, cursor_pos.y });
    zgui::utils::misc::push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y });
}
// ========================================================================