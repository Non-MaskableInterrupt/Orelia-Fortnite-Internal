namespace drawing {
	namespace menu {
		class c_menu : public widgets::c_widgets {
		public:
			void get_mouse_state(
				engine::a_player_controller* player_controller
			) {
				m_mouse_position = player_controller->get_mouse_position( );
				m_left_mouse_down = player_controller->is_input_key_down( engine::keys::g_left_mouse );
				m_left_mouse_clicked = player_controller->was_input_key_just_pressed( engine::keys::g_left_mouse );
				m_right_mouse_down = player_controller->is_input_key_down( engine::keys::g_right_mouse );
				m_right_mouse_clicked = player_controller->is_input_key_down( engine::keys::g_right_mouse );
			}

			bool is_gui_enabled(
				engine::a_player_controller* player_controller
			) {
				static const std::vector<engine::fkey> menu_keys = {
					engine::keys::g_insert,
					engine::keys::g_f7,
					engine::keys::g_f8,
				};

				if ( player_controller->was_input_key_just_pressed( menu_keys[ features::misc::menu_key_index ] ) )
					m_is_gui_active = !m_is_gui_active;
				player_controller->set_ignore_look_input( m_is_gui_active );
				return m_is_gui_active;
			}

			void render_menu(
				const wchar_t* title_name,
				static int* menu_tab
			) {
				m_menu_size = engine::fvector2d( 516.0, 560.0 );

				static auto click_position = engine::fvector2d( 0.0, 0.0 );
				m_window_hovered = is_hovered(
					engine::fvector2d( m_menu_position.m_x, m_menu_position.m_y - 5.0 ),
					engine::fvector2d( m_menu_size.m_x, 20.0 )
				);
				m_window_grabbed = m_window_hovered ? m_left_mouse_down : m_window_hovered && m_left_mouse_down;
				if ( m_window_grabbed ) {
					if ( !click_position ) {
						click_position = engine::fvector2d(
							m_mouse_position.m_x - m_menu_position.m_x,
							m_mouse_position.m_y - m_menu_position.m_y
						);
					}

					m_window_hovered = true;
				}

				if ( click_position ) {
					m_menu_position = engine::fvector2d(
						m_mouse_position.m_x - ( m_menu_size.m_x / 2 ),
						m_mouse_position.m_y - ( 20.0 / 2.0 )
					);
				}

				if ( !m_left_mouse_down ) {
					click_position = engine::fvector2d( );
				}

				filled_rect(
					m_menu_position,
					m_menu_size,
					engine::math::from_rgb( 19, 19, 19 ),
					1.f
				);

				rect(
					m_menu_position,
					m_menu_size,
					engine::math::from_rgb( 48, 48, 48 ),
					2.5f
				);

				m_offset.m_x = 7.0;
				m_offset.m_y = 6.0;

				float bar_center_x = m_menu_position.m_x + m_offset.m_x + ( m_menu_size.m_x - ( m_offset.m_x * 2.0 ) ) / 2.0;
				float bar_center_y = m_menu_position.m_y + m_offset.m_y + ( 2.5 / 2.0 );

				text(
					title_name,
					engine::fvector2d( bar_center_x, bar_center_y ),
					engine::math::from_rgb( 170, 85, 235 ),
					10.0,
					true,
					false,
					false
				);

				m_offset.m_y += 18.0;

				filled_rect(
					engine::fvector2d( m_menu_position.m_x + m_offset.m_x, m_menu_position.m_y + m_offset.m_y ),
					engine::fvector2d( m_menu_size.m_x - ( m_offset.m_x * 2.0 ), 2.5 ),
					engine::math::from_rgb( 170, 85, 235 ),
					1.f
				);

				filled_rect(
					engine::fvector2d( m_menu_position.m_x + m_offset.m_x, m_menu_position.m_y + m_offset.m_y ),
					engine::fvector2d( m_menu_size.m_x - ( m_offset.m_x * 2.0 ), 2.5 ),
					engine::math::from_rgb( 101, 51, 141 ),
					1.f
				);
				m_offset.m_y += 9;

				// tab 1
				{
					filled_rect(
						engine::fvector2d( m_menu_position.m_x + m_offset.m_x, m_menu_position.m_y + m_offset.m_y ),
						engine::fvector2d( 83, 30 ),
						engine::math::from_rgb( 41, 41, 41 ),
						1.5f
					);

					rect(
						engine::fvector2d( m_menu_position.m_x + m_offset.m_x, m_menu_position.m_y + m_offset.m_y ),
						engine::fvector2d( 83, 30 ),
						engine::math::from_rgb( 1, 1, 1 ),
						1.5f
					);

					text(
						engine::fstring( L"aimbot" ),
						engine::fvector2d( m_menu_position.m_x + m_offset.m_x + ( 83.0 / 2.0 ), ( m_menu_position.m_y + m_offset.m_y ) + ( 30 / 2 ) - 2 ),
						*menu_tab == 0 ? engine::math::from_rgb( 170, 85, 235 ) : engine::math::from_rgb( 140, 140, 140 ),
						10.0,
						true,
						true,
						false
					);

					auto result = is_hovered(
						engine::fvector2d( m_menu_position.m_x + m_offset.m_x, m_menu_position.m_y + m_offset.m_y ),
						engine::fvector2d( 83, 30 )
					);
					if ( result && m_left_mouse_clicked )
						*menu_tab = 0;

					m_offset.m_x += 83.0;
				}

				// tab 2
				{
					filled_rect(
						engine::fvector2d( m_menu_position.m_x + m_offset.m_x, m_menu_position.m_y + m_offset.m_y ),
						engine::fvector2d( 83, 30 ),
						engine::math::from_rgb( 41, 41, 41 ),
						1.5f
					);

					rect(
						engine::fvector2d( m_menu_position.m_x + m_offset.m_x, m_menu_position.m_y + m_offset.m_y ),
						engine::fvector2d( 83, 30 ),
						engine::math::from_rgb( 1, 1, 1 ),
						1.5f
					);

					text(
						encrypt( L"exploits" ),
						engine::fvector2d( m_menu_position.m_x + m_offset.m_x + ( 83.0 / 2.0 ), ( m_menu_position.m_y + m_offset.m_y ) + ( 30 / 2 ) - 2 ),
						*menu_tab == 1 ? engine::math::from_rgb( 170, 85, 235 ) : engine::math::from_rgb( 140, 140, 140 ),
						10.0,
						true,
						true,
						false
					);

					auto result = is_hovered(
						engine::fvector2d( m_menu_position.m_x + m_offset.m_x, m_menu_position.m_y + m_offset.m_y ),
						engine::fvector2d( 83, 30 )
					);
					if ( result && m_left_mouse_clicked )
						*menu_tab = 1;

					m_offset.m_x += 83.0;
				}

				// tab 3
				{
					filled_rect(
						engine::fvector2d( m_menu_position.m_x + m_offset.m_x, m_menu_position.m_y + m_offset.m_y ),
						engine::fvector2d( 83, 30 ),
						engine::math::from_rgb( 41, 41, 41 ),
						1.5f
					);

					rect(
						engine::fvector2d( m_menu_position.m_x + m_offset.m_x, m_menu_position.m_y + m_offset.m_y ),
						engine::fvector2d( 83, 30 ),
						engine::math::from_rgb( 1, 1, 1 ),
						1.5f
					);

					text(
						engine::fstring( L"visuals" ),
						engine::fvector2d( m_menu_position.m_x + m_offset.m_x + ( 83.0 / 2.0 ), ( m_menu_position.m_y + m_offset.m_y ) + ( 30 / 2 ) - 2 ),
						*menu_tab == 2 ? engine::math::from_rgb( 170, 85, 235 ) : engine::math::from_rgb( 140, 140, 140 ),
						10.0,
						true,
						true,
						false
					);

					auto result = is_hovered(
						engine::fvector2d( m_menu_position.m_x + m_offset.m_x, m_menu_position.m_y + m_offset.m_y ),
						engine::fvector2d( 83, 30 )
					);
					if ( result && m_left_mouse_clicked )
						*menu_tab = 2;

					m_offset.m_x += 83.0;
				}

				// tab 4
				{
					filled_rect(
						engine::fvector2d( m_menu_position.m_x + m_offset.m_x, m_menu_position.m_y + m_offset.m_y ),
						engine::fvector2d( 83, 30 ),
						engine::math::from_rgb( 41, 41, 41 ),
						1.5f
					);

					rect(
						engine::fvector2d( m_menu_position.m_x + m_offset.m_x, m_menu_position.m_y + m_offset.m_y ),
						engine::fvector2d( 83, 30 ),
						engine::math::from_rgb( 1, 1, 1 ),
						1.5f
					);

					text(
						engine::fstring( L"misc" ),
						engine::fvector2d( m_menu_position.m_x + m_offset.m_x + ( 83.0 / 2.0 ), ( m_menu_position.m_y + m_offset.m_y ) + ( 30 / 2 ) - 2 ),
						*menu_tab == 3 ? engine::math::from_rgb( 170, 85, 235 ) : engine::math::from_rgb( 140, 140, 140 ),
						10.0,
						true,
						true,
						false
					);

					auto result = is_hovered(
						engine::fvector2d( m_menu_position.m_x + m_offset.m_x, m_menu_position.m_y + m_offset.m_y ),
						engine::fvector2d( 83, 30 )
					);
					if ( result && m_left_mouse_clicked )
						*menu_tab = 3;

					m_offset.m_x += 83.0;
				}

				// tab 5
				{
					filled_rect(
						engine::fvector2d( m_menu_position.m_x + m_offset.m_x, m_menu_position.m_y + m_offset.m_y ),
						engine::fvector2d( 83, 30 ),
						engine::math::from_rgb( 41, 41, 41 ),
						1.5f
					);

					rect(
						engine::fvector2d( m_menu_position.m_x + m_offset.m_x, m_menu_position.m_y + m_offset.m_y ),
						engine::fvector2d( 83, 30 ),
						engine::math::from_rgb( 1, 1, 1 ),
						1.5f
					);

					text(
						engine::fstring( L"config" ),
						engine::fvector2d( m_menu_position.m_x + m_offset.m_x + ( 83.0 / 2.0 ), ( m_menu_position.m_y + m_offset.m_y ) + ( 30 / 2 ) - 2 ),
						*menu_tab == 4 ? engine::math::from_rgb( 170, 85, 235 ) : engine::math::from_rgb( 140, 140, 140 ),
						10.0,
						true,
						true,
						false
					);

					auto result = is_hovered(
						engine::fvector2d( m_menu_position.m_x + m_offset.m_x, m_menu_position.m_y + m_offset.m_y ),
						engine::fvector2d( 83, 30 )
					);
					if ( result && m_left_mouse_clicked )
						*menu_tab = 4;

					m_offset.m_x += 83.0;
				}

				// tab 6
				{
					filled_rect(
						engine::fvector2d( m_menu_position.m_x + m_offset.m_x, m_menu_position.m_y + m_offset.m_y ),
						engine::fvector2d( 83, 30 ),
						engine::math::from_rgb( 41, 41, 41 ),
						1.5f
					);

					rect(
						engine::fvector2d( m_menu_position.m_x + m_offset.m_x, m_menu_position.m_y + m_offset.m_y ),
						engine::fvector2d( 83, 30 ),
						engine::math::from_rgb( 1, 1, 1 ),
						1.5f
					);

					text(
						engine::fstring( L"world" ),
						engine::fvector2d( m_menu_position.m_x + m_offset.m_x + ( 83.0 / 2.0 ), ( m_menu_position.m_y + m_offset.m_y ) + ( 30 / 2 ) - 2 ),
						*menu_tab == 5 ? engine::math::from_rgb( 170, 85, 235 ) : engine::math::from_rgb( 140, 140, 140 ),
						10.0,
						true,
						true,
						false
					);

					auto result = is_hovered(
						engine::fvector2d( m_menu_position.m_x + m_offset.m_x, m_menu_position.m_y + m_offset.m_y ),
						engine::fvector2d( 83, 30 )
					);
					if ( result && m_left_mouse_clicked )
						*menu_tab = 5;

					m_offset.m_x += 83.0;
				}

				m_offset.m_x -= 498.0;
				m_offset.m_y += 44.0;

				filled_rect(
					engine::fvector2d( m_menu_position.m_x + m_offset.m_x, m_menu_position.m_y + m_offset.m_y ),
					engine::fvector2d( m_menu_size.m_x - ( m_offset.m_x * 2.0 ), m_menu_size.m_y - m_offset.m_y - 11.0 ),
					engine::math::from_rgb( 21, 21, 21 ),
					1.f
				);

				rect(
					engine::fvector2d( m_menu_position.m_x + m_offset.m_x, m_menu_position.m_y + m_offset.m_y ),
					engine::fvector2d( m_menu_size.m_x - ( m_offset.m_x * 2.0 ), m_menu_size.m_y - m_offset.m_y - 11.0 ),
					engine::math::from_rgb( 1, 1, 1 ),
					1.5f
				);

				m_offset.m_y += 12.0;
			}

			void draw_columns( int menu_tab ) {
				engine::fvector2d left_window_pos = { m_menu_position.m_x + m_offset.m_x + 5, m_menu_position.m_y + m_offset.m_y };
				engine::fvector2d left_window_size = { 240, 449 };

				engine::fvector2d right_window_pos = { m_menu_position.m_x + m_offset.m_x + 255, m_menu_position.m_y + m_offset.m_y };
				engine::fvector2d right_window_size = { 240, 200 };

				float effects_y = m_menu_position.m_y + m_offset.m_y + 210;
				engine::fvector2d bottom_window_pos = { m_menu_position.m_x + m_offset.m_x + 255, effects_y };
				engine::fvector2d bottom_window_size = { 240, 239 };

				if ( menu_tab == 0 ) {
					begin_scroll_area( true, left_window_pos, left_window_size, menu_tab, L"aimbot" );
					begin_scroll_area( false, right_window_pos, right_window_size, menu_tab, L"crosshair" );
					begin_scroll_area( false, bottom_window_pos, bottom_window_size, menu_tab, L"triggerbot" );

					engine::fvector2d curr_offset;
					curr_offset.m_y = 15.0;
					curr_offset.m_x = 15.0;

					auto pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"enable", pos, &features::aimbot::enabled );
					}

					curr_offset.m_y += 23.0f;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 24.0,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						std::vector<engine::fstring> key_options = { L"rmb", L"lshift", L"lalt", L"thumb", L"thumb1", L"controller" };
						combo_box( L"keybind", pos, &features::aimbot::keybind, key_options );
					}

					curr_offset.m_y += 27.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"target line", pos, &features::aimbot::target_line );
						static bool line_active = false;
						color_picker( pos, &features::aimbot::line_color, &line_active, false );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"target dot", pos, &features::aimbot::target_dot );
						static bool dot_active = false;
						color_picker( pos, &features::aimbot::dot_color, &dot_active );
					}

					curr_offset.m_y += 20.f;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"fov circle", pos, &features::aimbot::draw_fov );
						static bool fov_active = false;
						color_picker( pos, &features::aimbot::fov_color, &fov_active );
					}

					curr_offset.m_y += 20.f;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 24.0,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						slider( L"field of view", pos, &features::aimbot::fov_radius, 0.0f, 90.0f, 150.0f );
					}

					curr_offset.m_y += 35.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"projectile prediction", pos, &features::aimbot::engine_prediction );
					}

					curr_offset.m_y += 23.0f;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 24.0,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						std::vector<engine::fstring> options = { L"head", L"neck", L"body" };
						combo_box( L"aim bone", pos, &features::aimbot::aimbone, options, false );
					}

					curr_offset.m_y += 27.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"sticky target", pos, &features::aimbot::constant );
					}

					curr_offset.m_y += 20.f;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"aim curve", pos, &features::aimbot::curve );
					}

					curr_offset.m_y += 20.f;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 24.0,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						slider( L"smoothing", pos, &features::aimbot::smoothing, 0.0f, 20.0f, 150.0f );
					}

					curr_offset.m_y += 35.f;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"deadzone", pos, &features::aimbot::deadzone );
					}

					curr_offset.m_y += 20.f;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 24.0,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						slider( L"restriction", pos, &features::aimbot::restriction, 0.0f, 25.0f, 150.0f );
					}

					curr_offset.m_y += 35.f;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"only visible", pos, &features::aimbot::only_visible );
					}

					curr_offset.m_y += 20.f;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"ignore bots", pos, &features::aimbot::ignore_bots );
					}

					curr_offset.m_y += 20.f;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"ignore downed", pos, &features::aimbot::ignore_dbno );
					}

					curr_offset.m_y += 20.f;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 24.0,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						slider( L"max distance", pos, &features::aimbot::max_distance, 0.0f, 600.0f, 150.0f );
					}

					curr_offset.m_y = 15.0;
					curr_offset.m_x = 255.0 + 15.0;

					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						toggle_box( L"enable", pos, &features::aimbot::crosshair );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						toggle_box( L"hide reticle", pos, &features::aimbot::hide_reticle );
					}

					curr_offset.m_y += 20.f;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 24.0,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						slider( L"crosshair size", pos, &features::aimbot::crosshair_size, 1.0f, 15.0f, 150.0f );
					}

					curr_offset.m_y = 20.0;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						effects_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						toggle_box( L"enable", pos, &features::aimbot::trigger_bot );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						effects_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						toggle_box( L"activation key", pos, &features::aimbot::trigger_key );
					}

					curr_offset.m_y += 23.0f;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 24.0,
						effects_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						std::vector<engine::fstring> key_options = { L"lshift", L"lalt", L"thumb", L"thumb", L"thumb1" };
						combo_box( L"keybind", pos, &features::aimbot::triggerbot_key, key_options );
					}

					curr_offset.m_y += 27.0;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						effects_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						toggle_box( L"shotgun only", pos, &features::aimbot::shotgun_only );
					}

					curr_offset.m_y += 20.f;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 24.0,
						effects_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						slider( L"activation time", pos, &features::aimbot::triggerbot_time, 0.0f, 10.0f, 150.0f );
					}

					curr_offset.m_y += 35.0;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 24.0,
						effects_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						slider( L"max distance", pos, &features::aimbot::trigger_distance, 0.0f, 300.0f, 150.0f );
					}
				}
				else if ( menu_tab == 1 ) {
					begin_scroll_area( true, left_window_pos, left_window_size, menu_tab, L"exploits" );
					begin_scroll_area( false, right_window_pos, right_window_size, menu_tab, L"desync" );
					begin_scroll_area( false, bottom_window_pos, bottom_window_size, menu_tab, L"weapon" );

					engine::fvector2d curr_offset;
					curr_offset.m_y = 15.0;
					curr_offset.m_x = 15.0;

					auto pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"enable", pos, &features::exploits::enabled );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"chams", pos, &features::exploits::chams );
						static bool active = false;
						color_picker( pos, &features::exploits::chams_color, &active );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"weapon chams", pos, &features::exploits::weapon_chams );
						static bool weapon_active = false;
						color_picker( pos, &features::exploits::weapon_color, &weapon_active );
					}

					curr_offset.m_y += 23.0f;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 24.0,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						std::vector<engine::fstring> options = { L"glow", L"wave", L"highlight", L"defuse", L"holographic" };
						combo_box( L"material", pos, &features::exploits::chams_material, options, false );
					}

					curr_offset.m_y += 27.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"wireframe", pos, &features::exploits::wireframe );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"xray chams", pos, &features::exploits::xray_chams );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 24.0,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						slider( L"emissive", pos, &features::exploits::chams_emissive, 0.0f, 600.0f, 150.0f );
					}

					curr_offset.m_y += 35.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"crown win", pos, &features::exploits::crown_win );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						toggle_box( L"player fly", pos, &features::exploits::player_fly );
					}

					curr_offset.m_y = 15.0;
					curr_offset.m_x = 255.0 + 15.0;

					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						toggle_box( L"enable", pos, &features::exploits::desync );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						toggle_box( L"bhop", pos, &features::exploits::bhop );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						toggle_box( L"no clip", pos, &features::exploits::no_clip );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						toggle_box( L"super jump", pos, &features::exploits::super_jump );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						toggle_box( L"super slide", pos, &features::exploits::super_slide );
					}

					curr_offset.m_y = 15.0;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						effects_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						toggle_box( L"shoot through walls", pos, &features::exploits::shoot_through_walls );
					}

					curr_offset.m_y += 20;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						effects_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						toggle_box( L"no recoil", pos, &features::exploits::no_recoil );
					}

					curr_offset.m_y += 20;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						effects_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						toggle_box( L"rapid fire", pos, &features::exploits::rapid_fire );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 24.0,
						effects_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						slider( L"fire speed", pos, &features::exploits::fire_speed, 0.f, 10.f, 150.0f );
					}

					curr_offset.m_y += 35.0;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						effects_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						toggle_box( L"hit sound", pos, &features::exploits::hit_sound );
					}

					curr_offset.m_y += 23.0f;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 24.0,
						effects_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						std::vector<engine::fstring> options = { L"fire flys", L"chest", L"shield potion", L"battle bus", L"vending machine" };
						combo_box( L"sound", pos, &features::exploits::hit_sounds, options, false );
					}
				}
				else if ( menu_tab == 2 ) {
					begin_scroll_area( true, left_window_pos, left_window_size, menu_tab, L"visuals" );
					begin_scroll_area( false, right_window_pos, right_window_size, menu_tab, L"radar" );
					begin_scroll_area( false, bottom_window_pos, bottom_window_size, menu_tab, L"colors" );

					engine::fvector2d curr_offset;
					curr_offset.m_y = 15.0;
					curr_offset.m_x = 15.0;

					// Left column (visuals)
					auto pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"enable", pos, &features::visuals::enabled );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"box esp", pos, &features::visuals::box_esp );
						static bool box_active = false;
						color_picker( pos, &features::visuals::box_color, &box_active );

						auto second_picker_pos = get_scroll_adjusted_pos( true, {
							m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 20.0,
							m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
							} );
						static bool invis_box_active = false;
						color_picker( second_picker_pos, &features::visuals::invis_box_color, &invis_box_active );
					}

					curr_offset.m_y += 23.0f;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 24.0,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						std::vector<engine::fstring> options = { L"2d", L"corner", L"3d" };
						combo_box( L"box type", pos, &features::visuals::box_type, options, false );
					}

					curr_offset.m_y += 27.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"filled box", pos, &features::visuals::filled_box );
						static bool fill_active = false;
						color_picker( pos, &features::visuals::filled_color, &fill_active );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"outline box", pos, &features::visuals::outline );
						static bool outline_active = false;
						color_picker( pos, &features::visuals::outline_color, &outline_active );

						static bool invis_outline_active = false;
						auto second_picker_pos = get_scroll_adjusted_pos( true, {
							m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 20.0,
							m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
							} );
						color_picker( second_picker_pos, &features::visuals::invis_outline_color, &invis_outline_active );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"skeleton", pos, &features::visuals::skeleton );
						static bool skeleton_active = false;
						color_picker( pos, &features::visuals::skeleton_color, &skeleton_active );

						static bool invis_skeleton_active = false;
						auto second_picker_pos = get_scroll_adjusted_pos( true, {
							m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 20.0,
							m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
							} );
						color_picker( second_picker_pos, &features::visuals::invis_skeleton_color, &invis_skeleton_active );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"snap line", pos, &features::visuals::snaplines );
						static bool line_active = false;
						color_picker( pos, &features::visuals::line_color, &line_active );

						static bool invis_line_active = false;
						auto second_picker_pos = get_scroll_adjusted_pos( true, {
							m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 20.0,
							m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
							} );
						color_picker( second_picker_pos, &features::visuals::invis_line_color, &invis_line_active );
					}

					curr_offset.m_y += 23.0f;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 24.0,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						std::vector<engine::fstring> line_options = { L"top", L"center", L"bottom" };
						combo_box( L"position", pos, &features::visuals::line_type, line_options, false );
					}

					curr_offset.m_y += 27.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"bullet traces", pos, &features::visuals::bullet_tracers );
						static bool tracers_active = false;
						color_picker( pos, &features::visuals::tracer_color, &tracers_active );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"hit markers", pos, &features::visuals::hit_marker );
						static bool marker_active = false;
						color_picker( pos, &features::visuals::marker_color, &marker_active );
					}

					curr_offset.m_y += 20.f;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 24.0,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						slider( L"tracer seconds", pos, &features::visuals::tracer_time, 0.1f, 10.0f, 150.0f );
					}

					curr_offset.m_y += 35.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"view direction", pos, &features::visuals::view_direction );
						static bool view_active = false;
						color_picker( pos, &features::visuals::view_color, &view_active );

						auto second_picker_pos = get_scroll_adjusted_pos( true, {
							m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 20.0,
							m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
							} );
						static bool invis_box_active = false;
						color_picker( second_picker_pos, &features::visuals::invis_box_color, &invis_box_active );
					}

					curr_offset.m_y += 20.f;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 24.0,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						slider( L"view distance", pos, &features::visuals::view_distance, 0.0f, 260.0f, 150.0f );
					}

					curr_offset.m_y += 35.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"weapon", pos, &features::visuals::weapon );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"ammo bar", pos, &features::visuals::ammo_bar );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"inventory", pos, &features::visuals::inventory_esp );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"damage", pos, &features::visuals::damage_esp );
						static bool view_active = false;
						color_picker( pos, &features::visuals::damage_color, &view_active );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"storm prediction", pos, &features::visuals::storm_prediction );
						static bool view_active = false;
						color_picker( pos, &features::visuals::storm_color, &view_active );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"specator esp", pos, &features::visuals::specator_esp );
						static bool view_active = false;
						color_picker( pos, &features::visuals::specator_color, &view_active );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"kill feed", pos, &features::visuals::kill_feed );
						static bool view_active = false;
						color_picker( pos, &features::visuals::kill_feed_color, &view_active );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"rank progress", pos, &features::visuals::rank_esp );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"eliminations", pos, &features::visuals::kills );
						static bool view_active = false;
						color_picker( pos, &features::visuals::kills_color, &view_active );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"season level", pos, &features::visuals::season_level );
						static bool view_active = false;
						color_picker( pos, &features::visuals::level_color, &view_active );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"fov arrows", pos, &features::visuals::fov_arrows );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"user name", pos, &features::visuals::name );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"platform", pos, &features::visuals::platform );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"distance", pos, &features::visuals::distance );
					}

					// Right column (radar)
					curr_offset.m_y = 15.0;
					curr_offset.m_x = 255.0 + 15.0;

					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						toggle_box( L"enable", pos, &features::visuals::radar );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						toggle_box( L"local player", pos, &features::visuals::local_player );
						static bool local_active = false;
						color_picker( pos, &features::visuals::local_color, &local_active );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						toggle_box( L"fov lines", pos, &features::visuals::fov_lines );
						static bool fov_active = false;
						color_picker( pos, &features::visuals::fov_line_color, &fov_active );
					}

					curr_offset.m_y += 20.f;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 24.0,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						slider( L"meters per ring", pos, &features::visuals::scope, 0.0f, 300.0f, 150.0f );
					}

					curr_offset.m_y += 27.f;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 24.0,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						slider( L"radar size", pos, &features::visuals::radar_size, 0.0f, 600.0f, 150.0f );
					}

					// Bottom section (colors)
					curr_offset.m_y = 25.0;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						effects_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						text( L"text color", pos, engine::math::from_rgb( 140, 140, 140 ), 10.0, false, true, false );
						static bool text_active = false;
						color_picker( pos, &features::visuals::text_color, &text_active );

						auto second_picker_pos = get_scroll_adjusted_pos( false, {
							m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 20.0,
							effects_y + curr_offset.m_y 
						} );
						static bool invis_text_active = false;
						color_picker( second_picker_pos, &features::visuals::invis_text_color, &invis_text_active );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						effects_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						text( L"bot color", pos, engine::math::from_rgb( 140, 140, 140 ), 10.0, false, true, false );
						static bool knocked_active = false;
						color_picker( pos, &features::visuals::bot_color, &knocked_active );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						effects_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						text( L"team color", pos, engine::math::from_rgb( 140, 140, 140 ), 10.0, false, true, false );
						static bool team_active = false;
						color_picker( pos, &features::visuals::team_color, &team_active );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						effects_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						text( L"knocked color", pos, engine::math::from_rgb( 140, 140, 140 ), 10.0, false, true, false );
						static bool knocked_active = false;
						color_picker( pos, &features::visuals::knocked_color, &knocked_active );
					}
				}
				else if ( menu_tab == 3 ) {
					begin_scroll_area( true, left_window_pos, left_window_size, menu_tab, L"misc" );
					begin_scroll_area( false, right_window_pos, right_window_size, menu_tab, L"menu" );
					begin_scroll_area( false, bottom_window_pos, bottom_window_size, menu_tab, L"version" );

					engine::fvector2d curr_offset;
					curr_offset.m_y = 15.0;
					curr_offset.m_x = 15.0;

					// Left column (misc)
					auto pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"enable", pos, &features::misc::enabled );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"debug objects", pos, &features::misc::debug_objects );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"debug game", pos, &features::misc::game_information );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"debug render", pos, &features::misc::render_count );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"debug targeting", pos, &features::misc::debug_targeting );
					}

					// Right column (settings)
					curr_offset.m_y = 18.0;
					curr_offset.m_x = 255.0 + 15.0;

					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 24.0,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						std::vector<engine::fstring> key_options = { L"insert", L"f7", L"f8" };
						combo_box( L"key", pos, &features::misc::menu_key_index, key_options, false );
					}

					// Bottom section (info)
					curr_offset.m_y = 20;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						effects_y + curr_offset.m_y
						} );
				}
				else if ( menu_tab == 4 ) {
					begin_scroll_area( true, left_window_pos, left_window_size, menu_tab, L"configs" );
					begin_scroll_area( false, right_window_pos, right_window_size, menu_tab, L"options" );
					begin_scroll_area( false, bottom_window_pos, bottom_window_size, menu_tab, L"info" );

					engine::fvector2d curr_offset;
					curr_offset.m_y = 15.0;
					curr_offset.m_x = 15.0;

					// Left column (configs)
					//auto pos = get_scroll_adjusted_pos( true, {
					//	m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 24.0,
					//	m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
					//	} );

					//if ( is_position_visible( true, pos ) ) {
					//	std::vector<engine::fstring> config_options = { L"legit", L"rage", L"hvh", L"custom" };
					//	combo_box( L"config", pos, &features::config::selected_config, config_options, false );
					//}

					//curr_offset.m_y += 27.0;
					//pos = get_scroll_adjusted_pos( true, {
					//	m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
					//	m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
					//	} );

					//if ( is_position_visible( true, pos ) ) {
					//	if ( button( L"load", pos, { 70, 20 } ) ) {
					//		features::config::load( );
					//	}
					//}

					//curr_offset.m_y += 27.0;
					//pos = get_scroll_adjusted_pos( true, {
					//	m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
					//	m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
					//	} );

					//if ( is_position_visible( true, pos ) ) {
					//	if ( button( L"save", pos, { 70, 20 } ) ) {
					//		features::config::save( );
					//	}
					//}

					// Right column (options)
					//curr_offset.m_y = 20.0.0;
					//curr_offset.m_x = 255.0 + 15.0;

					//pos = get_scroll_adjusted_pos( false, {
					//	m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
					//	m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
					//	} );

					//if ( is_position_visible( false, pos ) ) {
					//	if ( button( L"reset", pos, { 70, 20 } ) ) {
					//		features::config::reset( );
					//	}
					//}

					// Bottom section (info)
					//curr_offset.m_y = 20;
					//pos = get_scroll_adjusted_pos( false, {
					//	m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
					//	m_menu_position.m_y + effects_y + curr_offset.m_y
					//	} );

					//if ( is_position_visible( false, pos ) ) {
					//	text( L"config system", pos, engine::math::from_rgb( 140, 140, 140 ), 10.0, false, true, false );
					//}
				}
				else if ( menu_tab == 5 ) {
					begin_scroll_area( true, left_window_pos, left_window_size, menu_tab, L"world" );
					begin_scroll_area( false, right_window_pos, right_window_size, menu_tab, L"effects" );
					begin_scroll_area( false, bottom_window_pos, bottom_window_size, menu_tab, L"colors" );

					engine::fvector2d curr_offset;
					curr_offset.m_y = 15.0;
					curr_offset.m_x = 15.0;

					// Left column (world)
					auto pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"enable", pos, &features::world::enabled );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"loot esp", pos, &features::world::loot_esp );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"loot icons", pos, &features::world::loot_icons );
					}

					curr_offset.m_y += 23.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 24.0,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						std::vector<engine::fstring> options = {
							L"all",
							L"common+",
							L"uncommon+",
							L"rare+",
							L"epic+",
							L"legendary+",
							L"mythic+"
						};
						combo_box( L"filter", pos, &features::world::loot_tier, options, false );
					}

					curr_offset.m_y += 27.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 24.0,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						slider( L"loot distance", pos, &features::world::loot_distance, 0.0f, 400.0f, 150.0f );
					}

					curr_offset.m_y += 35.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"container esp", pos, &features::world::chest_esp );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"container chams", pos, &features::world::chest_chams );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 24.0,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						slider( L"chest distance", pos, &features::world::chest_distance, 0.0f, 600.0f, 150.0f );
					}

					curr_offset.m_y += 35.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"vehicle esp", pos, &features::world::vehicle_esp );
						static bool vehicle_active = false;
						color_picker( pos, &features::world::vehicle_color, &vehicle_active );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"vehicle health", pos, &features::world::vehicle_health );
						static bool vehicle_active = false;
						color_picker( pos, &features::world::health_color, &vehicle_active );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"vehicle chams", pos, &features::world::vehicle_chams );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 24.0,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						slider( L"vehicle distance", pos, &features::world::vehicle_distance, 0.0f, 600.0f, 150.0f );
					}

					curr_offset.m_y += 35.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"weakspot esp", pos, &features::world::weakspot_esp );
						static bool weakspot_color = false;
						color_picker( pos, &features::world::weakspot_color, &weakspot_color );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						toggle_box( L"weakspot aimbot", pos, &features::world::weakspot_aimbot );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( true, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 24.0,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( true, pos ) ) {
						slider( L"weakspot distance", pos, &features::world::weakspot_distance, 0.0f, 100.0f, 150.0f );
					}

					// Right column (effects)
					curr_offset.m_y = 15.0;
					curr_offset.m_x = 255.0 + 15.0;

					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						toggle_box( L"enable", pos, &features::world::effects );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						toggle_box( L"preformence mode", pos, &features::world::preformence_mode );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						toggle_box( L"night mode", pos, &features::world::night_mode );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						toggle_box( L"sky light", pos, &features::world::sky_light );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x + 24.0,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						slider( L"light multiplier", pos, &features::world::sky_light_multiplier, 0.0f, 10.0f, 150.0f );
					}

					curr_offset.m_y += 35.0;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						toggle_box( L"disable shadow", pos, &features::world::disable_shadows );
					}

					curr_offset.m_y += 20.0;
					pos = get_scroll_adjusted_pos( false, {
						m_menu_position.m_x + m_offset.m_x + curr_offset.m_x,
						m_menu_position.m_y + m_offset.m_y + curr_offset.m_y
						} );

					if ( is_position_visible( false, pos ) ) {
						toggle_box( L"disable fog", pos, &features::world::disable_fog );
					}

					// Bottom section (misc)
					curr_offset.m_y = 15.0;
				}

				end_scroll_area( true );
				end_scroll_area( false );
				end_scroll_area( false );
			}
		};
	}
}