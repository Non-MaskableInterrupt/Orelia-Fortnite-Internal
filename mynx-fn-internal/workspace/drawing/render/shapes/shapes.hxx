namespace drawing {
	namespace shapes {
		void circle(
			engine::u_canvas* canvas,
			engine::fvector2d position,
			engine::flinear_color color,
			double radius,
			double segments,
			float thickness = 1.0f,
			bool filled = false
		) {
			if ( filled ) {
				for ( double r = 0; r <= radius; r += 1.0 ) {
					int count = 0;
					auto step = engine::math::pi * oxorany( 2.0 ) / segments;

					engine::fvector2d prev_point;
					bool first_point = true;

					for ( auto degree = 0.0; degree < engine::math::pi * oxorany( 2.0 ); degree += step ) {
						double x = r * engine::kismet::g_math_library->cos( degree ) + position.m_x;
						double y = r * engine::kismet::g_math_library->sin( degree ) + position.m_y;

						if ( !first_point ) {
							canvas->k2_draw_line( prev_point, engine::fvector2d( x, y ), 1.0f, color );
						}

						prev_point = engine::fvector2d( x, y );
						first_point = false;
					}
				}
			}
			else {
				int count = 0;
				auto step = engine::math::pi * oxorany( 2.0 ) / segments;

				engine::fvector2d vectors[ 128 ];
				for ( auto degree = 0.0; degree < engine::math::pi * oxorany( 2.0 ); degree += step ) {
					double x_1 = radius * engine::kismet::g_math_library->cos( degree ) + position.m_x;
					double y_1 = radius * engine::kismet::g_math_library->sin( degree ) + position.m_y;
					double x_2 = radius * engine::kismet::g_math_library->cos( degree + step ) + position.m_x;
					double y_2 = radius * engine::kismet::g_math_library->sin( degree + step ) + position.m_y;

					canvas->k2_draw_line(
						engine::fvector2d( x_1, y_1 ),
						engine::fvector2d( x_2, y_2 ),
						thickness,
						color
					);
				}
			}
		}

		void rect(
			engine::u_canvas* canvas,
			engine::fvector2d position,
			engine::fvector2d size,
			engine::flinear_color color,
			float thickness
		) {
			canvas->k2_draw_line( engine::fvector2d( position.m_x, position.m_y ), engine::fvector2d( position.m_x + size.m_x, position.m_y ), thickness, color );
			canvas->k2_draw_line( engine::fvector2d( position.m_x + size.m_x, position.m_y ), engine::fvector2d( position.m_x + size.m_x, position.m_y + size.m_y ), thickness, color );
			canvas->k2_draw_line( engine::fvector2d( position.m_x + size.m_x, position.m_y + size.m_y ), engine::fvector2d( position.m_x, position.m_y + size.m_y ), thickness, color );
			canvas->k2_draw_line( engine::fvector2d( position.m_x, position.m_y + size.m_y ), engine::fvector2d( position.m_x, position.m_y ), thickness, color );
		}

		void rect_filled(
			engine::u_canvas* canvas,
			engine::fvector2d position,
			engine::fvector2d size,
			engine::flinear_color color,
			float thickness
		) {
			auto default_texture = canvas->default_texture( );
			for ( float m_y = position.m_y; m_y < position.m_y + size.m_y; m_y++ ) {
				canvas->k2_draw_texture(
					default_texture,
					engine::fvector2d( position.m_x, m_y ),
					engine::fvector2d( size.m_x, 1.0 ),
					engine::fvector2d( ),
					engine::fvector2d( 1.0, 1.0 ),
					color,
					engine::e_blend_mode::translucent,
					0.0f,
					engine::fvector2d( )
				);
			}
		}

		void gradient_rect(
			engine::u_canvas* canvas,
			engine::fvector2d position,
			engine::fvector2d size,
			engine::flinear_color color_a,
			engine::flinear_color color_b,
			float thickness
		) {
			for ( auto i = position.m_y; i < position.m_y + size.m_y; i++ ) {
				float alpha = ( i - position.m_y ) / size.m_y;
				auto interpolated_color = engine::kismet::g_math_library->linear_color_lerp_using_hsv( color_a, color_b, alpha );
				canvas->k2_draw_line( engine::fvector2d( position.m_x, i ), engine::fvector2d( position.m_x + size.m_x, i ), thickness, interpolated_color );
			}
		}

		void diamond(
			engine::u_canvas* canvas,
			engine::fvector2d position,
			engine::fvector2d radius,
			engine::flinear_color color
		) {
			engine::fvector2d diamond_points[ 4 ];
			diamond_points[ 0 ] = { position.m_x, position.m_y - radius.m_y };
			diamond_points[ 1 ] = { position.m_x + radius.m_x, position.m_y };
			diamond_points[ 2 ] = { position.m_x, position.m_y + radius.m_y };
			diamond_points[ 3 ] = { position.m_x - radius.m_x, position.m_y };

			canvas->k2_draw_polygon( nullptr, diamond_points[ 0 ], radius, oxorany( 4 ), color );
		}

		void rounded_rect(
			engine::u_canvas* canvas,
			engine::fvector2d position,
			engine::fvector2d size,
			engine::flinear_color color,
			float rounding,
			bool fillled
		) {
			if ( fillled ) {
				rect_filled( canvas, engine::fvector2d( position.m_x + rounding, position.m_y ), engine::fvector2d( size.m_x - ( rounding * 2.0 ), size.m_y ), color, 1.f );
				rect_filled( canvas, engine::fvector2d( position.m_x, position.m_y + rounding ), engine::fvector2d( size.m_x, size.m_y - ( rounding * 2.0 ) ), color, 1.f );
			}

			diamond( canvas, engine::fvector2d( position.m_x + rounding - 2.0, position.m_y + rounding + 3.0 ), engine::fvector2d( rounding - 1.0, rounding - 2.0 ), color );
			diamond( canvas, engine::fvector2d( position.m_x + size.m_x - rounding + 1.0, position.m_y + rounding + 3.0 ), engine::fvector2d( rounding - 1.0, rounding - 2.0 ), color );
			diamond( canvas, engine::fvector2d( position.m_x + rounding - 2.0, position.m_y + size.m_y - rounding + 5.0 ), engine::fvector2d( rounding - 1.0, rounding - 2.0 ), color );
			diamond( canvas, engine::fvector2d( position.m_x + size.m_x - rounding + 1.0, position.m_y + size.m_y - rounding + 5.0 ), engine::fvector2d( rounding - 1.0, rounding - 2.0 ), color );
		}
	}
}