namespace drawing {
	namespace visuals {
		class c_visuals : public menu::c_menu {
		public:
			void crosshair(
				float size,
				const engine::flinear_color& color,
				float thickness = 1.0f,
				bool force_foreground = false
			) {
				line(
					engine::fvector2d( m_screen_center.m_x, m_screen_center.m_y - size ),
					engine::fvector2d( m_screen_center.m_x, m_screen_center.m_y + size ),
					color,
					thickness,
					force_foreground
				);

				line(
					engine::fvector2d( m_screen_center.m_x - size, m_screen_center.m_y ),
					engine::fvector2d( m_screen_center.m_x + size, m_screen_center.m_y ),
					color,
					thickness,
					force_foreground
				);
			}


			void draw_cursor( ) {
				const float outer_radius = 12.0f;
				const float inner_radius = 5.0f;
				const int num_points = 5;
				const float rotation_offset = -1.5708f;

				engine::fvector2d points[ 2 * num_points ];

				for ( int i = 0; i < num_points; i++ ) {
					float angle = static_cast< float >( i ) * ( 2.0f * engine::math::pi / num_points ) + rotation_offset;
					float next_angle = angle + ( engine::math::pi / num_points );

					points[ 2 * i ] = engine::fvector2d(
						m_mouse_position.m_x + outer_radius * cos( angle ),
						m_mouse_position.m_y + outer_radius * sin( angle )
					);

					points[ 2 * i + 1 ] = engine::fvector2d(
						m_mouse_position.m_x + inner_radius * cos( next_angle ),
						m_mouse_position.m_y + inner_radius * sin( next_angle )
					);
				}

				const engine::flinear_color star_color = engine::math::from_rgb( 170, 85, 235 );
				for ( int i = 0; i < 2 * num_points; i++ ) {
					int next_index = ( i + 1 ) % ( 2 * num_points );
					line( points[ i ], points[ next_index ], star_color, 2.5f, true );
				}

				const float cross_size = 4.0f;
				line(
					engine::fvector2d( m_mouse_position.m_x - cross_size, m_mouse_position.m_y ),
					engine::fvector2d( m_mouse_position.m_x + cross_size, m_mouse_position.m_y ),
					star_color, 1.5f, true
				);
				line(
					engine::fvector2d( m_mouse_position.m_x, m_mouse_position.m_y - cross_size ),
					engine::fvector2d( m_mouse_position.m_x, m_mouse_position.m_y + cross_size ),
					star_color, 1.5f, true
				);
			}

			engine::fvector2d text_size( engine::fstring render_text ) {
				return m_canvas->k2_text_size(
					m_font,
					render_text,
					engine::fvector2d( 1.0, 1.0 )
				);
			}

			void get_top_position(
				engine::fvector2d& text_pos,
				const engine::fvector2d& min_pos,
				const engine::fvector2d& max_pos,
				const engine::fvector2d& size,
				float& text_offset_y,
				float base_spacing = 15.0f,
				float scale_factor = 1.0f
			) {
				float adjusted_spacing = base_spacing * ( scale_factor / 10.0f );

				text_pos = engine::fvector2d(
					min_pos.m_x + ( size.m_x / 2.0 ),
					min_pos.m_y - text_offset_y
				);
				text_offset_y += adjusted_spacing;
			}

			void get_bottom_position(
				engine::fvector2d& text_pos,
				const engine::fvector2d& min_pos,
				const engine::fvector2d& max_pos,
				const engine::fvector2d& size,
				float& text_offset_y,
				float base_spacing = 15.0f,
				float scale_factor = 1.0f
			) {
				float adjusted_spacing = base_spacing * ( scale_factor / 10.0f );

				text_pos = engine::fvector2d(
					min_pos.m_x + ( size.m_x / 2.0 ),
					max_pos.m_y + text_offset_y
				);

				text_offset_y += adjusted_spacing;
			}

			float get_text_scale(
				float distance,
				float min_scale = 8.0f,
				float max_scale = 10.0f
			) {
				auto scale = max_scale - ( distance * 0.007f );
				return std::clamp( scale, min_scale, max_scale );
			}
			
