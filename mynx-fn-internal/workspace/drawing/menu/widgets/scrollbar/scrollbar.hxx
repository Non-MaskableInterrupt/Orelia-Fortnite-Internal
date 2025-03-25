namespace drawing {
	namespace widgets {
		class c_scrollbar : public c_radar {
		public:
			bool is_position_visible( bool is_left_child, const engine::fvector2d& pos ) {
				const scroll_info& child = is_left_child ? m_left_child :
					( pos.m_y > m_menu_position.m_y + m_menu_size.m_y / 2 ) ? m_bottom_child : m_right_child;
				if ( !child.initialized ) {
					return true;
				}

				const float padding = -10.0f;
				float relative_y = pos.m_y - child.window_pos.m_y;

				return relative_y >= -padding &&
					relative_y <= child.window_size.m_y + padding &&
					relative_y + child.scroll_value[ child.current_tab ] >= 0.0f &&
					relative_y + child.scroll_value[ child.current_tab ] <= child.current_content_height;
			}

			void handle_scrollbar_interaction( scroll_info& child, const engine::fvector2d& scroll_pos, float scroll_height, float max_scroll ) {
				const float padding = 10.0f;
				float adjusted_scroll_height = scroll_height;
				float adjusted_max_scroll = max_scroll;

				if ( is_hovered( scroll_pos, engine::fvector2d( m_scroll_bar_width, adjusted_scroll_height ) ) ) {
					if ( m_left_mouse_down ) {
						child.is_scrolling = true;
					}
				}

				if ( child.is_scrolling ) {
					if ( !m_left_mouse_down ) {
						child.is_scrolling = false;
					}
					else {
						float scroll_pos_ratio = ( m_mouse_position.m_y - child.window_pos.m_y ) / child.window_size.m_y;
						child.scroll_value[ child.current_tab ] = scroll_pos_ratio * adjusted_max_scroll;
						child.scroll_value[ child.current_tab ] = std::clamp( child.scroll_value[ child.current_tab ], 0.0f, adjusted_max_scroll );
					}
				}
			}

			void end_scroll_area( bool is_left_child ) {
				scroll_info& child = is_left_child ? m_left_child :
					( m_mouse_position.m_y > m_menu_position.m_y + m_menu_size.m_y / 2 ) ? m_bottom_child : m_right_child;

				if ( !child.initialized ) return;

				if ( child.current_content_height > child.window_size.m_y ) {
					const float padding = 10.0f;
					const float extra_scroll = 20.0f;

					float scroll_height = min(
						( ( child.window_size.m_y ) / ( child.current_content_height + padding + extra_scroll ) ) * child.window_size.m_y,
						child.window_size.m_y
					);

					float max_scroll = child.current_content_height - child.window_size.m_y + padding + extra_scroll;

					float scroll_ratio = std::clamp(
						child.scroll_value[ child.current_tab ] / max_scroll,
						0.0f,
						1.0f
					);

					float max_scroll_y = child.window_size.m_y - scroll_height;
					engine::fvector2d scroll_pos = {
						child.window_pos.m_x + child.window_size.m_x - m_scroll_bar_width,
						child.window_pos.m_y + ( scroll_ratio * max_scroll_y )
					};

					filled_rect(
						engine::fvector2d( scroll_pos.m_x, child.window_pos.m_y ),
						engine::fvector2d( m_scroll_bar_width, child.window_size.m_y ),
						engine::math::from_rgb( 45, 45, 45 )
					);

					filled_rect(
						scroll_pos,
						engine::fvector2d( m_scroll_bar_width, scroll_height ),
						child.is_scrolling ?
						engine::math::from_rgb( 170, 85, 235 ) :
						engine::math::from_rgb( 65, 65, 65 )
					);

					handle_scrollbar_interaction( child, scroll_pos, scroll_height, max_scroll );
				}

				child.initialized = false;
			}

			void track_content_height( bool is_left_child, float height ) {
				scroll_info& child = is_left_child ? m_left_child :
					( height > m_menu_position.m_y + m_menu_size.m_y / 2 ) ? m_bottom_child : m_right_child;

				if ( child.initialized ) {
					child.current_content_height = max( child.current_content_height, height - child.window_pos.m_y );
				}
			}

			engine::fvector2d get_scroll_adjusted_pos( bool is_left_child, const engine::fvector2d& pos ) {
				const scroll_info& child = is_left_child ? m_left_child :
					( pos.m_y > m_menu_position.m_y + m_menu_size.m_y / 2 ) ? m_bottom_child : m_right_child;

				if ( !child.initialized || child.measuring_phase )
					return pos;

				track_content_height( is_left_child, pos.m_y );

				return engine::fvector2d( pos.m_x, pos.m_y - child.scroll_value[ child.current_tab ] );
			}

			void begin_scroll_area( bool is_left_child, const engine::fvector2d& pos, const engine::fvector2d& size, int menu_tab, const wchar_t* header = nullptr ) {
				scroll_info& child = is_left_child ? m_left_child :
					( pos.m_y > m_menu_position.m_y + m_menu_size.m_y / 2 ) ? m_bottom_child : m_right_child;

				child.window_pos = pos;
				child.window_size = size;
				child.initialized = true;
				child.header = header;
				child.current_content_height = 0.0f;
				child.current_tab = menu_tab;

				filled_rect( pos, size, engine::math::from_rgb( 25, 25, 25 ), 1.0f );
				rect( pos, size, engine::math::from_rgb( 48, 48, 48 ), 1.0f );

				if ( header ) {
					engine::fvector2d text_pos = {
						pos.m_x + 10,
						pos.m_y
					};
					text( header, text_pos, engine::math::from_rgb( 140, 140, 140 ), 10.0, false, true, false );
				}
			}

			void handle_scroll_input( engine::a_actor* controller ) {
				float wheel_delta = controller->get_input_axis_key_value( engine::keys::g_mouse_wheel_axis ) * 25.0f;

				auto handle_child = [ & ]( scroll_info& child ) {
					if ( !child.initialized )
						return;

					if ( is_hovered( child.window_pos, child.window_size ) ) {
						float max_scroll = child.current_content_height - child.window_size.m_y + 50.0f;
						if ( max_scroll > 0.0f ) {
							child.scroll_value[ child.current_tab ] -= wheel_delta;
							child.scroll_value[ child.current_tab ] = std::clamp( child.scroll_value[ child.current_tab ], 0.0f, max_scroll );
						}
					}
					};

				handle_child( m_left_child );
				handle_child( m_right_child );
				handle_child( m_bottom_child );
			}
		};
	}
}