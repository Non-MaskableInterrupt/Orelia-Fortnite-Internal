#pragma once
#include <deque>

namespace targeting {
    struct tracer_info {
        engine::fvector start;
        engine::fvector end;
        double impact_angle;
        double creation_time;
    };

    struct target_score {
        engine::a_fort_player_pawn_athena* pawn;
        double crosshair_score;
        double distance_score;
        double total_score;
    };

    class c_targeting {
    public:
        bool setup(
            engine::u_world* world,
            engine::a_player_controller* controller,
            drawing::c_framework drawing,
            engine::c_camera_info camera_info
        ) {
            m_world = world;
            if ( !m_world ) return false;

            m_player_controller = controller;
            m_local_pawn = controller->acknowledged_pawn( );
            if ( !m_local_pawn ) return false;

            m_local_weapon = m_local_pawn->current_weapon( );
            m_screen_center = drawing.get_screen_center( );
            m_camera_location = camera_info.get_location( );
            m_camera_rotation = camera_info.get_rotation( );
            m_field_of_view = camera_info.get_fov( );
            m_aimbot_fov = camera_info.get_aimbot_fov(
                features::aimbot::fov_radius,
                m_screen_center.m_x
            );
        }

        void reset( ) {
            m_best_target_score = DBL_MAX;
            m_target_info = {};
        }

        void draw_target_info( drawing::c_framework& drawing ) {
            if ( m_target_info.pawn && features::misc::debug_targeting ) {
                std::vector<std::pair<engine::fstring, int>> score_info = {
                    { L"crosshair score", m_target_info.crosshair_score },
                    { L"distance score", m_target_info.distance_score },
                    { L"total score", m_target_info.total_score }
                };
                drawing.debug_box( L"target scores", score_info );
            }
        }

        void process_actor(
            drawing::c_framework& drawing,
            engine::a_fort_player_pawn_athena* pawn,
            engine::fvector2d screen_pos
        ) {
            if ( !is_pawn_valid( pawn ) || !drawing.in_circle( m_aimbot_fov, screen_pos ) )
                return;

            auto crosshair_score = screen_pos.distance_to( m_screen_center );

            auto pawn_location = pawn->k2_get_actor_location( );
            auto distance_score = pawn_location.distance_to( m_camera_location );

            auto total_score = crosshair_score + ( distance_score * 0.05 );
            if ( total_score < m_best_target_score ) {
                m_best_target_score = total_score;
                m_target_info = {
                    pawn,
                    crosshair_score,
                    distance_score,
                    total_score
                };
            }
        }

        bool handle_aimbot( drawing::c_framework& drawing ) {
            if ( !is_aimbot_valid( ) ) return false;

            auto target_pawn = m_target_info.pawn;
            if ( !target_pawn ) return false;

            engine::fvector aim_location;
            auto result = get_aim_location( target_pawn, &aim_location );
            if ( !result ) return false;

            engine::fvector2d aim_position;
            result = world_to_screen( aim_location, &aim_position );
            if ( !result ) return false;

            if ( features::aimbot::target_line ) {
                engine::fvector2d muzzle_position;
                auto muzzle_location = m_local_weapon->get_muzzle_location( 0 );
                if ( drawing.project( muzzle_location, &muzzle_position ) )
                    drawing.line(
                        muzzle_position,
                        aim_position,
                        features::aimbot::line_color,
                        1.f
                    );
            }

            auto targeting_location = m_local_weapon->get_targeting_source_location( );
            auto distance = engine::kismet::g_math_library->vector_distance(
                targeting_location,
                aim_location );
            apply_prediction( distance, targeting_location, aim_location );

            if ( features::aimbot::target_dot ) {
                auto screen_size = drawing.get_screen_size( );
                auto raduis = screen_size.m_y /
                    ( 2.0 * distance *
                        engine::kismet::g_math_library->tan( m_field_of_view * engine::math::pi / 360.0 )
                        ) * 8.0;

                engine::fvector2d world_position;
                if ( drawing.project( aim_location, &world_position ) )
                    drawing.circle(
                        world_position,
                        features::aimbot::dot_color,
                        raduis,
                        138.0,
                        true,
                        true
                    );
            }

            if ( features::visuals::minimap_esp ) {
                if ( auto marker_component = m_player_controller->marker_component( ) ) {
                    if ( auto last_ping_marker = marker_component->last_ping_marker( ) ) {
                        if ( auto marker_actor = last_ping_marker->marker_actor( ) ) {
                            marker_actor->color( features::visuals::minimap_color );
                            marker_actor->impact_location( aim_location );
                        }
                    }
                }
            }

            if ( features::exploits::silent_aim ) {
                exploits::psilent::g::silent_location = aim_location;
                if ( !exploits::psilent::apply( m_local_weapon ) )
                    return false;
            }
            else if ( is_holding_aim_key( ) ) {
                auto smoothed_location = apply_smoothing( aim_location, aim_position );
                if ( !smoothed_location ) return false;

                engine::frotator aim_rotation;
                auto result = get_aim_rotation( smoothed_location, &aim_rotation );
                if ( !result ) return false;

                move_mouse( aim_rotation );
            }

            return true;
        }

