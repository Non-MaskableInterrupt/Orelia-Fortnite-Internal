namespace drawing {
	namespace widgets {
		class c_debug : public gui::c_gui {
		public:
			void debug_box( const wchar_t* title, const std::vector<std::pair<engine::fstring, int>>& items ) {
				const float box_width = 200.0f;
				const float title_height = 28.0f;
				const float item_height = 15.0f;
				const float padding = 8.0f;
				const float title_padding = 10.0f;

				const float total_height = title_height + ( items.size( ) * item_height ) + ( padding * 2 ) + 5.0;

				filled_rect(
					engine::fvector2d( m_debug_box_x, m_debug_box_y ),
					engine::fvector2d( box_width, total_height ),
					engine::math::from_rgb( 19, 19, 19 ),
					true
				);

				filled_rect(
					engine::fvector2d( m_debug_box_x, m_debug_box_y ),
					engine::fvector2d( box_width, title_height ),
					engine::math::from_rgb( 25, 25, 25 ),
					true
				);

				filled_rect(
					engine::fvector2d( m_debug_box_x, m_debug_box_y + title_height - 2.0f ),
					engine::fvector2d( box_width, 2.0f ),
					engine::math::from_rgb( 170, 85, 235 ),
					true
				);

				rect(
					engine::fvector2d( m_debug_box_x, m_debug_box_y ),
					engine::fvector2d( box_width, total_height ),
					engine::math::from_rgb( 48, 48, 48 ),
					1.0f,
					true
				);

				text(
					engine::fstring( title ),
					engine::fvector2d(
					m_debug_box_x + ( box_width / 2.0f ),
					m_debug_box_y + ( title_height / 2.0f )
				),
					engine::math::from_rgb( 255, 255, 255 ),
					12.0f,
					true,
					true,
					false,
					true
				);


				float y_offset = m_debug_box_y + title_height + padding;
				for ( const auto& item : items ) {
					auto built_string = engine::kismet::g_string_library->build_string_int(
						item.first,
						engine::fstring( L": " ),
						item.second,
						engine::fstring( )
					);

					text(
						engine::fstring( built_string ),
						engine::fvector2d( m_debug_box_x + padding, y_offset ),
						engine::math::from_rgb( 180, 180, 180 ),
						12.0f,
						false,
						false,
						false,
						true
					);

					y_offset += item_height + 2.0;
				}

				m_debug_box_y += total_height + m_debug_box_spacing;
			}

			void debug_box( const wchar_t* title, const std::vector<std::tuple<engine::fstring, int, engine::flinear_color>>& items ) {
				const float box_width = 200.0f;
				const float title_height = 28.0f;
				const float item_height = 15.0f;
				const float padding = 8.0f;
				const float title_padding = 10.0f;

				const float total_height = title_height + ( items.size( ) * item_height ) + ( padding * 2 ) + 5.0;

				filled_rect(
					engine::fvector2d( m_debug_box_x, m_debug_box_y ),
					engine::fvector2d( box_width, total_height ),
					engine::math::from_rgb( 19, 19, 19 ),
					true
				);

				filled_rect(
					engine::fvector2d( m_debug_box_x, m_debug_box_y ),
					engine::fvector2d( box_width, title_height ),
					engine::math::from_rgb( 25, 25, 25 ),
					true
				);

				filled_rect(
					engine::fvector2d( m_debug_box_x, m_debug_box_y + title_height - 2.0f ),
					engine::fvector2d( box_width, 2.0f ),
					engine::math::from_rgb( 170, 85, 235 ),
					true
				);

				rect(
					engine::fvector2d( m_debug_box_x, m_debug_box_y ),
					engine::fvector2d( box_width, total_height ),
					engine::math::from_rgb( 48, 48, 48 ),
					1.0f,
					true
				);

				text(
					engine::fstring( title ),
					engine::fvector2d(
					m_debug_box_x + ( box_width / 2.0f ),
					m_debug_box_y + ( title_height / 2.0f )
				),
					engine::math::from_rgb( 255, 255, 255 ),
					12.0f,
					true,
					true,
					false,
					true
				);


				float y_offset = m_debug_box_y + title_height + padding;
				for ( const auto& item : items ) {
					auto [name, value, color] = item;

					auto built_string = engine::kismet::g_string_library->build_string_int(
						name,
						engine::fstring( L": " ),
						value,
						engine::fstring( )
					);

					text(
						engine::fstring( built_string ),
						engine::fvector2d( m_debug_box_x + padding, y_offset ),
						color,
						12.0f,
						false,
						false,
						false,
						true
					);

					y_offset += item_height + 2.0;
				}

				m_debug_box_y += total_height + m_debug_box_spacing;
			}

			void debug_draw_list( ) {
				if ( !features::misc::render_count ) return;

				std::vector<std::pair<engine::fstring, int>> draw_lists = {
					{ L"background", m_last_background_index },
					{ L"foreground", m_last_foreground_index },
				};

				debug_box( L"render objects", draw_lists );
			}
		};
	}
}