			void draw_texture(
				const wchar_t* texture_name,
				const engine::fvector2d& position,
				const engine::fvector2d& size,
				float opacity = 1.0f,
				bool force_foreground = true
			) {
				if ( !m_canvas || !texture_name ) return;

				auto texture = engine::u_object::find_object<engine::u_texture*>( texture_name );
				if ( !texture ) return;

				m_canvas->k2_draw_texture(
					texture,
					position,
					size,
					engine::fvector2d( 0.0f, 0.0f ),
					engine::fvector2d( 1.0f, 1.0f ),
					engine::flinear_color( 1.0f, 1.0f, 1.0f, opacity ),
					engine::e_blend_mode::translucent,
					0.0f,
					engine::fvector2d( 0.0f, 0.0f )
				);
			}

			void draw_3d_box(
				const engine::fvector& origin,
				const engine::fvector& extent,
				const engine::flinear_color& color,
				float thickness = 1.0f,
				bool force_foreground = false
			) {
				engine::fvector corners[ 8 ];
				corners[ 0 ] = engine::fvector( origin.m_x - extent.m_x, origin.m_y - extent.m_y, origin.m_z - extent.m_z );
				corners[ 1 ] = engine::fvector( origin.m_x + extent.m_x, origin.m_y - extent.m_y, origin.m_z - extent.m_z );
				corners[ 2 ] = engine::fvector( origin.m_x - extent.m_x, origin.m_y + extent.m_y, origin.m_z - extent.m_z );
				corners[ 3 ] = engine::fvector( origin.m_x + extent.m_x, origin.m_y + extent.m_y, origin.m_z - extent.m_z );
				corners[ 4 ] = engine::fvector( origin.m_x - extent.m_x, origin.m_y - extent.m_y, origin.m_z + extent.m_z );
				corners[ 5 ] = engine::fvector( origin.m_x + extent.m_x, origin.m_y - extent.m_y, origin.m_z + extent.m_z );
				corners[ 6 ] = engine::fvector( origin.m_x - extent.m_x, origin.m_y + extent.m_y, origin.m_z + extent.m_z );
				corners[ 7 ] = engine::fvector( origin.m_x + extent.m_x, origin.m_y + extent.m_y, origin.m_z + extent.m_z );

				engine::fvector2d screen_corners[ 8 ];
				for ( int i = 0; i < 8; i++ ) {
					if ( !project( corners[ i ], &screen_corners[ i ] ) ) {
						return;
					}
				}

				line( screen_corners[ 0 ], screen_corners[ 1 ], color, thickness, force_foreground );
				line( screen_corners[ 1 ], screen_corners[ 3 ], color, thickness, force_foreground );
				line( screen_corners[ 3 ], screen_corners[ 2 ], color, thickness, force_foreground );
				line( screen_corners[ 2 ], screen_corners[ 0 ], color, thickness, force_foreground );

				line( screen_corners[ 4 ], screen_corners[ 5 ], color, thickness, force_foreground );
				line( screen_corners[ 5 ], screen_corners[ 7 ], color, thickness, force_foreground );
				line( screen_corners[ 7 ], screen_corners[ 6 ], color, thickness, force_foreground );
				line( screen_corners[ 6 ], screen_corners[ 4 ], color, thickness, force_foreground );

				line( screen_corners[ 0 ], screen_corners[ 4 ], color, thickness, force_foreground );
				line( screen_corners[ 1 ], screen_corners[ 5 ], color, thickness, force_foreground );
				line( screen_corners[ 2 ], screen_corners[ 6 ], color, thickness, force_foreground );
				line( screen_corners[ 3 ], screen_corners[ 7 ], color, thickness, force_foreground );
			}