        bool handle_visuals( drawing::c_framework& drawing ) {
            if ( !is_weapon_valid( ) ) return false;
            if ( is_aimbot_valid( ) )
                draw_target_info( drawing );

            if ( features::aimbot::crosshair ) {
                drawing.crosshair(
                    features::aimbot::crosshair_size,
                    features::aimbot::crosshair_color,
                    1.0
                );

                m_local_weapon->enable_crosshair( features::aimbot::hide_reticle );
            }

            if ( features::aimbot::draw_fov ) {
                drawing.circle(
                    m_screen_center,
                    features::aimbot::fov_color,
                    m_aimbot_fov,
                    138.0
                );
            }

            if ( features::visuals::bullet_tracers && !m_tracer_cache.empty( ) ) {
                double current_time = engine::kismet::g_gameplay_statics->get_real_time_seconds( m_world );
                while ( !m_tracer_cache.empty( ) &&
                    ( current_time - m_tracer_cache.front( ).creation_time ) > features::visuals::tracer_time ) {
                    m_tracer_cache.pop_front( );
                }

                for ( const auto& tracer : m_tracer_cache ) {
                    engine::fvector2d start_screen, end_screen;
                    if ( !world_to_screen( tracer.start, &start_screen ) || !world_to_screen( tracer.end, &end_screen ) )
                        continue;

                    drawing.line( start_screen, end_screen, features::visuals::tracer_color, 1.0 );

                    if ( features::visuals::hit_marker ) {
                        double thickness = 2.0;
                        double cross_size = 8.0;

                        drawing.line(
                            { end_screen.m_x - cross_size, end_screen.m_y - cross_size },
                            { end_screen.m_x + cross_size, end_screen.m_y + cross_size },
                            features::visuals::marker_color,
                            1.0
                        );
                        drawing.line(
                            { end_screen.m_x - cross_size, end_screen.m_y + cross_size },
                            { end_screen.m_x + cross_size, end_screen.m_y - cross_size },
                            features::visuals::marker_color,
                            1.0
                        );

                        double impact_size = 1.0 - ( tracer.impact_angle / 180.0 );
                        const int segments = 8;
                        for ( int i = 0; i < segments; i++ ) {
                            double t1 = ( i / static_cast< double >( segments ) ) * ( 2.0 * engine::math::pi );
                            double t2 = ( ( i + 1 ) / static_cast< double >( segments ) ) * ( 2.0 * engine::math::pi );
                            double radius = impact_size * 6.0;

                            engine::fvector2d p1 = {
                                end_screen.m_x + std::cos( t1 ) * radius,
                                end_screen.m_y + std::sin( t1 ) * radius
                            };
                            engine::fvector2d p2 = {
                                end_screen.m_x + std::cos( t2 ) * radius,
                                end_screen.m_y + std::sin( t2 ) * radius
                            };

                            drawing.line( p1, p2, features::visuals::marker_color, 0.5 );
                        }
                    }
                }
            }

            return true;
        }

