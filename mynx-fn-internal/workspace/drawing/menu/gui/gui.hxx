namespace drawing {
	namespace gui {
		class c_gui : public drawlist::c_drawlist {
		public:
			bool is_hovered(
				engine::fvector2d position,
				engine::fvector2d size
			) {
				if ( m_mouse_position.m_x > position.m_x && m_mouse_position.m_y > position.m_y )
					if ( m_mouse_position.m_x < position.m_x + size.m_x && m_mouse_position.m_y < position.m_y + size.m_y )
						return true;
				return false;
			}

			bool is_point_in_triangle(
				const engine::fvector2d& point,
				const engine::fvector2d& v1,
				const engine::fvector2d& v2,
				const engine::fvector2d& v3
			) {
				float d1 = sign( point, v1, v2 );
				float d2 = sign( point, v2, v3 );
				float d3 = sign( point, v3, v1 );

				bool has_neg = ( d1 < 0 ) || ( d2 < 0 ) || ( d3 < 0 );
				bool has_pos = ( d1 > 0 ) || ( d2 > 0 ) || ( d3 > 0 );

				return !( has_neg && has_pos );
			}

			float sign(
				const engine::fvector2d& p1,
				const engine::fvector2d& p2,
				const engine::fvector2d& p3
			) {
				return ( p1.m_x - p3.m_x ) * ( p2.m_y - p3.m_y ) - ( p2.m_x - p3.m_x ) * ( p1.m_y - p3.m_y );
			}

			void filled_triangle(
				const engine::fvector2d& point1,
				const engine::fvector2d& point2,
				const engine::fvector2d& point3,
				const engine::flinear_color& color,
				bool force_foreground = true
			) {
				float min_x = min( min( point1.m_x, point2.m_x ), point3.m_x );
				float max_x = max( max( point1.m_x, point2.m_x ), point3.m_x );
				float min_y = min( min( point1.m_y, point2.m_y ), point3.m_y );
				float max_y = max( max( point1.m_y, point2.m_y ), point3.m_y );

				line( point1, point2, color, 1.0f, force_foreground );
				line( point2, point3, color, 1.0f, force_foreground );
				line( point3, point1, color, 1.0f, force_foreground );

				for ( float y = min_y; y <= max_y; y++ ) {
					for ( float x = min_x; x <= max_x; x++ ) {
						engine::fvector2d point( x, y );
						if ( is_point_in_triangle( point, point1, point2, point3 ) ) {
							line(
								engine::fvector2d( x, y ),
								engine::fvector2d( x + 1, y ),
								color,
								1.0f,
								force_foreground
							);
						}
					}
				}
			}

			engine::flinear_color get_hue_color( float hue ) {
				float h = hue * 6.0f;
				float x = 1.0f - std::abs( std::fmod( h, 2.0f ) - 1.0f );

				if ( h < 1 ) return engine::math::from_rgb( 255, x * 255, 0 );
				if ( h < 2 ) return engine::math::from_rgb( x * 255, 255, 0 );
				if ( h < 3 ) return engine::math::from_rgb( 0, 255, x * 255 );
				if ( h < 4 ) return engine::math::from_rgb( 0, x * 255, 255 );
				if ( h < 5 ) return engine::math::from_rgb( x * 255, 0, 255 );
				return engine::math::from_rgb( 255, 0, x * 255 );
			}

			engine::flinear_color hsv_to_rgb( float h, float s, float v ) {
				float c = v * s;
				float x = c * ( 1.0f - std::abs( std::fmod( h * 6.0f, 2.0f ) - 1.0f ) );
				float m = v - c;

				float r, g, b;
				if ( h < 1.0f / 6.0f ) { r = c; g = x; b = 0; }
				else if ( h < 2.0f / 6.0f ) { r = x; g = c; b = 0; }
				else if ( h < 3.0f / 6.0f ) { r = 0; g = c; b = x; }
				else if ( h < 4.0f / 6.0f ) { r = 0; g = x; b = c; }
				else if ( h < 5.0f / 6.0f ) { r = x; g = 0; b = c; }
				else { r = c; g = 0; b = x; }

				return engine::math::from_rgb(
					( r + m ) * 255,
					( g + m ) * 255,
					( b + m ) * 255
				);
			}

			engine::flinear_color get_color_by_tier( engine::e_fort_rarity tier ) {
				switch ( tier ) {
				case engine::e_fort_rarity::common:
					return engine::math::from_rgb( 185, 185, 185 );
				case engine::e_fort_rarity::uncommon:
					return engine::math::from_rgb( 96, 170, 58 );
				case engine::e_fort_rarity::rare:
					return engine::math::from_rgb( 75, 128, 255 );
				case engine::e_fort_rarity::epic:
					return engine::math::from_rgb( 209, 110, 244 );
				case engine::e_fort_rarity::legendary:
					return engine::math::from_rgb( 255, 189, 62 );
				case engine::e_fort_rarity::mythic:
					return engine::math::from_rgb( 255, 215, 0 );
				case engine::e_fort_rarity::transcendent:
					return engine::math::from_rgb( 255, 100, 100 );
				case engine::e_fort_rarity::unattainable:
					return engine::math::from_rgb( 255, 255, 255 );
				default:
					return engine::math::from_rgb( 185, 185, 185 );
				}
			}

			auto determine_colors( bool is_visible, bool is_teammate, bool knocked, bool bot ) {
				struct colors_t {
					engine::flinear_color box, filled, skeleton, text, kills, level, line, view, outline;
				};

				auto result = colors_t{
					is_visible ? features::visuals::box_color : features::visuals::invis_box_color,
					features::visuals::filled_color,
					is_visible ? features::visuals::skeleton_color : features::visuals::invis_skeleton_color,
					is_visible ? features::visuals::text_color : features::visuals::invis_text_color,
					is_visible ? features::visuals::kills_color : features::visuals::invis_text_color,
					is_visible ? features::visuals::level_color : features::visuals::invis_text_color,
					is_visible ? features::visuals::line_color : features::visuals::invis_line_color,
					is_visible ? features::visuals::view_color : features::visuals::invis_view_color,
					is_visible ? features::visuals::outline_color : features::visuals::invis_outline_color
				};

				result = knocked ? colors_t {
					features::visuals::knocked_color,
					features::visuals::knocked_color,
					features::visuals::knocked_color,
					features::visuals::knocked_color,
					features::visuals::knocked_color,
					features::visuals::knocked_color,
					features::visuals::knocked_color,
					features::visuals::knocked_color,
					features::visuals::outline_color
				} : result;

				result = bot ? colors_t{
					features::visuals::bot_color,
					features::visuals::bot_color,
					features::visuals::bot_color,
					features::visuals::bot_color,
					features::visuals::bot_color,
					features::visuals::bot_color,
					features::visuals::bot_color,
					features::visuals::bot_color,
					features::visuals::outline_color
				} : result;

				result = is_teammate ? colors_t{
					features::visuals::team_color,
					features::visuals::team_color,
					features::visuals::team_color,
					features::visuals::team_color,
					features::visuals::kills_color,
					features::visuals::level_color,
					features::visuals::team_color,
					features::visuals::team_color,
					features::visuals::outline_color,
				} : result;
				return result;
			}

			engine::flinear_color get_rank_color( int32_t rank ) {
				if ( rank >= 0 && rank <= 2 )
					return engine::math::from_rgb( 0xCD, 0x7F, 0x32 );
				else if ( rank >= 3 && rank <= 5 )
					return engine::math::from_rgb( 0xC0, 0xC0, 0xC0 );
				else if ( rank >= 6 && rank <= 8 )
					return engine::math::from_rgb( 0xFF, 0xD7, 0x00 );
				else if ( rank >= 9 && rank <= 11 )
					return engine::math::from_rgb( 0xE5, 0xE4, 0xE2 );
				else if ( rank >= 12 && rank <= 14 )
					return engine::math::from_rgb( 0xB9, 0xF2, 0xFF );
				else if ( rank == 15 )
					return engine::math::from_rgb( 0x71, 0x71, 0x71 );
				else if ( rank == 16 )
					return engine::math::from_rgb( 0xFF, 0x00, 0x00 );
				else if ( rank == 17 )
					return engine::math::from_rgb( 0xFF, 0x00, 0xFF );

				return engine::math::from_rgb( 0x80, 0x80, 0x80 );
			}

			const wchar_t* get_rank_name( int32_t rank ) {
				if ( rank >= 0 && rank <= 2 )
					return L"Bronze";
				else if ( rank >= 3 && rank <= 5 )
					return L"Silver";
				else if ( rank >= 6 && rank <= 8 )
					return L"Gold";
				else if ( rank >= 9 && rank <= 11 )
					return L"Platinum";
				else if ( rank >= 12 && rank <= 14 )
					return L"Diamond";
				else if ( rank == 15 )
					return L"Elite";
				else if ( rank == 16 )
					return L"Champion";
				else if ( rank == 17 )
					return L"Unreal";

				return L"";
			}
		};
	}
}