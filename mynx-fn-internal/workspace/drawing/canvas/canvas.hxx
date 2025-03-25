namespace drawing {
	namespace canvas {
		class c_canvas : public c_drawing {
		public:
			bool setup( 
				engine::u_canvas* canvas
			) {
				if ( !canvas )
					return false;

				m_canvas = canvas;
				m_screen_size = engine::fvector2d(
					m_canvas->clip_x( ),
					m_canvas->clip_y( )
				);

				m_screen_center = engine::fvector2d(
					m_screen_size.m_x / 2.0,
					m_screen_size.m_y / 2.0
				);

				if ( !m_font ) {
					m_font = engine::u_object::find_object<engine::u_font*>(
						encrypt( L"Engine/EngineFonts/Roboto.Roboto" ),
						reinterpret_cast< engine::u_object* >( -1 )
					);
				}

				return m_font;
			}

			bool project(
				engine::fvector world_location,
				engine::fvector2d* screen_position
			) {
				if ( !m_canvas || !world_location || !screen_position )
					return false;

				auto result = m_canvas->k2_project( world_location );
				if ( result.m_z <= 0.0 ) {
					result.m_x *= -1.0;
					result.m_y *= -1.0;

					result.m_x += m_screen_size.m_x;
					result.m_y += m_screen_size.m_y;
				}

				if ( result.m_z > 0.0 )
					*screen_position = engine::fvector2d( result.m_x, result.m_y );
				return screen_position && in_screen( *screen_position );
			}

			bool deproject(
				engine::fvector camera_location,
				engine::frotator camera_rotation,
				engine::fvector2d screen_position,
				engine::fvector* world_location
			) {
				if ( !m_canvas || !screen_position )
					return false;

				*world_location = m_canvas->k2_deproject(
					screen_position,
					camera_location,
					camera_rotation.get_forward_vector( )
				);

				return world_location;
			}
		};
	}
}