        bool handle_bullets( drawing::c_framework& drawing, engine::u_game_viewport_client* viewport ) {
            if ( !features::visuals::bullet_tracers ) 
                return true;

            if ( !is_weapon_valid( ) ) 
                return false;

            if ( !m_local_weapon->is_firing( ) )
                return false;

            auto location = m_local_weapon->get_muzzle_location( 0 );
            auto direction = m_camera_rotation.get_forward_vector( );
            auto trace_end = location + ( direction * 50000.0 );

            engine::f_hit_result hit_result;
            auto result = viewport->line_trace( location, trace_end, &hit_result );
            if ( hit_result.m_face_index == -1 ) {
                auto impact_direction = m_local_pawn->last_fired_direction( );
                if ( !impact_direction ) return false;
                impact_direction.normalize( );

                auto impact_location = m_local_pawn->last_fired_location( );
                if ( !impact_location ) return false;

                auto impact_angle = engine::kismet::g_math_library->acos(
                    direction.dot( impact_direction )
                );
                auto angle_degrees = impact_angle * ( 180.0 / 3.14159 );

                auto distance = static_cast< double >( hit_result.m_distance );
                auto end_location = impact_location + ( impact_direction * distance );

                if ( features::visuals::bullet_tracers ) {
                    if ( !is_duplicate_tracer( impact_location, end_location ) ) {
                        if ( m_tracer_cache.size( ) >= m_max_tracers )
                            m_tracer_cache.pop_front( );

                        m_tracer_cache.push_back( {
                            impact_location,
                            end_location,
                            angle_degrees,
                            engine::kismet::g_gameplay_statics->get_real_time_seconds( m_world )
                            } );
                    }
                }
            }

            return true;
        }

        bool handle_triggerbot( ) {
            if ( !features::aimbot::trigger_bot )
                return true;

            if ( !is_aimbot_valid( ) )
                return false;

            auto target_pawn = m_target_info.pawn;
            if ( !target_pawn )
                return false;

            const bool key_valid = features::aimbot::trigger_key ?
                is_holding_trigger_key( ) : true;
            const bool valid_weapon = features::aimbot::shotgun_only ?
                m_local_weapon->is_holding_shotgun( ) :
                m_local_weapon->can_fire( );
            if ( !valid_weapon || !key_valid ) {
                if ( m_is_firing && m_local_weapon->is_firing( ) )
                    m_local_pawn->pawn_stop_fire( 1 );
                m_trigger_start_time = 0.0;
                return false;
            }

            if ( is_visible( target_pawn ) ) {
                auto current_time = engine::kismet::g_gameplay_statics->get_real_time_seconds( m_world );
                if ( m_trigger_start_time == 0.0 ) 
                    m_trigger_start_time = current_time;

                auto is_time_valid = features::aimbot::triggerbot_time ?
                    ( current_time - m_trigger_start_time ) >= features::aimbot::triggerbot_time :
                    true;
                if ( is_time_valid ) {
                    m_local_pawn->pawn_start_fire( 1 );
                    m_is_firing = true;
                    return true;
                }
            }

            return false;
        }

        bool handle_magic_bullet( ) {
            if ( !is_weapon_valid( ) ) 
                return false;

            if ( !features::exploits::enabled )
                return false;

            if ( !features::exploits::shoot_through_walls )
                return false;

            auto target_pawn = m_target_info.pawn;
            if ( !target_pawn )
                return false;

            auto building_array = m_world->get_actors_of_class<engine::a_building_actor*>( 
                engine::classes::g_building
            );
            for ( int i = 0; i < building_array.size( ); i++ ) {
                auto building_actor = building_array[ i ];
                if ( !building_actor ) continue;

                if ( is_holding_aim_key( ) ) {
                    auto target_mesh = target_pawn->mesh( );
                    if ( target_mesh ) {
                        target_mesh->invert_use_as_occluder( true );
                        target_mesh->set_collision_response_to_channel(
                            engine::e_collision_channel::visibility,
                            engine::e_collision_response::ignore
                        );
                    }

                    building_actor->set_actor_enable_collision( false );
                    m_local_pawn->set_actor_enable_collision( false );
                }
                else {
                    building_actor->set_actor_enable_collision( true );
                    m_local_pawn->set_actor_enable_collision( true );
                }
            }

            return true;
        }

        void reset_triggerbot( ) {
            if ( !is_weapon_valid( ) ) return;
            if ( m_is_firing && m_local_weapon->is_firing( ) )
                m_local_pawn->pawn_stop_fire( 1 );
            m_is_firing = false;
            m_trigger_start_time = 0.0;
        }

    public:
        void move_mouse( engine::frotator aim_rotation ) {
            m_local_pawn->server_set_aimbot_detection( false );
            m_player_controller->weapon_aim_offset( aim_rotation );
            m_player_controller->desired_aim_target_offset( aim_rotation.m_pitch );
            if ( features::exploits::no_recoil )
                m_player_controller->weapon_recoil_offset( aim_rotation.normalize( ) );
        }

