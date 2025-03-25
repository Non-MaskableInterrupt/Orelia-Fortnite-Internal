namespace drawing {
	namespace widgets {
		class c_radar : public c_debug {
		public:
			void add_local_player_to_radar(
				const engine::fvector2d& radar_pos,
				float radar_size,
				float fov,
				const engine::frotator& local_rotation,
				bool force_foreground = true
			) {
				const float background_padding = 25.0f;
				const float pi = 3.14159f;

				float center_x = radar_pos.m_x + ( radar_size / 2.0f );
				float center_y = radar_pos.m_y + ( radar_size / 2.0f );

				if ( features::visuals::fov_lines ) {
					const float line_length = ( radar_size / 2.0f ) + background_padding;

					const float fov_rad = ( fov / 2.0f ) * ( pi / 180.0f );
					const float view_yaw_rad = ( -local_rotation.m_yaw - 90.0f ) * ( pi / 180.0f );

					float left_angle = view_yaw_rad - fov_rad;
					float right_angle = view_yaw_rad + fov_rad;

					engine::fvector2d left_end(
						center_x + line_length * cos( left_angle ),
						center_y - line_length * sin( left_angle )
					);

					engine::fvector2d right_end(
						center_x + line_length * cos( right_angle ),
						center_y - line_length * sin( right_angle )
					);

					line(
						engine::fvector2d( center_x, center_y ),
						left_end,
						features::visuals::fov_line_color,
						1.0f,
						force_foreground
					);

					line(
						engine::fvector2d( center_x, center_y ),
						right_end,
						features::visuals::fov_line_color,
						1.0f,
						force_foreground
					);
				}

				if ( features::visuals::local_player ) {
					const float yaw_rad = ( 90.0f - local_rotation.m_yaw ) * ( pi / 180.0f );
					const float cos_yaw = cos( yaw_rad );
					const float sin_yaw = sin( yaw_rad );

					const float triangle_size = 8.f;
					const engine::fvector2d triangle_points[ 3 ] = {
						engine::fvector2d(
							center_x + triangle_size * cos_yaw,
							center_y - triangle_size * sin_yaw
						),
						engine::fvector2d(
							center_x + ( triangle_size / 2.0f ) * cos( yaw_rad + ( 2.0f * pi / 3.0f ) ),
							center_y - ( triangle_size / 2.0f ) * sin( yaw_rad + ( 2.0f * pi / 3.0f ) )
						),
						engine::fvector2d(
							center_x + ( triangle_size / 2.0f ) * cos( yaw_rad - ( 2.0f * pi / 3.0f ) ),
							center_y - ( triangle_size / 2.0f ) * sin( yaw_rad - ( 2.0f * pi / 3.0f ) )
						)
					};

					filled_triangle(
						triangle_points[ 0 ],
						triangle_points[ 1 ],
						triangle_points[ 2 ],
						features::visuals::local_color,
						force_foreground
					);

					line(
						triangle_points[ 0 ],
						triangle_points[ 1 ],
						engine::math::from_rgb( 255, 255, 255 ),
						1.0f,
						force_foreground
					);
					line(
						triangle_points[ 1 ],
						triangle_points[ 2 ],
						engine::math::from_rgb( 255, 255, 255 ),
						1.0f,
						force_foreground
					);
					line(
						triangle_points[ 2 ],
						triangle_points[ 0 ],
						engine::math::from_rgb( 255, 255, 255 ),
						1.0f,
						force_foreground
					);
				}
			}

			bool draw_radar(
				const engine::fvector2d& position = engine::fvector2d( 30.0f, 50.0f ),
				float size = features::visuals::radar_size,
				bool force_foreground = true
			) {
				const float center_x = position.m_x + ( size / 2.0f );
				const float center_y = position.m_y + ( size / 2.0f );
				const engine::fvector2d center( center_x, center_y );
				const float text_y_offset = 12.0f;
				const float background_padding = 25.0f;

				const float meters_per_ring = features::visuals::scope;
				const float inner_ring_meters = meters_per_ring;
				const float middle_ring_meters = meters_per_ring * 2;
				const float outer_ring_meters = meters_per_ring * 3;

				if ( m_debug_box_y == 5.0f && features::visuals::radar ) {
					m_debug_box_y = position.m_y + size + 45.0f;
					const float debug_box_width = 200.0f;
					m_debug_box_x = position.m_x + ( size / 2.0f ) - ( debug_box_width / 2.0f );
				}

				circle(
					center,
					engine::math::from_rgb( 19, 19, 19 ),
					( size / 2.0f ) + background_padding,
					64,
					1.f,
					true,
					force_foreground
				);

				circle(
					center,
					engine::math::from_rgb( 170, 85, 235 ),
					( size / 2.0f ) + background_padding,
					64,
					2.0f,
					false,
					force_foreground
				);

				circle(
					center,
					engine::math::from_rgb( 48, 48, 48 ),
					( size / 2.0f ),
					64,
					2.0f,
					false,
					force_foreground
				);

				circle(
					center,
					engine::math::from_rgb( 48, 48, 48 ),
					size / 6.0f,
					32,
					1.5f,
					false,
					force_foreground
				);

				circle(
					center,
					engine::math::from_rgb( 48, 48, 48 ),
					( size / 2.0f ) * ( 2.0f / 3.0f ),
					48,
					1.5f,
					false,
					force_foreground
				);

				auto draw_distance_text = [ & ]( float meters, float ring_radius ) {
					auto built_text = engine::kismet::g_string_library->build_string_int(
						L"", L"", static_cast< int >( meters ), L"m"
					);

					const float angle = ( 3.0f * 3.14159f ) / 4.0f;
					const float text_offset = 5.0f;

					float text_x = center_x + ( ring_radius - text_offset ) * cos( angle );
					float text_y = center_y + ( ring_radius - text_offset ) * sin( angle );

					text(
						built_text,
						engine::fvector2d( text_x, text_y ),
						engine::math::from_rgb( 48, 48, 48 ),
						12.0,
						true,
						true,
						true,
						force_foreground
					);
					};

				draw_distance_text( inner_ring_meters, size / 6.0f );
				draw_distance_text( middle_ring_meters, ( size / 2.0f ) * ( 2.0f / 3.0f ) );
				draw_distance_text( outer_ring_meters, size / 2.0f );

				line(
					engine::fvector2d( center_x - ( ( size / 2.0f ) + background_padding ), center_y ),
					engine::fvector2d( center_x + ( ( size / 2.0f ) + background_padding ), center_y ),
					engine::math::from_rgb( 48, 48, 48 ),
					1.0f,
					force_foreground
				);

				line(
					engine::fvector2d( center_x, center_y - ( ( size / 2.0f ) + background_padding ) ),
					engine::fvector2d( center_x, center_y + ( ( size / 2.0f ) + background_padding ) ),
					engine::math::from_rgb( 48, 48, 48 ),
					1.0f,
					force_foreground
				);

				return m_radar_drawed;
			}

			void add_player_to_radar(
				const engine::fvector& local_position,
				const engine::frotator& local_rotation,
				const engine::fvector& target_position,
				const engine::fvector2d& radar_pos,
				float radar_size,
				const engine::flinear_color& player_color,
				bool force_foreground = true
			) {
				const float background_padding = 25.0f;
				const float pi = 3.14159f;

				auto dx = target_position.m_x - local_position.m_x;
				auto dy = target_position.m_y - local_position.m_y;

				const float yaw_rad = -local_rotation.m_yaw * ( pi / 180.0f );
				const float cos_yaw = cos( yaw_rad );
				const float sin_yaw = sin( yaw_rad );

				float rotated_x = dx * cos_yaw + dy * sin_yaw;
				float rotated_y = -( dx * sin_yaw - dy * cos_yaw );

				float center_x = radar_pos.m_x + ( radar_size / 2.0f );
				float center_y = radar_pos.m_y + ( radar_size / 2.0f );
				double calculated_range = features::visuals::scope * 100;
				float effective_radius = ( radar_size / 2.0f ) + background_padding;

				engine::fvector2d screen_pos;
				screen_pos.m_x = center_x + ( rotated_x / calculated_range ) * effective_radius;
				screen_pos.m_y = center_y + ( rotated_y / calculated_range ) * effective_radius;

				float dist_from_center = sqrt(
					( screen_pos.m_x - center_x ) * ( screen_pos.m_x - center_x ) +
					( screen_pos.m_y - center_y ) * ( screen_pos.m_y - center_y )
				);

				if ( dist_from_center > effective_radius )
					return;

				float enemy_angle = atan2( rotated_y, rotated_x );
				const float triangle_size = 8.5f;

				engine::fvector2d triangle_points[ 3 ];
				triangle_points[ 0 ] = engine::fvector2d(
					screen_pos.m_x + triangle_size * sin( enemy_angle ),
					screen_pos.m_y - triangle_size * cos( enemy_angle )
				);

				triangle_points[ 1 ] = engine::fvector2d(
					screen_pos.m_x + triangle_size / 2 * sin( enemy_angle + 2.0944f ),
					screen_pos.m_y - triangle_size / 2 * cos( enemy_angle + 2.0944f )
				);

				triangle_points[ 2 ] = engine::fvector2d(
					screen_pos.m_x + triangle_size / 2 * sin( enemy_angle - 2.0944f ),
					screen_pos.m_y - triangle_size / 2 * cos( enemy_angle - 2.0944f )
				);

				filled_triangle(
					triangle_points[ 0 ],
					triangle_points[ 1 ],
					triangle_points[ 2 ],
					player_color,
					force_foreground
				);

				line(
					triangle_points[ 0 ],
					triangle_points[ 1 ],
					engine::math::from_rgb( 255, 255, 255 ),
					1.0f,
					force_foreground
				);
				line(
					triangle_points[ 1 ],
					triangle_points[ 2 ],
					engine::math::from_rgb( 255, 255, 255 ),
					1.0f,
					force_foreground
				);
				line(
					triangle_points[ 2 ],
					triangle_points[ 0 ],
					engine::math::from_rgb( 255, 255, 255 ),
					1.0f,
					force_foreground
				);
			}
		};
	}
}