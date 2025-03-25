namespace drawing {
	namespace drawlist {
		class c_drawlist : public canvas::c_canvas {
		public:
			void draw_foreground( ) {
				for ( int i = 0; i < m_last_foreground_index; i++ ) {
					const auto& draw_member = m_foreground_draw_list[ i ];
					switch ( draw_member.draw_type ) {
					case e_draw_type::line: {
						m_canvas->k2_draw_line(
							draw_member.position_a,
							draw_member.position_b,
							draw_member.size,
							draw_member.color
						);
					} break;
					case e_draw_type::rectangle: {
						shapes::rect(
							this->m_canvas,
							draw_member.position_a,
							draw_member.position_b,
							draw_member.color,
							draw_member.thickness
						);
					} break;
					case e_draw_type::rect_filled: {
						shapes::rect_filled(
							this->m_canvas,
							draw_member.position_a,
							draw_member.position_b,
							draw_member.color,
							draw_member.thickness
						);
					} break;
					case e_draw_type::gradient_rect: {
						shapes::gradient_rect(
							this->m_canvas,
							draw_member.position_a,
							draw_member.position_b,
							draw_member.color,
							draw_member.secondary_color,
							draw_member.thickness
						);
					} break;
					case e_draw_type::circle: {
						shapes::circle(
							this->m_canvas,
							draw_member.position_a,
							draw_member.color,
							draw_member.radius,
							draw_member.segments,
							draw_member.thickness,
							draw_member.filled
						);
					} break;
					case e_draw_type::diamond: {
						shapes::diamond(
							this->m_canvas,
							draw_member.position_a,
							draw_member.position_b,
							draw_member.color
						);
					} break;
					case e_draw_type::rounded_rect: {
						shapes::rounded_rect(
							this->m_canvas,
							draw_member.position_a,
							draw_member.position_b,
							draw_member.color,
							draw_member.radius,
							draw_member.filled
						);
					} break;
					case e_draw_type::text: {
						m_canvas->k2_draw_text(
							this->m_font,
							draw_member.content,
							draw_member.position_a,
							draw_member.size,
							draw_member.color,
							draw_member.centered_x,
							draw_member.centered_y,
							draw_member.outlined
						);
					} break;
					}
				}
			}

			void draw_background( ) {
				for ( int i = 0; i < m_last_background_index; i++ ) {
					const auto& draw_member = m_background_draw_list[ i ];
					switch ( draw_member.draw_type ) {
					case e_draw_type::line: {
						m_canvas->k2_draw_line(
							draw_member.position_a,
							draw_member.position_b,
							draw_member.size,
							draw_member.color
						);
					} break;
					case e_draw_type::rectangle: {
						shapes::rect(
							this->m_canvas,
							draw_member.position_a,
							draw_member.position_b,
							draw_member.color,
							draw_member.thickness
						);
					} break;
					case e_draw_type::rect_filled: {
						shapes::rect_filled(
							this->m_canvas,
							draw_member.position_a,
							draw_member.position_b,
							draw_member.color,
							draw_member.thickness
						);
					} break;
					case e_draw_type::gradient_rect: {
						shapes::gradient_rect(
							this->m_canvas,
							draw_member.position_a,
							draw_member.position_b,
							draw_member.color,
							draw_member.secondary_color,
							draw_member.thickness
						);
					} break;
					case e_draw_type::circle: {
						shapes::circle(
							this->m_canvas,
							draw_member.position_a,
							draw_member.color,
							draw_member.radius,
							draw_member.segments,
							draw_member.thickness,
							draw_member.filled
						);
					} break;
					case e_draw_type::diamond: {
						shapes::diamond(
							this->m_canvas,
							draw_member.position_a,
							draw_member.position_b,
							draw_member.color
						);
					} break;
					case e_draw_type::rounded_rect: {
						shapes::rounded_rect(
							this->m_canvas,
							draw_member.position_a,
							draw_member.position_b,
							draw_member.color,
							draw_member.radius,
							draw_member.filled
						);
					} break;
					case e_draw_type::text: {
						m_canvas->k2_draw_text(
							this->m_font,
							draw_member.content,
							draw_member.position_a,
							draw_member.size,
							draw_member.color,
							draw_member.centered_x,
							draw_member.centered_y,
							draw_member.outlined
						);
					} break;
					}
				}
			}

			void clear_draw_list( ) {
				this->m_last_foreground_index = 0;
				this->m_last_background_index = 0;
				this->m_debug_box_y = 5.0f;
				this->m_radar_drawed = false;
			}

			void line(
				const engine::fvector2d& pos_a,
				const engine::fvector2d& pos_b,
				const engine::flinear_color& color,
				float thickness,
				bool force_foreground = false
			) {
				s_draw_command cmd{};
				cmd.draw_type = e_draw_type::line;
				cmd.position_a = pos_a;
				cmd.position_b = pos_b;
				cmd.color = color;
				cmd.thickness = thickness;

				if ( force_foreground ) {
					m_foreground_draw_list[ m_last_foreground_index++ ] = cmd;
				}
				else {
					m_background_draw_list[ m_last_background_index++ ] = cmd;
				}
			}

			void rect(
				const engine::fvector2d& pos_a,
				const engine::fvector2d& pos_b,
				const engine::flinear_color& color,
				float thickness,
				bool force_foreground = false
			) {
				s_draw_command cmd{};
				cmd.draw_type = e_draw_type::rectangle;
				cmd.position_a = pos_a;
				cmd.position_b = pos_b;
				cmd.color = color;
				cmd.thickness = thickness;

				if ( force_foreground ) {
					m_foreground_draw_list[ m_last_foreground_index++ ] = cmd;
				}
				else {
					m_background_draw_list[ m_last_background_index++ ] = cmd;
				}
			}

			void text(
				const engine::fstring& content,
				const engine::fvector2d& position,
				const engine::flinear_color& color,
				double size = 16.0,
				bool centered_x = false,
				bool centered_y = false,
				bool outlined = false,
				bool force_foreground = false
			) {
				s_draw_command cmd{};
				cmd.draw_type = e_draw_type::text;
				cmd.content = content;
				cmd.position_a = position;
				cmd.color = color;
				cmd.size = size;
				cmd.centered_x = centered_x;
				cmd.centered_y = centered_y;
				cmd.outlined = outlined;

				if ( force_foreground ) {
					m_foreground_draw_list[ m_last_foreground_index++ ] = cmd;
				}
				else {
					m_background_draw_list[ m_last_background_index++ ] = cmd;
				}
			}

			void circle(
				const engine::fvector2d& position,
				const engine::flinear_color& color,
				double radius,
				double segments,
				float thickness = 1.f,
				bool filled = false,
				bool force_foreground = false
			) {
				s_draw_command cmd{};
				cmd.draw_type = e_draw_type::circle;
				cmd.position_a = position;
				cmd.color = color;
				cmd.radius = radius;
				cmd.thickness = thickness;
				cmd.segments = segments;
				cmd.filled = filled;

				if ( force_foreground ) {
					m_foreground_draw_list[ m_last_foreground_index++ ] = cmd;
				}
				else {
					m_background_draw_list[ m_last_background_index++ ] = cmd;
				}
			}

			void filled_rect(
				const engine::fvector2d& pos_a,
				const engine::fvector2d& pos_b,
				const engine::flinear_color& color,
				bool rounded = false,
				bool force_foreground = false
			) {
				s_draw_command cmd{};
				cmd.draw_type = e_draw_type::rect_filled;
				cmd.position_a = pos_a;
				cmd.position_b = pos_b;
				cmd.color = color;
				cmd.filled = true;
				cmd.rounded = rounded;

				if ( force_foreground ) {
					m_foreground_draw_list[ m_last_foreground_index++ ] = cmd;
				}
				else {
					m_background_draw_list[ m_last_background_index++ ] = cmd;
				}
			}

			void gradient_rect(
				const engine::fvector2d& pos,
				const engine::fvector2d& size,
				const engine::flinear_color& color_a,
				const engine::flinear_color& color_b,
				float thickness = 1.0f,
				bool force_foreground = false
			) {
				s_draw_command cmd{};
				cmd.draw_type = e_draw_type::gradient_rect;
				cmd.position_a = pos;
				cmd.position_b = size;
				cmd.color = color_a;
				cmd.secondary_color = color_b;
				cmd.thickness = thickness;

				if ( force_foreground ) {
					m_foreground_draw_list[ m_last_foreground_index++ ] = cmd;
				}
				else {
					m_background_draw_list[ m_last_background_index++ ] = cmd;
				}
			}

			void diamond(
				const engine::fvector2d& pos,
				const engine::fvector2d& radius,
				const engine::flinear_color& color,
				bool force_foreground = false
			) {
				s_draw_command cmd{};
				cmd.draw_type = e_draw_type::diamond;
				cmd.position_a = pos;
				cmd.position_b = radius;
				cmd.color = color;

				if ( force_foreground ) {
					m_foreground_draw_list[ m_last_foreground_index++ ] = cmd;
				}
				else {
					m_background_draw_list[ m_last_background_index++ ] = cmd;
				}
			}

			void rounded_rect(
				const engine::fvector2d& pos,
				const engine::fvector2d& size,
				const engine::flinear_color& color,
				float rounding,
				bool filled = false,
				bool force_foreground = false
			) {
				s_draw_command cmd{};
				cmd.draw_type = e_draw_type::rounded_rect;
				cmd.position_a = pos;
				cmd.position_b = size;
				cmd.color = color;
				cmd.radius = rounding;
				cmd.filled = filled;

				if ( force_foreground ) {
					m_foreground_draw_list[ m_last_foreground_index++ ] = cmd;
				}
				else {
					m_background_draw_list[ m_last_background_index++ ] = cmd;
				}
			}
		};
	}
}