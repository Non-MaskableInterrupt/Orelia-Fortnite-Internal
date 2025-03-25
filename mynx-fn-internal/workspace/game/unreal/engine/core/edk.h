namespace offsets {
	// vtable
	std::uint32_t process_event = 0x22;
	std::uint32_t draw_transition = 115;
	std::uint32_t update_damage_start_location = 253;

	//functions
	std::uint32_t malloc = 0x1E83B10;
	std::uint32_t free = 0x1D7467C;
	std::uint32_t static_find_object = 0x21EE808;
	std::uint32_t static_construct_object_internal = 0x1512C30;
	std::uint32_t static_allocate_object = 0x151320C;

	// non-sdk
	std::uint32_t projectile_gravity = 0x1f14;
	std::uint32_t projectile_speed = 0x1f34; // gravity + 200
}

namespace engine {
	class u_object;
	class f_memory;
	class u_particle_system;
	class u_particle_system_component;
	class u_sound_base;
	class u_sound_attenuation;
	class u_sound_concurrency;
	class u_initial_active_sound_params;
	class u_blueprint_function_library;
	class u_font;
	class u_class;
	class u_field;
	class u_struct;
	class u_function;
	class u_world;
	class u_scene_component;
	class a_actor;
	class a_fort_player_state_athena;
	class u_material_interface;
	class a_fort_player_state_zone;
	class u_fort_world_item;
	class u_fort_world_item_definition;
	class a_controller;
	class a_player_controller;
	class u_kismet_math_library;
	class u_kismet_string_library;
	class u_kismet_system_library;
	class u_fort_kismet_library;
	class u_gameplay_statics;
	class u_mesh_component;
	class u_material_instance_dynamic;
	class a_fort_game_state_athena;

	namespace kismet {
		u_kismet_math_library* g_math_library;
		u_kismet_string_library* g_string_library;
		u_kismet_system_library* g_system_library;
		u_fort_kismet_library* g_fort_library;
		u_gameplay_statics* g_gameplay_statics;
	}

	namespace classes {
		u_class* g_fort_weapon;
		u_class* g_fort_pickup;
		u_class* g_container;
		u_class* g_weakspot;
		u_class* g_vehicle;
		u_class* g_material_instance;
		u_class* g_building;
		u_class* g_item_definition;
		u_class* g_player_controller;
	}
}

namespace bones {
	const std::pair<const wchar_t*, const wchar_t*> bone_pairs[ ] = {
		std::make_pair( L"neck_01", L"upperarm_r" ),
		std::make_pair( L"neck_01", L"upperarm_l" ),
		std::make_pair( L"upperarm_l", L"lowerarm_l" ),
		std::make_pair( L"lowerarm_l", L"hand_l" ),
		std::make_pair( L"upperarm_r", L"lowerarm_r" ),
		std::make_pair( L"lowerarm_r", L"hand_r" ),
		std::make_pair( L"neck_01", L"pelvis" ),
		std::make_pair( L"pelvis", L"thigh_r" ),
		std::make_pair( L"pelvis", L"thigh_l" ),
		std::make_pair( L"thigh_r", L"calf_r" ),
		std::make_pair( L"calf_r", L"ik_foot_r" ),
		std::make_pair( L"thigh_l", L"calf_l" ),
		std::make_pair( L"calf_l", L"ik_foot_l" )
	};

	const std::vector<std::pair<const wchar_t*, const wchar_t*>> skeleton_pairs(
		std::begin( bone_pairs ),
		std::end( bone_pairs )
	);
}