			void corner_box(
				const engine::fvector2d& min_pos,
				const engine::fvector2d& size,
				const engine::flinear_color& color,
				float corner_width = 0.25f,
				float corner_height = 0.25f,
				float thickness = 1.0f,
				bool force_foreground = false
			) {
				const float box_width = size.m_x;
				const float box_height = size.m_y;

				float corner_w = box_width * corner_width;
				float corner_h = box_height * corner_height;

				line(
					engine::fvector2d( min_pos.m_x, min_pos.m_y ),
					engine::fvector2d( min_pos.m_x, min_pos.m_y + corner_h ),
					color,
					thickness,
					force_foreground
				);
				line(
					engine::fvector2d( min_pos.m_x, min_pos.m_y ),
					engine::fvector2d( min_pos.m_x + corner_w, min_pos.m_y ),
					color,
					thickness,
					force_foreground
				);

				line(
					engine::fvector2d( min_pos.m_x + box_width - corner_w, min_pos.m_y ),
					engine::fvector2d( min_pos.m_x + box_width, min_pos.m_y ),
					color,
					thickness,
					force_foreground
				);
				line(
					engine::fvector2d( min_pos.m_x + box_width, min_pos.m_y ),
					engine::fvector2d( min_pos.m_x + box_width, min_pos.m_y + corner_h ),
					color,
					thickness,
					force_foreground
				);

				line(
					engine::fvector2d( min_pos.m_x, min_pos.m_y + box_height - corner_h ),
					engine::fvector2d( min_pos.m_x, min_pos.m_y + box_height ),
					color,
					thickness,
					force_foreground
				);
				line(
					engine::fvector2d( min_pos.m_x, min_pos.m_y + box_height ),
					engine::fvector2d( min_pos.m_x + corner_w, min_pos.m_y + box_height ),
					color,
					thickness,
					force_foreground
				);

				line(
					engine::fvector2d( min_pos.m_x + box_width - corner_w, min_pos.m_y + box_height ),
					engine::fvector2d( min_pos.m_x + box_width, min_pos.m_y + box_height ),
					color,
					thickness,
					force_foreground
				);
				line(
					engine::fvector2d( min_pos.m_x + box_width, min_pos.m_y + box_height - corner_h ),
					engine::fvector2d( min_pos.m_x + box_width, min_pos.m_y + box_height ),
					color,
					thickness,
					force_foreground
				);
			}

			void outlined_rect(
				const engine::fvector2d& min_pos,
				const engine::fvector2d& size,
				const engine::flinear_color& box_color,
				const engine::flinear_color& outline_color,
				float thickness = 1.0f,
				bool force_foreground = false
			) {
				rect(
					min_pos - engine::fvector2d( 1.0f, 1.0f ),
					size + engine::fvector2d( 2.0f, 2.0f ),
					outline_color,
					thickness,
					force_foreground
				);
				rect(
					min_pos + engine::fvector2d( 1.0f, 1.0f ),
					size - engine::fvector2d( 2.0f, 2.0f ),
					outline_color,
					thickness,
					force_foreground
				);
				rect(
					min_pos,
					size,
					box_color,
					thickness,
					force_foreground
				);
			}

			void outlined_corner_box(
				const engine::fvector2d& min_pos,
				const engine::fvector2d& size,
				const engine::flinear_color& box_color,
				const engine::flinear_color& outline_color,
				float corner_width = 0.25f,
				float corner_height = 0.25f,
				float thickness = 1.0f,
				bool force_foreground = false
			) {
				corner_box(
					min_pos - engine::fvector2d( 1.0f, 1.0f ),
					size + engine::fvector2d( 2.0f, 2.0f ),
					outline_color,
					corner_width,
					corner_height,
					thickness,
					force_foreground
				);
				corner_box(
					min_pos + engine::fvector2d( 1.0f, 1.0f ),
					size - engine::fvector2d( 2.0f, 2.0f ),
					outline_color,
					corner_width,
					corner_height,
					thickness,
					force_foreground
				);
				corner_box(
					min_pos,
					size,
					box_color,
					corner_width,
					corner_height,
					thickness,
					force_foreground
				);
			}

			void outlined_3d_box(
				const engine::fvector& origin,
				const engine::fvector& extent,
				const engine::flinear_color& box_color,
				const engine::flinear_color& outline_color,
				float thickness = 1.0f,
				bool force_foreground = false
			) {
				draw_3d_box(
					origin,
					extent + engine::fvector( 1.0f, 1.0f, 1.0f ),
					outline_color,
					thickness,
					force_foreground
				);
				draw_3d_box(
					origin,
					extent,
					box_color,
					thickness,
					force_foreground
				);
			}
		};
	}
}