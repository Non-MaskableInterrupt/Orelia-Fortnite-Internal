
namespace engine {
	class u_fort_kismet_library : public u_blueprint_function_library {
	public:
		a_controller* get_controller_from_actor( a_actor* actor ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortKismetLibrary.GetControllerFromActor" );
				function = find_object<u_function*>( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				a_actor* actor;
				a_controller* return_value;
			} params{ actor };

			this->process_event( function, &params );

			return params.return_value;
		}

		a_fort_game_state_athena* get_game_state_athena( u_world* world_context_object ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortKismetLibrary.GetGameStateAthena" );
				function = find_object<u_function*>( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				u_world* world_context_object;
				a_fort_game_state_athena* return_value;
			} params{ world_context_object };

			this->process_event( function, &params );

			return params.return_value;
		}

		fstring get_human_readable_name( a_actor* actor_a ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortKismetLibrary.GetHumanReadableName" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				a_actor* actor_a;
				fstring return_value;
			} params{ actor_a };

			this->process_event( function, &params );

			return params.return_value;
		}

		bool check_line_of_sight_to_actor_with_channel( fvector source_pos, a_actor* target, e_collision_channel target_filter_channel, a_actor* source ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortKismetLibrary.CheckLineOfSightToActorWithChannel" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				fvector source_pos;
				a_actor* target;
				e_collision_channel target_filter_channel;
				a_actor* source;
				bool return_value;
			} params{ source_pos,target, target_filter_channel, source };

			this->process_event( function, &params );

			return params.return_value;
		}

		bool on_same_team( a_actor* actor_a, a_actor* actor_b ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortKismetLibrary.OnSameTeam" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				a_actor* actor_a;
				a_actor* actor_b;
				bool return_value;
			} params { actor_a, actor_b };

			this->process_event( function, &params );

			return params.return_value;
		}
	};
}