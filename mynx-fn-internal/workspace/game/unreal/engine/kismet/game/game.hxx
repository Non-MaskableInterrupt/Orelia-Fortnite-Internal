
namespace engine {
	class u_gameplay_statics : public u_blueprint_function_library {
	public:
        double get_real_time_seconds( u_object* world_context_object ) {
            static u_function* function = 0;
            if ( !function ) {
                auto fn_name = encrypt( L"GameplayStatics.GetRealTimeSeconds" );
                function = find_object< u_function* >( fn_name.decrypt( ) );
                fn_name.clear( );
            }

            struct {
                u_object* world_context_object;
                double return_value;
            } params{ world_context_object };

            this->process_event( function, &params );

            return params.return_value;
        }

        u_particle_system_component* spawn_emitter_at_location(
            u_object* world,
            u_particle_system emitter_template,
            const fvector& location,
            const frotator& rotation,
            const fvector& scale,
            bool auto_destroy,
            e_psc_pool_method pooling_method,
            bool auto_activate
        ) {
            static u_function* function = 0;
            if ( !function ) {
                auto fn_name = encrypt( L"GameplayStatics.SpawnEmitterAtLocation" );
                function = find_object< u_function* >( fn_name.decrypt( ) );
                fn_name.clear( );
            }

            struct {
                u_object* world_context_object;
                u_particle_system emitter_template;
                fvector location;
                frotator rotation;
                fvector scale;
                bool auto_destroy;
                e_psc_pool_method pooling_method;
                bool auto_activate;
                u_particle_system_component* return_value;
            } params;

            params.world_context_object = world;
            params.emitter_template = emitter_template;
            params.location = location;
            params.rotation = rotation;
            params.scale = scale;
            params.auto_destroy = auto_destroy;
            params.pooling_method = pooling_method;
            params.auto_activate = auto_activate;

            process_event( function, &params );
            return params.return_value;
        }

        bool play_sound_at_location(
            u_object* world,
            u_sound_base sound,
            const fvector& location,
            float volume_multiplier,
            float pitch_multiplier,
            float start_time = 0.0f,
            u_sound_attenuation* attenuation_settings = nullptr,
            u_sound_concurrency* concurrency_settings = nullptr,
            a_actor* owning_actor = nullptr
        ) {
            static u_function* function = 0;
            if ( !function ) {
                auto fn_name = encrypt( L"GameplayStatics.PlaySoundAtLocation" );
                function = find_object< u_function* >( fn_name.decrypt( ) );
                fn_name.clear( );
            }

            struct {
                u_object* world_context_object;
                u_sound_base sound;
                fvector location;
                frotator rotation;
                float volume_multiplier;
                float pitch_multiplier;
                float start_time;
                u_sound_attenuation* attenuation_settings;
                u_sound_concurrency* concurrency_settings;
                a_actor* owning_actor;
                u_initial_active_sound_params* initial_params;
            } params;

            params.world_context_object = world;
            params.sound = sound;
            params.location = location;
            params.rotation = frotator( );
            params.volume_multiplier = volume_multiplier;
            params.pitch_multiplier = pitch_multiplier;
            params.start_time = start_time;
            params.attenuation_settings = attenuation_settings;
            params.concurrency_settings = concurrency_settings;
            params.owning_actor = owning_actor;
            params.initial_params = nullptr;

            process_event( function, &params );
            return true;
        }

		template <class t>
		tarray<t> get_all_actors_of_class( u_object* world_context_object, u_object* actor_class ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"GameplayStatics.GetAllActorsOfClass" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				u_object* world_context_object;
				u_object* actor_class;
				tarray<t> out_actors;
			} params{ world_context_object, actor_class };

			this->process_event( function, &params );

			return params.out_actors;
		}

		double get_world_delta_seconds( u_object* world_context_object ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"GameplayStatics.GetWorldDeltaSeconds" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				u_object* world_context_object;
				double return_value;
			} params{ world_context_object };

			this->process_event( function, &params );

			return params.return_value;
		}

        double get_time_seconds( u_object* world_context_object ) {
            static u_function* function = 0;
            if ( !function ) {
                auto fn_name = encrypt( L"GameplayStatics.GetTimeSeconds" );
                function = find_object< u_function* >( fn_name.decrypt( ) );
                fn_name.clear( );
            }

            struct {
                u_object* world_context_object;
                double return_value;
            } params{ world_context_object };

            this->process_event( function, &params );

            return params.return_value;
        }

		bool object_is_a( u_object* object, u_object* object_class ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"GameplayStatics.ObjectIsA" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				u_object* object;
				u_object* object_class;
				bool return_value;
			} params{ object, object_class };

			this->process_event( function, &params );

			return params.return_value;
		}
	};
}