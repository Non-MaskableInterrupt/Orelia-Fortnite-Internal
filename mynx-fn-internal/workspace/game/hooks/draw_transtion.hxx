#pragma once

namespace draw_transition {
	void( *draw_transition_original )( engine::u_game_viewport_client*, engine::u_canvas* ) = nullptr;
	void draw_transition( engine::u_game_viewport_client* viewport_client, engine::u_canvas* canvas ) {
		const auto radar_size = features::visuals::radar_size;
		const engine::fvector2d radar_pos( 30.0f, 50.0f );
		if ( !viewport_client || !canvas )
			return draw_transition_original( viewport_client, canvas );

		auto world = viewport_client->world( );
		if ( !world ) return draw_transition_original( viewport_client, canvas );

		auto game_state = world->game_state( );
		if ( !game_state ) return draw_transition_original( viewport_client, canvas );

		auto game_instance = world->game_instance( );
		if ( !game_instance ) return draw_transition_original( viewport_client, canvas );

		auto local_player = game_instance->get_localplayer( );
		if ( !local_player ) return draw_transition_original( viewport_client, canvas );

		auto view_state = local_player->get_view_state( );
		if ( !view_state ) return draw_transition_original( viewport_client, canvas );
		auto camera_info = world->queue_camera_info( view_state );

		auto player_controller = local_player->player_controller( );
		if ( !player_controller ) return draw_transition_original( viewport_client, canvas );
		auto acknowledged_pawn = player_controller->acknowledged_pawn( );

		static drawing::c_framework drawing;
		if ( !drawing.setup( canvas ) )
			return draw_transition_original( viewport_client, canvas );

		static exploits::c_desync desync;
		desync.apply( acknowledged_pawn );
		desync.update_fly_controls(
			player_controller,
			acknowledged_pawn
		);
		desync.handle_movement_validation( acknowledged_pawn );
		desync.force_network_update( acknowledged_pawn );

		static targeting::c_targeting targeting;
		targeting.setup(
			world, player_controller,
			drawing, camera_info
		);

		static console::c_console console;
		console.apply_world_rendering( viewport_client );

		auto screen_size = drawing.get_screen_size( );
		auto screen_center = drawing.get_screen_center( );
		auto aimbot_fov = camera_info.get_aimbot_fov(
			features::aimbot::fov_radius,
			screen_center.m_x
		);

		if ( features::visuals::radar ) {
			drawing.draw_radar( radar_pos, radar_size );

			if ( acknowledged_pawn ) {
				drawing.add_local_player_to_radar(
					radar_pos,
					radar_size,
					aimbot_fov,
					camera_info.get_rotation( ),
					true
				);
			}
		}

		static exploits::c_weapon weapon;
		if ( features::exploits::enabled ) {
			if ( acknowledged_pawn ) {
				auto current_weapon = acknowledged_pawn->current_weapon( );
				if ( current_weapon ) {
					weapon.apply_hit_sound( current_weapon );
					weapon.apply_rapid_fire( current_weapon );
				}
			}
		}

		static exploits::c_chams chams;
		if ( features::world::enabled ) {
			if ( features::world::loot_esp ) {
				auto pickup_array = world->get_actors_of_class<engine::a_fort_pickup*>(
					engine::classes::g_fort_pickup
				);
				for ( int i = 0; i < pickup_array.size( ); i++ ) {
					auto pickup = pickup_array[ i ];
					if ( !pickup ) continue;

					engine::fvector2d actor_position;
					auto actor_location = pickup->k2_get_actor_location( );
					if ( !drawing.project( actor_location, &actor_position ) ) continue;

					auto distance = actor_location.distance_to( camera_info.get_location( ) );
					auto text_size = drawing.get_text_scale( distance, 7.f, 8.f );
					if ( distance > features::world::loot_distance ) continue;

					auto item_entry = pickup->primary_pickup_item_entry( );
					auto item_definition = item_entry.item_definition;
					if ( !item_definition ) continue;

					auto rarity = item_definition->rarity( );
					auto color = drawing.get_color_by_tier( rarity );
					if ( features::world::loot_tier != 0 &&
						static_cast< int >( rarity ) < features::world::loot_tier )
						continue;

					auto display_name = item_definition->item_name( ).get( );
					if ( !display_name ) continue;

					auto built_string = engine::fstring( std::format( L"{:.1f}0", distance ).c_str( ) );
					built_string = engine::kismet::g_string_library->concat_str_str(
						engine::kismet::g_string_library->concat_str_str(
						engine::kismet::g_string_library->concat_str_str(
						display_name.c_str( ),
						encrypt( L" [" )
					),
						built_string
					),
						encrypt( L"m]" )
					);

					drawing.text(
						built_string,
						actor_position,
						color,
						text_size,
						true,
						true,
						true
					);

					if ( features::world::loot_icons ) {
						auto small_preview_image = item_definition->get_small_preview_image( );
						auto assest_path_string = engine::kismet::g_string_library->conv_name_to_string(
							small_preview_image.m_object_id.m_asset_path_name
						);

						auto texture = world->load_object<engine::u_texture>(
							assest_path_string.c_str( ),
							player_controller
						);

						auto size = engine::fvector2d( 25.0, 25.0 );
						engine::fvector2d icon_pos = {
							actor_position.m_x - ( size.m_x / 2.0f ),
							actor_position.m_y + ( text_size * 2 ) + 7.0f
						};

						canvas->k2_draw_texture(
							texture,
							icon_pos,
							size,
							engine::fvector2d( 0.0, 0.0 ),
							engine::fvector2d( 1.0, 1.0 ),
							engine::flinear_color( 1.f, 1.f, 1.f, 1.f ),
							engine::e_blend_mode::translucent,
							0.0f,
							engine::fvector2d( 0.0, 0.0 )
						);
					}
				}
			}

			if ( features::world::chest_esp ) {
				auto container_array = world->get_actors_of_class<engine::a_building_container*>(
					engine::classes::g_container
				);
				for ( int i = 0; i < container_array.size( ); i++ ) {
					auto container = container_array[ i ];
					if ( !container ) continue;
					if ( container->b_already_searched( ) ) continue;

					engine::fvector2d actor_position;
					auto actor_location = container->k2_get_actor_location( );
					if ( !drawing.project( actor_location, &actor_position ) ) continue;

					auto distance = actor_location.distance_to( camera_info.get_location( ) );
					auto text_size = drawing.get_text_scale( distance, 7.f, 8.f );
					if ( distance > features::world::chest_distance ) continue;

					if ( features::world::chest_chams ) {
						auto static_mesh_component = container->static_mesh_component( );
						if ( static_mesh_component )
							chams.apply_stencil( static_mesh_component );
					}

					engine::flinear_color color;
					engine::fstring display_name;

					auto object_name = container->get_object_name( );
					if ( engine::kismet::g_string_library->contains( object_name, encrypt( L"Tiered_Chest" ), true, false ) ) {
						display_name = encrypt( L"Chest [" );
						color = engine::math::from_rgb( 235, 101, 41 );
					}
					else if ( engine::kismet::g_string_library->contains( object_name, encrypt( L"Tiered_Safe" ), true, false ) ) {
						display_name = encrypt( L"Safe [" );
						color = engine::math::from_rgb( 97, 98, 134 );
					}
					else if ( engine::kismet::g_string_library->contains( object_name, encrypt( L"Tiered_Ammo" ), true, false ) ) {
						display_name = encrypt( L"Ammobox [" );
						color = engine::math::from_rgb( 38, 119, 40 );
					}

					if ( display_name ) {
						auto distance_text = engine::fstring( std::format(
							L"{:.1f}0",
							distance ).c_str( )
						);

						display_name = engine::kismet::g_string_library->concat_str_str(
							engine::kismet::g_string_library->concat_str_str(
							display_name,
							distance_text ),
							encrypt( L"m]" )
						);

						drawing.text(
							display_name,
							actor_position,
							color,
							text_size,
							true,
							true,
							true
						);
					}
				}
			}

			if ( features::world::weakspot_esp ) {
				double closest_distance = DBL_MAX;
				engine::a_actor* target_actor = nullptr;
				auto weakspot_array = world->get_actors_of_class<engine::a_building_weakspot*>(
					engine::classes::g_weakspot
				);
				for ( int i = 0; i < weakspot_array.size( ); i++ ) {
					auto weakspot = weakspot_array[ i ];
					if ( !weakspot ) continue;
					if ( weakspot->b_hit( ) || !weakspot->b_active( ) ) continue;

					engine::fvector2d actor_position;
					auto actor_location = weakspot->k2_get_actor_location( );
					if ( !drawing.project( actor_location, &actor_position ) ) continue;

					auto distance = actor_location.distance_to( camera_info.get_location( ) );
					auto text_size = drawing.get_text_scale( distance, 7.f, 8.f );
					if ( distance > features::world::weakspot_distance ) continue;

					drawing.text(
						encrypt( L"WS" ),
						actor_position,
						features::world::weakspot_color,
						text_size,
						true,
						true,
						true
					);

					auto center_distance = actor_position.distance_to( drawing.get_screen_center( ) );
					if ( center_distance < closest_distance ) {
						closest_distance = center_distance;
						target_actor = weakspot;
					}
				}

				if ( features::world::weakspot_aimbot ) {
					if ( target_actor ) {
						auto aim_location = target_actor->k2_get_actor_location( );
						engine::fvector2d aim_position;
						if ( drawing.project( aim_location, &aim_position ) ) {
							if ( features::aimbot::target_line ) {
								drawing.line(
									aim_position,
									drawing.get_screen_center( ),
									features::aimbot::line_color,
									1.f
								);
							}

							if ( targeting.is_holding_aim_key( ) ) {
								auto smoothed_location = targeting.apply_smoothing( aim_location, aim_position );
								//auto aim_rotation = targeting.get_aim_rotation( smoothed_location );
								//targeting.move_mouse( aim_rotation );
							}
						}
					}
				}
			}

			if ( features::world::vehicle_esp ) {
				auto vehicle_array = world->get_actors_of_class < engine::a_fort_athena_vehicle* >(
					engine::classes::g_vehicle
				);
				for ( int i = 0; i < vehicle_array.size( ); i++ ) {
					auto vehicle = vehicle_array[ i ];
					if ( !vehicle ) continue;

					if ( !vehicle->can_contain_players( ) ) continue;
					if ( vehicle->has_driver( ) ) continue;

					engine::fvector2d actor_position;
					auto actor_location = vehicle->k2_get_actor_location( );
					if ( !drawing.project( actor_location, &actor_position ) ) continue;

					auto distance = actor_location.distance_to( camera_info.get_location( ) );
					auto text_size = drawing.get_text_scale( distance, 7.f, 8.f );
					if ( distance > features::world::chest_distance ) continue;

					if ( features::world::vehicle_chams )
						vehicle->set_vehicle_stencil_highlighted( vehicle, true, 11 );

					auto display_name = engine::fstring( std::format( L"{:.1f}0", distance ).c_str( ) );
					display_name = engine::kismet::g_string_library->concat_str_str(
						engine::kismet::g_string_library->concat_str_str(
						encrypt( L"Vehicle [" ),
						display_name
					),
						encrypt( L"m]" )
					);

					drawing.text(
						display_name,
						actor_position,
						features::world::vehicle_color,
						text_size,
						true,
						true,
						true
					);

					if ( features::world::vehicle_health ) {
						display_name = engine::kismet::g_string_library->build_string_int( encrypt( L"" ), encrypt( L" [" ), vehicle->get_health( ), encrypt( L"/" ) );
						display_name = engine::kismet::g_string_library->build_string_int( display_name, encrypt( L"" ), vehicle->get_max_health( ), encrypt( L"HP]" ) );
						drawing.text(
							display_name,
							engine::fvector2d( actor_position.m_x, actor_position.m_y + 18.f * ( text_size / 10.0f ) ),
							features::world::health_color,
							text_size,
							true,
							true,
							true
						);
					}
				}
			}
		}

		auto player_array = game_state->player_array( );
		for ( auto i = 0; i < player_array.size( ); i++ ) {
			auto player_state = player_array[ i ];
			if ( !player_state ) continue;
			auto is_bot = player_state->is_a_bot( );

			auto pawn_private = player_state->pawn_private( );
			if ( !pawn_private ) continue;
			chams.apply( pawn_private );

			auto controller = engine::kismet::g_fort_library->get_controller_from_actor( pawn_private );
			if ( !controller ) continue;

			auto current_weapon = pawn_private->current_weapon( );
			if ( current_weapon )
				chams.apply( current_weapon );

			if ( pawn_private == acknowledged_pawn ) continue;
			if ( pawn_private->is_dead( ) ) continue;
			auto is_dbno = pawn_private->is_dbno( );

			auto mesh = pawn_private->mesh( );
			if ( !mesh ) continue;

			auto is_visible = targeting.is_visible( pawn_private );
			auto is_teammate = engine::kismet::g_fort_library->on_same_team(
				pawn_private,
				acknowledged_pawn
			);

			engine::fvector2d head_position;
			auto head_location = mesh->get_bone_location( encrypt( L"head" ) );
			drawing.project( head_location, &head_position );

			if ( !is_teammate )
				targeting.process_actor(
					drawing,
					pawn_private,
					head_position
				);

			const auto colors = drawing.determine_colors( is_visible, is_teammate, is_dbno, is_bot );
			const auto& [
				box_color,
				filled_color,
				skeleton_color,
				text_color,
				kills_color,
				level_color,
				line_color,
				view_color,
				outline_color
			] = colors;

			if ( !features::visuals::enabled ) continue;
			if ( features::visuals::fov_arrows && targeting.is_weapon_valid( ) ) {
				if ( !drawing.in_circle( aimbot_fov, head_position ) ) {
					auto delta_seconds = engine::kismet::g_gameplay_statics->get_world_delta_seconds( world );
					auto angle = engine::kismet::g_math_library->atan2(
						head_position.m_y - screen_center.m_y,
						head_position.m_x - screen_center.m_x
					);

					engine::fvector2d arrow_position{
						screen_center.m_x + ( aimbot_fov + 12.0 ) * engine::kismet::g_math_library->cos( angle ),
						screen_center.m_y + ( aimbot_fov + 12.0 ) * engine::kismet::g_math_library->sin( angle )
					};

					auto rotation_angle = angle - delta_seconds;
					engine::fvector2d arrow_vertex[ 3 ]{
						engine::fvector2d(
							arrow_position.m_x + engine::kismet::g_math_library->cos( rotation_angle ) * 15.0,
							arrow_position.m_y + engine::kismet::g_math_library->sin( rotation_angle ) * 15.0
						),
						engine::fvector2d(
							arrow_position.m_x + engine::kismet::g_math_library->cos( rotation_angle - 1.5 ) * 12.0,
							arrow_position.m_y + engine::kismet::g_math_library->sin( rotation_angle - 1.5 ) * 12.0
						),
						engine::fvector2d(
							arrow_position.m_x + engine::kismet::g_math_library->cos( rotation_angle + 1.5 ) * 12.0,
							arrow_position.m_y + engine::kismet::g_math_library->sin( rotation_angle + 1.5 ) * 12.0
						),
					};

					for ( float t = 0; t <= 1.0f; t += 0.1f ) {
						engine::fvector2d left = {
							arrow_vertex[ 0 ].m_x + ( arrow_vertex[ 1 ].m_x - arrow_vertex[ 0 ].m_x ) * t,
							arrow_vertex[ 0 ].m_y + ( arrow_vertex[ 1 ].m_y - arrow_vertex[ 0 ].m_y ) * t
						};

						engine::fvector2d right = {
							arrow_vertex[ 0 ].m_x + ( arrow_vertex[ 2 ].m_x - arrow_vertex[ 0 ].m_x ) * t,
							arrow_vertex[ 0 ].m_y + ( arrow_vertex[ 2 ].m_y - arrow_vertex[ 0 ].m_y ) * t
						};

						drawing.line( left, right, box_color, 1.5f );
					}

					drawing.line( arrow_vertex[ 0 ], arrow_vertex[ 1 ], engine::math::from_rgb( 0, 0, 0 ), 2.5f );
					drawing.line( arrow_vertex[ 1 ], arrow_vertex[ 2 ], engine::math::from_rgb( 0, 0, 0 ), 2.5f );
					drawing.line( arrow_vertex[ 2 ], arrow_vertex[ 0 ], engine::math::from_rgb( 0, 0, 0 ), 2.5f );
				}
			}

			auto root_location = mesh->get_bone_location( encrypt( L"root" ) );
			if ( features::visuals::radar && acknowledged_pawn ) {
				auto local_mesh = acknowledged_pawn->mesh( );
				if ( local_mesh ) {
					drawing.add_player_to_radar(
						local_mesh->get_bone_location( encrypt( L"root" ) ),
						camera_info.get_rotation( ),
						root_location,
						engine::fvector2d( 30.0f, 50.0f ),
						features::visuals::radar_size,
						box_color
					);
				}
			}

			engine::fvector2d root_position;
			drawing.project( root_location, &root_position );
			if ( !drawing.in_screen( root_position ) ||
				!drawing.in_screen( head_position ) )
				continue;

			auto distance = root_location.distance_to( camera_info.get_location( ) );
			auto text_scale = drawing.get_text_scale( distance );
			auto info_scale = drawing.get_text_scale( distance, 6.f, 8.5f );

			auto origin = engine::fvector( );
			auto extent = engine::fvector( );
			auto sphere_radius = 0.f;
			engine::kismet::g_system_library->get_component_bounds( mesh, &origin, &extent, &sphere_radius );

			engine::fvector2d min_pos( DBL_MAX, DBL_MAX );
			engine::fvector2d max_pos( DBL_MIN, DBL_MIN );
			for ( int i = 0; i < 8; i++ ) {
				auto corner = engine::fvector(
					( i & 1 ) ? extent.m_x : -extent.m_x,
					( i & 2 ) ? extent.m_y : -extent.m_y,
					( i & 4 ) ? extent.m_z : -extent.m_z
				);

				engine::fvector2d origin_2d;
				if ( !drawing.project( origin + corner, &origin_2d ) ) continue;

				min_pos.m_x = min( min_pos.m_x, origin_2d.m_x );
				min_pos.m_y = min( min_pos.m_y, origin_2d.m_y );
				max_pos.m_x = max( max_pos.m_x, origin_2d.m_x );
				max_pos.m_y = max( max_pos.m_y, origin_2d.m_y );
			}

			engine::fvector2d size;
			size.m_x = max_pos.m_x - min_pos.m_x;
			size.m_y = max_pos.m_y - min_pos.m_y;

			float top_offset = 14.0f;
			float bottom_offset = 11.0f;
			const float text_spacing = 14.5f;

			if ( features::visuals::box_esp ) {
				switch ( features::visuals::box_type ) {
				case 0:
					if ( features::visuals::outline )
						drawing.outlined_rect( min_pos, size, box_color, outline_color );
					else
						drawing.rect( min_pos, size, box_color, 1.f );
					break;

				case 1:
					if ( features::visuals::outline )
						drawing.outlined_corner_box( min_pos, size, box_color, outline_color, 0.25f, 0.35f );
					else
						drawing.corner_box( min_pos, size, box_color, 0.25f, 0.35f );
					break;

				case 2:
					if ( features::visuals::outline )
						drawing.outlined_3d_box( origin, extent, box_color, outline_color );
					else
						drawing.draw_3d_box( origin, extent, box_color );
					break;
				}
			}

			if ( features::visuals::filled_box ) {
				drawing.filled_rect(
					min_pos,
					size,
					filled_color.with_alpha( 0.20f )
				);
			}

			if ( features::visuals::snaplines ) {
				static const std::vector<engine::fvector2d> end_positions = {
					{ screen_center.m_x , 0.0 },
					screen_center,
					{ screen_center.m_x , ( screen_center.m_y * 2.0 ) + 10.0 },
				};

				static const std::vector<engine::fvector2d> start_positions = {
					head_position,
					head_position,
					root_position
				};

				drawing.line(
					start_positions[ features::visuals::line_type ],
					end_positions[ features::visuals::line_type ],
					skeleton_color,
					2.f
				);
			}

			if ( features::visuals::skeleton ) {
				for ( const auto& pair : bones::skeleton_pairs ) {
					engine::fvector2d first_pos;
					auto first_loc = mesh->get_bone_location( pair.first );
					if ( !drawing.project( first_loc, &first_pos ) ) continue;

					engine::fvector2d second_pos;
					auto second_loc = mesh->get_bone_location( pair.second );
					if ( !drawing.project( second_loc, &second_pos ) ) continue;

					drawing.line(
						first_pos,
						second_pos,
						skeleton_color,
						2.5f
					);
				}
			}

			if ( features::visuals::view_direction ) {
				auto root_component = pawn_private->root_component( );
				if ( root_component ) {
					auto direction = root_component->get_forward_vector( );

					auto view_distance = static_cast< double >( features::visuals::view_distance );
					auto view_location = head_location + ( direction * view_distance );

					engine::fvector2d view_position;
					if ( drawing.project( view_location, &view_position ) )
						drawing.line(
							head_position,
							view_position,
							view_color,
							1.f
						);
				}
			}

			if ( features::visuals::rank_esp ) {
				auto habanero_component = player_state->habanero_component( );
				if ( habanero_component ) {
					auto rank_progress = habanero_component->ranked_progress( );

					auto rank_color = drawing.get_rank_color( rank_progress.m_rank );
					auto rank_name = drawing.get_rank_name( rank_progress.m_rank );
					if ( rank_name ) {
						engine::fvector2d text_pos;
						drawing.get_top_position( text_pos, min_pos, max_pos, size, top_offset, text_spacing, text_scale );
						drawing.text(
							rank_name,
							text_pos,
							rank_color,
							text_scale,
							true,
							true,
							true
						);
					}
				}
			}

			if ( features::visuals::name ) {
				engine::fvector2d text_pos;
				drawing.get_top_position( text_pos, min_pos, max_pos, size, top_offset, text_spacing, text_scale );

				auto display_name = engine::kismet::g_fort_library->get_human_readable_name( pawn_private );
				display_name = is_bot ? encrypt( L"Bot" ) : display_name;
				display_name = engine::kismet::g_string_library->build_string_int(
					display_name,
					encrypt( L" [" ),
					player_state->player_id( ),
					encrypt( L"]" )
				);

				drawing.text(
					display_name,
					text_pos,
					text_color,
					text_scale,
					true,
					true,
					true
				);
			}

			if ( features::visuals::distance ) {
				engine::fvector2d text_pos;
				drawing.get_top_position( text_pos, min_pos, max_pos, size, top_offset, text_spacing, text_scale );

				auto distance_text = engine::fstring( std::format( L"{:.1f}0", distance ).c_str( ) );
				distance_text = engine::kismet::g_string_library->concat_str_str(
					distance_text,
					encrypt( L"m" )
				);

				drawing.text(
					distance_text,
					text_pos,
					text_color,
					text_scale,
					true,
					true,
					true
				);
			}

			if ( features::visuals::ammo_bar && current_weapon ) {
				if ( current_weapon->object_is_a( engine::classes::g_fort_weapon ) ) {
					const auto bar_height = 4.0f;
					const auto bar_width = size.m_x;
					const auto bar_y = max_pos.m_y + 2.0f;

					auto ammo = current_weapon->get_magazine_ammo_count( );
					auto max_ammo = current_weapon->get_bullets_per_clip( );
					auto ammo_percentage = static_cast< float >( ammo ) / static_cast< float >( max_ammo );

					drawing.filled_rect(
						engine::fvector2d( min_pos.m_x, bar_y ),
						engine::fvector2d( bar_width, bar_height ),
						engine::math::from_rgb( 41, 41, 41 ),
						false
					);

					auto fill_width = bar_width * ammo_percentage;
					drawing.filled_rect(
						engine::fvector2d( min_pos.m_x, bar_y ),
						engine::fvector2d( fill_width, bar_height ),
						box_color,
						false
					);

					drawing.rect(
						engine::fvector2d( min_pos.m_x, bar_y ),
						engine::fvector2d( bar_width, bar_height ),
						engine::math::from_rgb( 48, 48, 48 ),
						1.0f
					);

					bottom_offset += bar_height * ( text_scale / 10.0f );
				}
			}

			if ( features::visuals::weapon && current_weapon ) {
				if ( current_weapon->object_is_a( engine::classes::g_fort_weapon ) ) {
					auto weapon_data = current_weapon->weapon_data( );
					if ( weapon_data ) {
						auto display_name = weapon_data->item_name( ).get( );
						if ( display_name ) {
							auto rarity = weapon_data->rarity( );
							auto color = drawing.get_color_by_tier( rarity );

							auto ammo = current_weapon->get_magazine_ammo_count( );
							auto max_ammo = current_weapon->get_bullets_per_clip( );
							if ( ammo && max_ammo ) {
								display_name = engine::kismet::g_string_library->build_string_int(
									display_name,
									encrypt( L" [" ),
									ammo,
									encrypt( L"/" )
								);

								display_name = engine::kismet::g_string_library->build_string_int(
									display_name,
									encrypt( L"" ),
									max_ammo,
									encrypt( L"]" )
								);
							}

							engine::fvector2d text_pos;
							drawing.get_bottom_position( text_pos, min_pos, max_pos, size, bottom_offset, text_scale );
							drawing.text(
								display_name,
								text_pos,
								color,
								text_scale,
								true,
								true,
								true
							);

							bottom_offset += text_spacing * ( text_scale / 10.0f );
						}
					}
				}
			}

			if ( features::visuals::platform ) {
				auto platform_name = player_state->get_platform( );
				if ( platform_name ) {
					engine::fvector2d text_pos;
					drawing.get_bottom_position( text_pos, min_pos, max_pos, size, bottom_offset, text_spacing, text_scale );
					drawing.text(
						platform_name,
						text_pos,
						text_color,
						text_scale,
						true,
						true,
						true
					);
				}
			}

			if ( features::visuals::kills ) {
				engine::fvector2d text_pos;
				text_pos.m_y = min_pos.m_y + 5.0f;
				text_pos.m_x = max_pos.m_x + 5.0f;

				auto kill_score = acknowledged_pawn ? player_state->kill_score( ) : 0;
				auto built_string = engine::kismet::g_string_library->build_string_int(
					encrypt( L"kills" ),
					encrypt( L": " ),
					kill_score,
					encrypt( L"" )
				);

				drawing.text(
					built_string,
					text_pos,
					kills_color,
					info_scale,
					false,
					true,
					true
				);
			}

			if ( features::visuals::season_level ) {
				engine::fvector2d text_pos;
				text_pos.m_y = min_pos.m_y + 20.0f;
				text_pos.m_x = max_pos.m_x + 5.0f;

				auto season_level = acknowledged_pawn ? player_state->season_level_ui_display( ) : 0;
				auto built_string = engine::kismet::g_string_library->build_string_int(
					encrypt( L"level" ),
					encrypt( L": " ),
					season_level,
					encrypt( L"" )
				);

				drawing.text(
					built_string,
					text_pos,
					level_color,
					info_scale,
					false,
					true,
					true
				);
			}
		}

		if ( features::misc::game_information && acknowledged_pawn ) {
			auto player_state = acknowledged_pawn->player_state( );
			if ( player_state ) {
				if ( features::visuals::specator_esp ) {
					std::vector<std::pair<engine::fstring, int>> spectator_information;
					auto game_state_athena = engine::kismet::g_fort_library->get_game_state_athena( world );
					if ( game_state_athena ) {
						auto spectator_array = game_state_athena->spectator_array( );
						for ( int i = 0; i < spectator_array.size( ); i++ ) {
							auto spectator = spectator_array[ i ];
							if ( !spectator ) continue;

							spectator_information.push_back( {
								encrypt( L"player id" ),
								spectator->player_id( )
								} );
						}
					}

					if ( !spectator_information.empty( ) ) {
						auto built_string = engine::kismet::g_string_library->build_string_int(
							encrypt( L"specators" ),
							encrypt( L" [" ),
							player_state->get_spectator_count( ),
							encrypt( L"]" )
						);
						drawing.debug_box( built_string.c_str( ), spectator_information );
					}
				}

				if ( features::visuals::inventory_esp ) {
					std::vector<std::tuple<engine::fstring, int, engine::flinear_color>> inventory_information;
					auto client_quick_bars = player_controller->client_quick_bars( );
					if ( client_quick_bars ) {
						auto primary_quick_bar = client_quick_bars->primary_quick_bar( );
						auto elements = primary_quick_bar.equipped_item_definitions.m_elements;
						for ( int i = 0; i < elements.size( ); i++ ) {
							auto& element = elements[ i ];
							auto item_definition = element.m_value;
							if ( !item_definition ) continue;

							auto name = item_definition->item_name( );
							auto display_name = name.get( );
							if ( !display_name ) continue;

							auto rarity = item_definition->rarity( );
							auto color = drawing.get_color_by_tier( rarity );

							inventory_information.push_back( {
								display_name.c_str( ),
								static_cast< int >( item_definition->get_max_num_stacks( ) ),
								color
								} );
						}
					}

					if ( !inventory_information.empty( ) )
						drawing.debug_box( encrypt( L"inventory items" ), inventory_information );
				}
			}
		}

		if ( features::aimbot::enabled && acknowledged_pawn ) {
			targeting.handle_magic_bullet( );
			targeting.handle_aimbot( drawing );
			targeting.handle_visuals( drawing );
			targeting.handle_bullets( drawing, viewport_client );
			if ( !targeting.handle_triggerbot( ) )
				targeting.reset_triggerbot( );
			targeting.reset( );
		}

		drawing.get_mouse_state( player_controller );
		if ( drawing.is_gui_enabled( player_controller ) ) {
			static int menu_tab = 0;
			drawing.render_menu( encrypt( L"orelia | fortnite internal" ), &menu_tab );
			drawing.handle_scroll_input( player_controller );
			drawing.draw_columns( menu_tab );
			drawing.draw_cursor( );
		}

		drawing.debug_draw_list( );
		drawing.draw_background( );
		drawing.draw_foreground( );
		drawing.clear_draw_list( );

		return draw_transition_original( viewport_client, canvas );
	}
}