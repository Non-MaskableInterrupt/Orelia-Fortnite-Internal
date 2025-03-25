#pragma once

namespace features {
	namespace aimbot {
		bool enabled = true;
		bool curve = true;
		bool constant = true;
		int keybind = 0;
		bool draw_fov = true;
		auto fov_color = engine::flinear_color( 1.f, 1.f, 1.f, 1.f );
		bool target_line = true;
		auto line_color = engine::flinear_color( 1.f, 0.f, 0.f, 1.f );
		bool engine_prediction = true;
		bool target_dot = false;
		auto dot_color = engine::math::from_rgb( 188, 26, 66 );
		int aimbone = 0;
		bool only_visible = false;
		bool ignore_bots = true;
		bool ignore_dbno = true;
		bool deadzone = false;
		float max_distance = 600.f;
		float fov_radius = 80.f;
		float smoothing = 3.f;
		bool trigger_bot = false;
		bool trigger_key = false;
		bool shotgun_only = false;
		bool crosshair = true;
		auto crosshair_color = engine::math::from_rgb( 151, 25, 115 );
		bool hide_reticle = true;
		float crosshair_size = 7.f;
		float restriction = 6.f;
		float triggerbot_time = 2.f;
		float trigger_distance = 100.f;
		int triggerbot_key = 0;
	}

	namespace visuals {
		bool enabled = true;
		bool box_esp = true;
		auto box_color = engine::math::from_rgb( 209, 110, 244 );
		auto invis_box_color = engine::math::from_rgb( 76, 110, 244 );
		int box_type = 0;
		bool outline = false;
		auto outline_color = engine::math::from_rgb( 140, 73, 163 );
		auto invis_outline_color = engine::math::from_rgb( 51, 73, 163 );
		bool snaplines = false;
		auto line_color = engine::math::from_rgb( 209, 110, 244 );
		auto invis_line_color = engine::math::from_rgb( 76, 110, 244 );
		int line_type = 0;
		bool filled_box = true;
		auto filled_color = engine::math::from_rgb( 136, 68, 188 );
		auto invis_filled_color = engine::math::from_rgb( 136, 68, 188 );
		bool skeleton = false;
		auto skeleton_color = engine::math::from_rgb( 225, 56, 237 );
		auto invis_skeleton_color = engine::math::from_rgb( 76, 110, 244 );
		bool name = true;
		bool platform = true;
		bool weapon = true;
		bool ammo_bar = true;
		bool damage_esp = true;
		auto damage_color = engine::math::from_rgb( 235, 20, 73 );
		bool inventory_esp = false;
		bool specator_esp = false;
		auto specator_color = engine::math::from_rgb( 223, 116, 44 );
		bool kill_feed = false;
		auto kill_feed_color = engine::math::from_rgb( 204, 48, 58 );
		bool minimap_esp = false;
		auto minimap_color = engine::math::from_rgb( 211, 81, 23 );
		bool kills = false;
		auto kills_color = engine::math::from_rgb( 235, 20, 73 );
		bool storm_prediction = false;
		auto storm_color = engine::math::from_rgb( 193, 28, 230 );
		bool season_level = false;
		auto level_color = engine::math::from_rgb( 209, 135, 57 );
		bool distance = true;
		bool bullet_tracers = false;
		auto tracer_color = engine::math::from_rgb( 117, 53, 184 );
		bool hit_marker = true;
		auto marker_color = engine::math::from_rgb( 161, 26, 122 );
		float tracer_time = 2.0f;
		bool view_direction = false;
		auto view_color = engine::math::from_rgb( 134, 49, 213 );
		auto invis_view_color = engine::math::from_rgb( 53, 101, 232 );
		float view_distance = 180.0;
		bool view_cone = false;
		float view_cone_radius = 100.0f;
		auto view_cone_color = engine::math::from_rgb( 165, 63, 172 );
		bool rank_esp = false;
		bool fov_arrows = true;
		bool radar = true;
		bool fov_lines = true;
		auto fov_line_color = engine::math::from_rgb( 207, 67, 126 );
		bool local_player = true;
		auto local_color = engine::math::from_rgb( 232, 93, 162 );
		bool radar_distance = false;
		float scope = 100.f;
		float radar_size = 200.f;
		auto text_color = engine::math::from_rgb( 255, 220, 245 );
		auto invis_text_color = engine::math::from_rgb( 117, 143, 246 );
		auto team_color = engine::math::from_rgb( 209, 135, 57 );
		auto knocked_color = engine::math::from_rgb( 53, 196, 110 );
		auto bot_color = engine::math::from_rgb( 235, 75, 75 );
	}

	namespace world {
		bool enabled = true;
		bool loot_esp = false;
		bool loot_icons = false;
		int loot_tier = 0;
		float loot_distance = 200.f;
		bool chest_esp = false;
		bool chest_chams = false;
		float chest_distance = 300.f;
		bool weakspot_esp = false;
		auto weakspot_color = engine::math::from_rgb( 170, 0, 1 );
		bool weakspot_aimbot = true;
		bool vehicle_esp = false;
		auto vehicle_color = engine::math::from_rgb( 34, 154, 239 );
		bool vehicle_health = true;
		auto health_color = engine::math::from_rgb( 57, 209, 117 );
		bool vehicle_chams = false;
		float vehicle_distance = 400.f;
		float weakspot_distance = 10.0;
		bool effects = false;
		bool preformence_mode = false;
		bool night_mode = false;
		bool disable_fog = false;
		bool disable_shadows = false;
		bool sky_light = false;
		float sky_light_multiplier = 1.f;
	}

	namespace exploits {
		bool enabled = false;
		bool chams = false;
		bool crown_win = false;
		bool weapon_chams = false;
		bool wireframe = false;
		bool xray_chams = false;
		int chams_material = 0;
		auto chams_color = engine::flinear_color( 1.f, 0.4f, 1.f, 1.f );
		auto weapon_color = engine::flinear_color( 0.190463f, 0.737205f, 1.f, 2.f );
		float chams_emissive = 500.f;
		bool silent_aim = false;
		bool no_recoil = false;
		bool desync = false;
		bool player_fly = false;
		bool no_clip = false;
		bool bhop = false;
		bool super_jump = false;
		bool super_slide = false;
		bool rapid_fire = false;
		float fire_speed = 1.2f;
		bool hit_sound = false;
		int hit_sounds = 0;
		bool shoot_through_walls = false;
		bool disable_collision = false;
	}

	namespace misc {
		bool enabled = true;
		bool debug_objects = false;
		bool game_information = true;
		bool render_count = false;
		bool debug_targeting = true;
		int menu_key_index = 0;
	}
}