        bool move_mouse( engine::fvector2d aim_position ) {
            auto game_instance = m_world->game_instance( );
            if ( !game_instance ) return false;

            auto local_player = game_instance->get_localplayer( );
            if ( !local_player ) return false;

            auto client_settings_record = local_player->client_settings_record( );
            if ( !client_settings_record ) return false;

            auto screen_width = m_screen_center.m_x * 2.0f;
            auto screen_height = m_screen_center.m_y * 2.0f;

            auto normalized_x = aim_position.m_x / screen_width;
            auto normalized_y = aim_position.m_y / screen_height;

            float input_x = ( normalized_x * 2.0f ) - 1.0f;
            float input_y = ( normalized_y * 2.0f ) - 1.0f;

            auto dead_zone = client_settings_record->gamepad_look_stick_dead_zone( );
            auto sensitivity = client_settings_record->gamepad_look_sensitivity( );

            auto magnitude = std::sqrt( input_x * input_x + input_y * input_y );
            auto adjusted_magnitude = ( magnitude - dead_zone ) / ( 1.0f - dead_zone );
            input_x = ( input_x / magnitude ) * adjusted_magnitude;
            input_y = ( input_y / magnitude ) * adjusted_magnitude;

            input_x *= sensitivity.m_x;
            input_y *= sensitivity.m_y;

            input_x = std::clamp( input_x, -1.0f, 1.0f );
            input_y = std::clamp( input_y, -1.0f, 1.0f );

            int stick_x = static_cast< int >( ( input_x + 1.0f ) * 127.5f );
            int stick_y = static_cast< int >( ( input_y + 1.0f ) * 127.5f );

            stick_x = std::clamp( stick_x, 0, 255 );
            stick_y = std::clamp( stick_y, 0, 255 );

            g_pipe.move_mouse( stick_x, stick_y );
            g_pipe.reset_mouse( );
            return true;
        }
        
        void apply_prediction( double distance, engine::fvector start_location, engine::fvector& world_location ) {
            if ( features::aimbot::engine_prediction ) {
                auto projectile_speed = m_local_weapon->projectile_speed( );
                if ( projectile_speed > 0 ) {
                    auto target_velocity = get_velocity( m_target_info.pawn );

                    auto projectile_gravity = m_local_weapon->projectile_gravity( );
                    float gravity = projectile_gravity * -980.0f;
                    float dx = world_location.m_x - start_location.m_x;
                    float dy = world_location.m_y - start_location.m_y;
                    float dz = world_location.m_z - start_location.m_z;

                    float ground_dist = engine::kismet::g_math_library->sqrt( dx * dx + dy * dy );
                    float time = ground_dist / projectile_speed;

                    float pred_x = world_location.m_x + target_velocity.m_x * time;
                    float pred_y = world_location.m_y + target_velocity.m_y * time;
                    float pred_z = world_location.m_z + target_velocity.m_z * time;

                    float drop = 0.5f * gravity * time * time;
                    pred_z += drop;

                    float new_dx = pred_x - start_location.m_x;
                    float new_dy = pred_y - start_location.m_y;
                    float new_dz = pred_z - start_location.m_z;

                    float new_dist = engine::kismet::g_math_library->sqrt(
                        new_dx * new_dx +
                        new_dy * new_dy +
                        new_dz * new_dz
                    );

                    time = new_dist / projectile_speed;

                    world_location.m_x += target_velocity.m_x * time;
                    world_location.m_y += target_velocity.m_y * time;
                    world_location.m_z += target_velocity.m_z * time - ( 0.5f * gravity * time * time );
                }
            }
        }

