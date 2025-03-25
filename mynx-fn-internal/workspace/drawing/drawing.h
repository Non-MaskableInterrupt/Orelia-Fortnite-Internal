#pragma once
namespace drawing {
	enum class e_draw_type : std::uint32_t {
		line,
		rectangle,
		circle,
		text,
		rect_filled,
		gradient_rect,
		diamond,
		rounded_rect
	};

	struct s_draw_command {
		e_draw_type draw_type{};
		engine::fvector2d position_a{};
		engine::fvector2d position_b{};
		engine::fvector2d position_c{};
		engine::flinear_color color{};
		engine::flinear_color secondary_color{};
		float thickness{};
		double radius{};
		double segments{};
		double size{};
		engine::fstring content{};
		bool filled{ false };
		bool outlined{ false };
		bool centered_x{ false };
		bool centered_y{ false };
		bool rounded{ false };
	};

	const std::uint32_t m_max_draw_members = 9999;
	s_draw_command m_foreground_draw_list[ m_max_draw_members ]{ };
	s_draw_command m_background_draw_list[ m_max_draw_members ]{ };

	struct rank_info {
		const wchar_t* texture_name;
		engine::flinear_color color;
		const wchar_t* display_name;
	};

	struct icon_config {
		engine::fvector2d size{ 50.0f, 50.0f };
		engine::fvector2d offset{ -20.0f, 0.0f };
		float opacity = 1.0f;
		bool show_text = true;
		float text_padding = 4.0f;
	};

	struct scroll_info {
		bool initialized = false;
		bool is_scrolling = false;
		bool measuring_phase = false;
		float scroll_value[ 6 ] = { 0 };
		float current_content_height = 0.0f;
		engine::fvector2d window_pos;
		engine::fvector2d window_size;
		const wchar_t* header = nullptr;
		int current_tab = 0;
	};

	class c_drawing {
	public:
		c_drawing( ) = default;

		engine::fvector2d get_screen_size( ) const {
			return m_screen_size;
		}

		engine::fvector2d get_screen_center( ) const {
			return m_screen_center;
		}

		bool in_screen(
			engine::fvector2d screen_position
		) const {
			if ( screen_position.m_x < 5.0 ||
				screen_position.m_x > m_screen_size.m_x - ( 5.0 * 2 ) &&
				screen_position.m_y < 5.0 ||
				screen_position.m_y > m_screen_size.m_y - ( 5.0 * 2 ) )
				return false;
			return true;
		}

	public:
		engine::u_font* m_font = nullptr;
		engine::u_canvas* m_canvas = nullptr;
		engine::fvector2d m_screen_size{ };
		engine::fvector2d m_screen_center{ };

		engine::fvector2d m_mouse_position;
		bool m_left_mouse_down = false;
		bool m_left_mouse_clicked = false;
		bool m_right_mouse_down = false;
		bool m_right_mouse_clicked = false;
		bool m_is_gui_active = true;
		bool m_is_aimbot_key_down = false;

		bool m_already_initialized;
		engine::fvector2d m_menu_position{ 200.0, 200.0 };
		engine::fvector2d m_menu_size;
		engine::fvector2d m_offset;

		bool m_window_hovered = false;
		bool m_window_grabbed = false;

		float m_debug_box_y = 5.0f;
		const float m_debug_box_spacing = 4.0f;
		float m_debug_box_x = 5.0f;
		bool m_radar_drawed = false;

		scroll_info m_left_child = {};
		scroll_info m_right_child = {};
		scroll_info m_bottom_child = {};
		const float m_scroll_bar_width = 8.0f;
		const float m_scroll_speed = 30.0f;

		int m_last_foreground_index = 0;
		int m_last_background_index = 0;
	};
}

#include <workspace/drawing/canvas/canvas.hxx>
#include <workspace/drawing/render/shapes/shapes.hxx>
#include <workspace/drawing/render/drawlist/drawlist.hxx>

#include <workspace/drawing/menu/gui/gui.hxx>
#include <workspace/drawing/menu/widgets/debug/debug.hxx>
#include <workspace/drawing/menu/widgets/radar/radar.hxx>
#include <workspace/drawing/menu/widgets/scrollbar/scrollbar.hxx>
#include <workspace/drawing/menu/widgets/widgets.hxx>
#include <workspace/drawing/menu/menu.hxx>

#include <workspace/drawing/visuals/visuals.hxx>

namespace drawing {
	class c_framework : public visuals::c_visuals {
	public:
		bool in_rect(
			double radius,
			engine::fvector2d screen_position
		) const {
			return m_screen_center.m_x >= ( m_screen_center.m_x - radius ) && m_screen_center.m_x <= ( m_screen_center.m_x + radius ) &&
				screen_position.m_y >= ( screen_position.m_y - radius ) && screen_position.m_y <= ( screen_position.m_y + radius );
		}

		bool in_circle(
			double radius,
			engine::fvector2d screen_position
		) const {
			if ( in_rect( radius, screen_position ) ) {
				auto dx = m_screen_center.m_x - screen_position.m_x; dx *= dx;
				auto dy = m_screen_center.m_y - screen_position.m_y; dy *= dy;
				return dx + dy <= radius * radius;
			} return false;
		}
	};
}