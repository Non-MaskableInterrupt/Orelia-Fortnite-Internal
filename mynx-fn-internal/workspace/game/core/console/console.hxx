namespace console {
    class c_console {
    public:
        bool update(
            engine::u_world* world,
            engine::a_player_controller* controller,
            engine::u_game_viewport_client* viewport
        ) {
            if ( !features::world::effects ) return true;

            if ( !m_unlock_ue_console ) {
                spawn_console( viewport );
                m_unlock_ue_console = true;
            }

            m_world = world;
            apply_visual_modifications( controller );
            return true;
        }

        void apply_world_rendering( engine::u_game_viewport_client* viewport ) {
            if ( !features::world::effects ) return;

            const bool disable_rendering = features::world::preformence_mode;
            if ( disable_rendering != m_disable_rendering ) {
                viewport->b_disable_world_rendering( disable_rendering );
                m_disable_rendering = disable_rendering;
            }
        }

    private:
        void spawn_console( engine::u_game_viewport_client* viewport ) {

        }

        engine::u_object* spawn_via_gameplay_statics(
            engine::u_class* class_obj,
            engine::u_object* outer
        ) {
            static engine::u_function* spawn_func = engine::u_object::find_object<engine::u_function*>(
                encrypt( L"GameplayStatics.SpawnObject" )
            );

            if ( spawn_func ) {
                struct {
                    engine::u_object* object_class;
                    engine::u_object* outer;
                    engine::u_object* return_value;
                } params{ class_obj, outer };

                engine::kismet::g_gameplay_statics->process_event( spawn_func, &params );
                return params.return_value;
            }
            return nullptr;
        }

        void apply_visual_modifications( engine::a_player_controller* controller ) {
            update_skylight( controller );
            update_night_mode( controller );
            update_fog( controller );
            update_shadows( controller );
        }

        void update_skylight( engine::a_player_controller* controller ) {
            if ( m_skylight.update( features::world::sky_light ) ) {
                const wchar_t* cmd = features::world::sky_light
                    ? L"r.SkyLightIntensityMultiplier 5.0"
                    : L"r.SkyLightIntensityMultiplier 1.0";
                execute_command( controller, cmd );
            }
        }

        void update_night_mode( engine::a_player_controller* controller ) {
            if ( m_unlit.update( features::world::night_mode ) ) {
                execute_command( controller,
                    features::world::night_mode
                    ? L"r.SkyAtmosphere 0"
                    : L"r.SkyAtmosphere 1"
                );
            }
        }

        void update_fog( engine::a_player_controller* controller ) {
            if ( m_fog.update( features::world::disable_fog ) ) {
                execute_command( controller,
                    features::world::disable_fog
                    ? L"r.Fog 0"
                    : L"r.Fog 1"
                );
            }
        }

        void update_shadows( engine::a_player_controller* controller ) {
            if ( m_shadows.update( features::world::disable_shadows ) ) {
                execute_command( controller,
                    features::world::disable_shadows
                    ? L"r.ShadowQuality 0"
                    : L"r.ShadowQuality 1"
                );
            }
        }

        void execute_command( engine::a_player_controller* pc, const wchar_t* cmd ) {
            engine::kismet::g_system_library->execute_console_command( m_world, cmd, pc );
        }

    private:
        struct state_tracker {
            bool previous_state = false;

            bool update( bool current_state ) {
                if ( current_state != previous_state ) {
                    previous_state = current_state;
                    return true;
                }
                return false;
            }
        };

        state_tracker m_unlit, m_fog, m_shadows, m_skylight;
        engine::u_world* m_world;
        bool m_unlock_ue_console = false;
        bool m_disable_rendering = false;
    };
}