        engine::fvector apply_smoothing( engine::fvector aim_location, engine::fvector2d aim_position ) {
            if ( !aim_location || !aim_position ||
                features::aimbot::smoothing <= 0 ) {
                return aim_location;
            }

            float dx = aim_position.m_x - m_screen_center.m_x;
            float dy = aim_position.m_y - m_screen_center.m_y;

            if ( features::aimbot::constant ) {
                float max_step = 100.0f / ( features::aimbot::smoothing + 1.0f );

                float distance = std::sqrt( dx * dx + dy * dy );
                if ( distance > max_step ) {
                    dx = ( dx / distance ) * max_step;
                    dy = ( dy / distance ) * max_step;
                }

                dx = std::clamp( dx, -max_step, max_step );
                dy = std::clamp( dy, -max_step, max_step );
            }
            else {
                float smoothing = features::aimbot::smoothing + 5.0f;
                dx /= smoothing;
                dy /= smoothing;

                if ( dx > 0.24f && dx < 1.0f )
                    dx = 1.0f;
                else if ( dx < -0.24f && dx > -1.0f )
                    dx = -1.0f;

                if ( dy > 0.24f && dy < 1.0f )
                    dy = 1.0f;
                else if ( dy < -0.24f && dy > -1.0f )
                    dy = -1.0f;

                if ( smoothing <= 2.0f ) {
                    dx = std::clamp( dx, -256.0f, 256.0f );
                    dy = std::clamp( dy, -256.0f, 256.0f );
                }
                else {
                    dx = std::clamp( dx, -68.0f, 68.0f );
                    dy = std::clamp( dy, -68.0f, 68.0f );
                }
            }

            bool curve_enabled = features::aimbot::curve;
            if ( curve_enabled && ( rand( ) % 1550 == 0 ) ) {
                m_positive_curve = !m_positive_curve;
            }

            auto apply_random = [ ]( float& value, bool positive ) {
                int abs_val = static_cast< int >( std::abs( value ) );
                int rand_val = 0;

                if ( abs_val >= 32 ) rand_val = rand( ) % 21;
                else if ( abs_val > 16 ) rand_val = rand( ) % 8;
                else if ( abs_val > 8 ) rand_val = rand( ) % 4;
                else if ( abs_val > 3 ) rand_val = rand( ) % 2;
                else if ( abs_val > 1 ) rand_val = rand( ) % 1;

                if ( positive ) value += rand_val;
                else value -= rand_val;
                };

            if ( curve_enabled ) {
                apply_random( dx, m_positive_curve );
                apply_random( dy, m_positive_curve );
            }


            engine::fvector2d smoothed_position(
                m_screen_center.m_x + dx,
                m_screen_center.m_y + dy
            );

            engine::fvector world_location;
            engine::fvector world_direction;
            if ( m_player_controller->deproject_screen_position_to_world(
                smoothed_position,
                &world_location,
                &world_direction ) ) {
                auto distance = m_camera_location.distance_to( aim_location );
                return world_location + ( world_direction * distance );
            }

            return aim_location;
        }

    public:
        bool is_aimbot_valid( ) {
            if ( !is_weapon_valid( ) ||
                !m_target_info.pawn )
                return false;
            return m_target_info.total_score < DBL_MAX;
        }

        bool is_weapon_valid( ) {
            if ( !m_player_controller || !m_local_pawn || !m_local_weapon )
                return false;
            return m_local_weapon->object_is_a( engine::classes::g_fort_weapon );
        }

        bool is_pawn_valid( engine::a_fort_player_pawn* pawn ) const {
            return pawn && ( !features::aimbot::ignore_dbno || !pawn->is_dbno( ) ) &&
                ( !features::aimbot::only_visible || is_visible( pawn ) ) &&
                ( !features::aimbot::ignore_bots || !pawn->player_state( )->is_a_bot( ) );
        }

        bool is_visible( engine::a_pawn* pawn ) const {
            return engine::kismet::g_fort_library->check_line_of_sight_to_actor_with_channel(
                m_camera_location,
                pawn,
                engine::e_collision_channel::visibility,
                m_local_pawn
            );
        }

        bool is_holding_aim_key( ) const {
            if ( !m_player_controller )
                return false;

            static const std::vector<engine::fkey> aimbot_keys = {
                engine::keys::g_right_mouse,
                engine::keys::g_left_shift,
                engine::keys::g_left_alt,
                engine::keys::g_thumb_mouse_button,
                engine::keys::g_thumb_mouse_button2,
                engine::keys::g_gamepad_left_trigger
            };

            if ( features::aimbot::keybind >= 0 &&
                features::aimbot::keybind < aimbot_keys.size( ) ) {
                return m_player_controller->is_input_key_down( aimbot_keys[ features::aimbot::keybind ] );
            }

            return false;
        }

    private:
        bool is_holding_trigger_key( ) const {
            if ( !m_player_controller )
                return false;

            if ( features::aimbot::trigger_key ) {
                static const std::vector<engine::fkey> triggerbot_keys = {
                    engine::keys::g_left_shift,
                    engine::keys::g_left_alt,
                    engine::keys::g_thumb_mouse_button,
                    engine::keys::g_thumb_mouse_button2
                };

                if ( features::aimbot::triggerbot_key >= 0 &&
                    features::aimbot::triggerbot_key < triggerbot_keys.size( ) ) {
                    return m_player_controller->is_input_key_down( triggerbot_keys[ features::aimbot::triggerbot_key ] );
                }
            }

            return true;
        }

