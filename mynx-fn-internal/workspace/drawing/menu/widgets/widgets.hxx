namespace drawing {
	namespace widgets {
		class c_widgets : public c_scrollbar {
		public:
			void color_picker(
				engine::fvector2d curr_position,
				engine::flinear_color* color,
				bool* active,
				bool force_foreground = true
			) {
				const double overall_box_size = 15.0;
				static engine::fvector2d circle_pos = { 0, 0 };
				static float hue_value = 0.0f;

				engine::fvector2d position(
					curr_position.m_x + 180,
					curr_position.m_y
				);

				filled_rect(
					position,
					engine::fvector2d( overall_box_size, overall_box_size - 8 ),
					*color,
					false,
					false
				);

				rect(
					position,
					engine::fvector2d( overall_box_size, overall_box_size - 8 ),
					engine::math::from_rgb( 1, 1, 1 ),
					1.5f,
					false
				);

				bool is_widget_hovered = this->is_hovered(
					position,
					engine::fvector2d( overall_box_size, overall_box_size - 8 )
				);

				if ( m_left_mouse_clicked ) {
					if ( is_widget_hovered ) {
						*active = !*active;
					}
					else {
						is_widget_hovered = this->is_hovered(
							position,
							engine::fvector2d( 300, 210 )
						);
						if ( !is_widget_hovered )
							*active = false;
					}
				}

				if ( *active ) {
					engine::fvector2d picker_pos(
						position.m_x + overall_box_size + 6,
						position.m_y
					);

					filled_rect(
						picker_pos,
						engine::fvector2d( 300, 210 ),
						engine::math::from_rgb( 21, 21, 21 ),
						false,
						force_foreground
					);

					rect(
						picker_pos,
						engine::fvector2d( 300, 210 ),
						engine::math::from_rgb( 48, 48, 48 ),
						1.5f,
						force_foreground
					);

					filled_rect(
						picker_pos,
						engine::fvector2d( 300, 2.5f ),
						engine::math::from_rgb( 170, 85, 235 ),
						false,
						force_foreground
					);

					engine::fvector2d square_pos( picker_pos.m_x + 10, picker_pos.m_y + 12 );
					engine::fvector2d square_size( 280, 160 );

					const float color_size = 10.0f;
					const int cols = 28;
					const int rows = 16;

					engine::flinear_color hue_color = get_hue_color( hue_value );
					engine::fvector2d color_pos = square_pos;

					for ( int y = 0; y < rows; y++ ) {
						for ( int x = 0; x < cols; x++ ) {
							float saturation = x / ( float )( cols - 1 );
							float value = 1.0f - ( y / ( float )( rows - 1 ) );

							engine::flinear_color grid_color = hsv_to_rgb( hue_value, saturation, value );

							filled_rect(
								color_pos,
								engine::fvector2d( color_size, color_size ),
								grid_color,
								false,
								force_foreground
							);

							color_pos.m_x += color_size;
						}
						color_pos.m_x = square_pos.m_x;
						color_pos.m_y += color_size;
					}

					engine::fvector2d slider_pos( picker_pos.m_x + 10, picker_pos.m_y + 182 );
					engine::fvector2d slider_size( 280, 20 );

					for ( float x = 0; x < slider_size.m_x; x++ ) {
						float hue = x / slider_size.m_x;
						engine::flinear_color hue_color = get_hue_color( hue );

						line(
							engine::fvector2d( slider_pos.m_x + x, slider_pos.m_y ),
							engine::fvector2d( slider_pos.m_x + x, slider_pos.m_y + slider_size.m_y ),
							hue_color,
							1.0f,
							force_foreground
						);
					}

					rect(
						slider_pos,
						slider_size,
						engine::math::from_rgb( 48, 48, 48 ),
						1.5f,
						force_foreground
					);

					float handle_x = slider_pos.m_x + ( hue_value * slider_size.m_x );

					circle(
						engine::fvector2d( handle_x, slider_pos.m_y + slider_size.m_y / 2 ),
						engine::math::from_rgb( 48, 48, 48 ),
						6.0,
						32,
						false,
						force_foreground
					);

					circle(
						engine::fvector2d( handle_x, slider_pos.m_y + slider_size.m_y / 2 ),
						engine::math::from_rgb( 255, 255, 255 ),
						5.0,
						32,
						false,
						force_foreground
					);

					bool square_hovered = is_hovered( square_pos, square_size );
					bool slider_hovered = is_hovered( slider_pos, slider_size );
					if ( m_left_mouse_down ) {
						if ( slider_hovered ) {
							hue_value = ( m_mouse_position.m_x - slider_pos.m_x ) / slider_size.m_x;
							hue_value = std::clamp( hue_value, 0.0f, 1.0f );
						}
						else if ( square_hovered ) {
							circle_pos = m_mouse_position;

							float saturation = ( m_mouse_position.m_x - square_pos.m_x ) / square_size.m_x;
							float value = 1.0f - ( ( m_mouse_position.m_y - square_pos.m_y ) / square_size.m_y );

							saturation = std::clamp( saturation, 0.0f, 1.0f );
							value = std::clamp( value, 0.0f, 1.0f );

							*color = hsv_to_rgb( hue_value, saturation, value );
						}
					}

					if ( circle_pos.m_x != 0 && circle_pos.m_y != 0 ) {
						circle(
							circle_pos,
							engine::math::from_rgb( 255, 255, 255 ),
							5.0,
							32,
							false,
							force_foreground
						);
					}
				}
			}

			void toggle_box(
				const wchar_t* label,
				engine::fvector2d position,
				bool* value,
				bool force_foreground = false
			) {
				const float box_size = 16.0f;
				const float text_padding = 24.0f;

				engine::fvector2d box_position = position;

				rect(
					box_position,
					engine::fvector2d( box_size, box_size ),
					engine::math::from_rgb( 48, 48, 48 ),
					1.0f,
					force_foreground
				);

				if ( *value ) {
					gradient_rect(
						engine::fvector2d( box_position.m_x + 2, box_position.m_y + 2 ),
						engine::fvector2d( box_size - 4, box_size - 4 ),
						engine::math::from_rgb( 101, 51, 141 ),
						engine::math::from_rgb( 170, 85, 235 ),
						1.f,
						force_foreground
					);
				}

				text(
					engine::fstring( label ),
					engine::fvector2d( box_position.m_x + text_padding, box_position.m_y + ( box_size / 2 ) - 2 ),
					engine::math::from_rgb( 140, 140, 140 ),
					10.0,
					false,
					true,
					false,
					force_foreground
				);

				auto is_hover = is_hovered(
					box_position,
					engine::fvector2d( box_size + text_padding + 50, box_size )
				);
				if ( is_hover && m_left_mouse_clicked )
					*value = !*value;
			}

			void slider(
				const wchar_t* label,
				engine::fvector2d position,
				float* value,
				float min_value,
				float max_value,
				float width = 180.0f,
				bool force_foreground = false
			) {
				const float height = 6.0f;
				const float label_spacing = 20.0f;
				const float thumb_radius = 4.0f;

				static bool s_slider_dragging = false;
				static float* s_current_slider = nullptr;
				static engine::fvector2d s_initial_click_pos;

				text(
					engine::fstring( label ),
					engine::fvector2d( position.m_x, position.m_y ),
					engine::math::from_rgb( 140, 140, 140 ),
					10.0,
					false,
					false,
					false,
					force_foreground
				);

				position.m_y += label_spacing;

				filled_rect(
					position,
					engine::fvector2d( width, height ),
					engine::math::from_rgb( 41, 41, 41 ),
					true,
					force_foreground
				);

				rect(
					position,
					engine::fvector2d( width, height ),
					engine::math::from_rgb( 48, 48, 48 ),
					1.0f,
					force_foreground
				);

				float fill_width = ( ( *value - min_value ) / ( max_value - min_value ) ) * width;

				gradient_rect(
					position,
					engine::fvector2d( fill_width, height ),
					engine::math::from_rgb( 101, 51, 141 ),
					engine::math::from_rgb( 170, 85, 235 ),
					1.0f,
					force_foreground
				);

				const bool is_thumb_hovered = is_hovered( position, engine::fvector2d( width + 3.0, height + 3.0 ) );
				if ( m_left_mouse_clicked && is_thumb_hovered ) {
					s_slider_dragging = true;
					s_current_slider = value;
					s_initial_click_pos = m_mouse_position;
				}

				if ( s_slider_dragging && s_current_slider == value ) {
					if ( !m_left_mouse_down ) {
						s_slider_dragging = false;
						s_current_slider = nullptr;
					}
					else {
						const float mouse_x_offset = m_mouse_position.m_x - position.m_x;
						const float percentage = std::clamp( mouse_x_offset / width, 0.0f, 1.0f );
						*value = min_value + ( max_value - min_value ) * percentage;
					}
				}

				auto built_string = engine::fstring( std::format( L"{:.1f}", *value ).c_str( ) );
				built_string = engine::kismet::g_string_library->concat_str_str(
					built_string,
					L"%"
				);

				text(
					engine::fstring( built_string ),
					engine::fvector2d( position.m_x + fill_width - 15.0f, position.m_y ),
					engine::math::from_rgb( 140, 140, 140 ),
					10.0,
					true,
					true,
					true,
					force_foreground
				);
			}

			void combo_box(
				const wchar_t* label,
				engine::fvector2d position,
				int* current_item,
				const std::vector<engine::fstring>& items,
				bool draw_item_label = true,
				bool force_foreground = true
			) {
				const float box_width = 150.0f;
				const float box_height = 20.0f;
				const float text_padding = 10.0f;

				static bool is_open = false;
				static int* active_combo = nullptr;

				filled_rect(
					position,
					engine::fvector2d( box_width, box_height ),
					engine::math::from_rgb( 41, 41, 41 ),
					false,
					false
				);

				rect(
					position,
					engine::fvector2d( box_width, box_height ),
					engine::math::from_rgb( 48, 48, 48 ),
					1.0f,
					false
				);

				if ( *current_item >= 0 && *current_item < items.size( ) ) {
					auto lowercase_label = engine::kismet::g_string_library->to_lower( label );

					text(
						lowercase_label,
						engine::fvector2d( position.m_x + 9.0f, position.m_y + box_height / 2 - 1.0 ),
						engine::math::from_rgb( 140, 140, 140 ),  // Gray color for label
						10.0,
						false,
						true,
						false,
						force_foreground
					);

					auto label_len = engine::kismet::g_string_library->len( lowercase_label );
					float label_width = label_len * 5.5f;

					text(
						L": ",
						engine::fvector2d( position.m_x + 9.0f + label_width + 2.0f, position.m_y + box_height / 2 - 1.0 ),
						engine::math::from_rgb( 140, 140, 140 ),
						10.0,
						false,
						true,
						false,
						force_foreground
					);

					auto lowercase_item = engine::kismet::g_string_library->to_lower( items[ *current_item ] );
					text(
						lowercase_item,
						engine::fvector2d( position.m_x + 9.0f + label_width + 11.0f, position.m_y + box_height / 2 - 1.0 ),
						engine::math::from_rgb( 170, 85, 235 ),  // Purple color for selected item
						10.0,
						false,
						true,
						false,
						force_foreground
					);
				}

				const float arrow_size = 5.0f;
				const float arrow_padding = 8.0f;
				engine::fvector2d arrow_pos(
					position.m_x + box_width - arrow_padding - arrow_size - 5.0,
					position.m_y + box_height / 2 - 2.0
				);

				line(
					arrow_pos,
					engine::fvector2d( arrow_pos.m_x + arrow_size, arrow_pos.m_y + arrow_size ),
					engine::math::from_rgb( 140, 140, 140 ),
					1.0f,
					force_foreground
				);
				line(
					engine::fvector2d( arrow_pos.m_x + arrow_size, arrow_pos.m_y + arrow_size ),
					engine::fvector2d( arrow_pos.m_x + arrow_size * 2, arrow_pos.m_y ),
					engine::math::from_rgb( 140, 140, 140 ),
					1.0f,
					force_foreground
				);

				bool is_combo_hovered = is_hovered( position, engine::fvector2d( box_width, box_height ) );
				if ( is_combo_hovered && m_left_mouse_clicked ) {
					if ( is_open && active_combo == current_item ) {
						is_open = false;
						active_combo = nullptr;
					}
					else {
						is_open = true;
						active_combo = current_item;
					}
				}

				if ( is_open && active_combo == current_item ) {
					const float dropdown_height = items.size( ) * box_height;
					engine::fvector2d dropdown_pos( position.m_x, position.m_y + box_height );

					filled_rect(
						dropdown_pos,
						engine::fvector2d( box_width, dropdown_height ),
						engine::math::from_rgb( 41, 41, 41 ),
						false,
						force_foreground
					);

					rect(
						dropdown_pos,
						engine::fvector2d( box_width, dropdown_height ),
						engine::math::from_rgb( 48, 48, 48 ),
						1.0f,
						force_foreground
					);

					for ( int i = 0; i < items.size( ); i++ ) {
						engine::fvector2d item_pos( dropdown_pos.m_x, dropdown_pos.m_y + i * box_height );
						bool is_item_hovered = is_hovered( item_pos, engine::fvector2d( box_width, box_height ) );

						if ( is_item_hovered ) {
							filled_rect(
								item_pos,
								engine::fvector2d( box_width, box_height ),
								engine::math::from_rgb( 51, 51, 51 ),
								false,
								force_foreground
							);
						}

						text(
							items[ i ],
							engine::fvector2d( item_pos.m_x + text_padding, item_pos.m_y + box_height / 2 ),
							i == *current_item ?
							engine::math::from_rgb( 170, 85, 235 ) :
							engine::math::from_rgb( 140, 140, 140 ),
							10.0,
							false,
							true,
							false,
							force_foreground
						);

						if ( is_item_hovered && m_left_mouse_clicked ) {
							*current_item = i;
							is_open = false;
							active_combo = nullptr;
						}
					}

					if ( m_left_mouse_clicked && !is_hovered(
						dropdown_pos,
						engine::fvector2d( box_width, dropdown_height ) ) &&
						!is_combo_hovered ) {
						is_open = false;
						active_combo = nullptr;
					}
				}
			}
		};
	}
}