       bool get_aim_location( engine::a_fort_player_pawn_athena* pawn, engine::fvector* world_location ) {
            auto closest_mesh = pawn->mesh( );
            if ( !closest_mesh ) return engine::fvector( );

            static const std::vector<engine::fstring> aimbone_names = {
                 L"head",
                 L"neck_02",
                 L"spine_05"
            };

            *world_location = closest_mesh->get_socket_location(
                engine::kismet::g_string_library->conv_string_to_name(
                aimbone_names[ features::aimbot::aimbone ]
            )
            );

            return world_location;
        }

        bool get_aim_rotation( engine::fvector target_location, engine::frotator* aim_rotation ) const {
            auto rotation = engine::kismet::g_math_library->find_look_at_rotation( m_camera_location, target_location );
            rotation -= m_camera_rotation;
            if ( features::aimbot::deadzone ) {
                rotation = engine::frotator(
                    engine::math::clamp( rotation.m_pitch, -features::aimbot::restriction, features::aimbot::restriction ),
                    engine::math::clamp( rotation.m_yaw, -features::aimbot::restriction, features::aimbot::restriction ),
                    0
                );
            }

            if ( std::abs( rotation.m_pitch ) < 0.1f ) rotation.m_pitch = 0.f;
            if ( std::abs( rotation.m_yaw ) < 0.1f ) rotation.m_yaw = 0.f;
            *aim_rotation = rotation;
            return rotation;
        }

        bool is_duplicate_tracer( engine::fvector& start, engine::fvector& end, double threshold = 0.50 ) {
            for ( const auto& tracer : m_tracer_cache ) {
                auto tracer_start = tracer.start;
                auto tracer_end = tracer.end;
                if ( tracer_start.distance_to( start ) < threshold &&
                    tracer_end.distance_to( end ) < threshold )
                    return true;
            }
            return false;
        }

        bool world_to_screen( engine::fvector world_location, engine::fvector2d* screen_position ) {
            auto matrix = engine::math::get_rotation_matrix( m_camera_rotation );

            engine::fvector x_axis = {
                matrix.m_x_plane.m_x,
                matrix.m_x_plane.m_y,
                matrix.m_x_plane.m_z
            };

            engine::fvector y_axis = {
                matrix.m_y_plane.m_x,
                matrix.m_y_plane.m_y,
                matrix.m_y_plane.m_z
            };

            engine::fvector z_axis = {
                matrix.m_z_plane.m_x,
                matrix.m_z_plane.m_y,
                matrix.m_z_plane.m_z
            };

            auto delta = world_location - m_camera_location;

            engine::fvector transform = {
                delta.dot( y_axis ),
                delta.dot( z_axis ),
                delta.dot( x_axis )
            };

            if ( transform.m_z < 0.01 )
                transform.m_z = 0.01;

            double fov_multiplier = m_screen_center.m_x / engine::kismet::g_math_library->tan( m_field_of_view * engine::math::pi / 360.0 );
            screen_position->m_x = m_screen_center.m_x + transform.m_x * fov_multiplier / transform.m_z;
            screen_position->m_y = m_screen_center.m_y - transform.m_y * fov_multiplier / transform.m_z;
            return *screen_position;
        }

        engine::fvector get_velocity( engine::a_fort_player_pawn_athena* pawn ) const {
            if ( pawn->is_in_vehicle( ) ) {
                auto current_vehicle = pawn->current_vehicle( );
                if ( current_vehicle )
                    return current_vehicle->get_velocity( );
            }

            return pawn->get_velocity( );
        }

    private:
        engine::u_world* m_world;
        engine::a_fort_player_pawn_athena* m_local_pawn = nullptr;
        engine::a_fort_weapon* m_local_weapon = nullptr;
        engine::a_player_controller* m_player_controller = nullptr;
        const size_t m_max_tracers = 5;

        engine::fvector2d m_screen_center;
        engine::fvector m_camera_location;
        engine::frotator m_camera_rotation;
        float m_field_of_view;
        double m_aimbot_fov;

        double m_best_target_score = DBL_MAX;
        target_score m_target_info = {};
        bool m_positive_curve = false;

        bool m_is_firing = false;
        double m_trigger_start_time = 0.0;
        std::deque<tracer_info> m_tracer_cache;
    };
}