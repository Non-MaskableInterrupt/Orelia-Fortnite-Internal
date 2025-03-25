#pragma once

#define declare_member(type, name, offset) type name() { return *(type*)( this + offset ); } 
#define declare_member_bit(bit, name, offset) bool name( ) { return bool( *(char*)( this + offset) & (1 << bit)); }

#define apply_member(type, name, offset) void name( type val ) { *(type*)( this + offset ) = val; }
#define apply_member_bit(bit, name, offset) void name(bool value) { auto bitfield = *(char*)(this + offset); bitfield |= (value << bit); *(char*)(this + offset) = bitfield; }
#define invert_member_bit(bit, name, offset) void name(bool value) { auto bitfield = *(char*)(this + offset); bitfield &= ~(value << bit); *(char*)(this + offset) = bitfield; }

#define declare_member_array(type, name, size, offset) \
    type* name() { return (type*)(this + offset); } \
    type& name(int i) { return ((type*)(this + offset))[i]; }

#define apply_member_array(type, name, size, offset) \
    void name(type val, int i) { *(type*)(this + offset + (i * sizeof(type))) = val; }

namespace engine {
	namespace math {
		frotator normalize_euler_angles( frotator& result );
		float arctan_reciprocal_to_degrees( double half_tangent );
	}

	class c_camera_info {
	public:
		c_camera_info(
			fvector camera_location,
			frotator camera_rotation,
			float field_of_view
		) :
			m_camera_location( camera_location ),
			m_camera_rotation( math::normalize_euler_angles( camera_rotation ) ),
			m_field_of_view( math::arctan_reciprocal_to_degrees( field_of_view ) ) {
		}

		fvector get_location( ) const {
			return m_camera_location;
		}

		frotator get_rotation( ) const {
			return m_camera_rotation;
		}

		float get_fov( ) const {
			return m_field_of_view;
		}

		float get_aimbot_fov( double radius, double screen_width ) const {
			return ( radius * screen_width / m_field_of_view ) / 2.0;
		}

	private:
		fvector m_camera_location;
		frotator m_camera_rotation;
		double m_field_of_view;
		float m_aimbot_fov;
	};

	class u_object {
	public:
		declare_member( std::uintptr_t, vtable, 0x0 );
		declare_member( e_object_flags, object_flags, 0x8 );
		declare_member( std::uint32_t, internal_index, 0xc );
		declare_member( u_object*, class_private, 0x10 );
		declare_member( fname, name_private, 0x18 );
		declare_member( u_object*, outer_private, 0x20 );

		void process_event( u_function* fn, void* params ) {
			if ( auto vtable = *( void*** )this ) {
				reinterpret_cast< void( __cdecl* )( u_object*, u_function*, void* ) >( vtable[ offsets::process_event ] )( this, fn, params );
			}
		}

		static u_object* static_find_object( u_object* fn_class, u_object* outer, const wchar_t* name, bool exact_class ) {
			return reinterpret_cast< u_object * ( __cdecl* )( u_object*, u_object*, const wchar_t*, bool ) >( orelia::base_address + offsets::static_find_object )( fn_class, outer, name, exact_class );
		}

		template <class type>
		static type find_object( const wchar_t* name, u_object* outer = nullptr ) {
			return reinterpret_cast< type >( u_object::static_find_object( nullptr, outer, name, false ) );
		}

		static void static_construct_object_internal( u_object* obj ) {
			reinterpret_cast< void( __cdecl* )( u_object* ) >(
				orelia::base_address + offsets::static_construct_object_internal
				)( obj );
		}

		static u_object* static_allocate_object(
			u_class* class_obj,
			u_object* outer,
			const fname& name,
			e_object_flags flags,
			e_internal_object_flags internal_flags,
			bool b_set_default,
			u_object* template_obj,
			bool b_copy_transients
		) {
			return reinterpret_cast< u_object * ( __cdecl* )(
				u_class*, u_object*, const fname&,
				e_object_flags, e_internal_object_flags,
				bool, u_object*, bool
				) >( orelia::base_address + offsets::static_allocate_object )(
					class_obj, outer, name, flags,
					internal_flags, b_set_default,
					template_obj, b_copy_transients
					);
		}

		template <class type>
		type* load_object(
			const wchar_t* command,
			a_player_controller* player_controller
		);

		fstring get_object_name( );
		bool object_is_a( u_class* object_class );

		template <class t>
		tarray<t> get_actors_of_class( u_class* object_class );
	};

	class u_texture_2d : public u_object {
	public:
	};

	class u_class : public u_object {
	public:
	};

	class u_blueprint_function_library : public u_object {
	public:
	};

	class u_sound_base : public u_object {
	public:
	};

	class u_texture : public u_object {
	public:
	};

	class u_particle_system : public u_object {
	public:
	};

	class f_memory : public u_object {
	public:
		static uintptr_t malloc( int count, uint32_t alignment ) {
			return reinterpret_cast< uintptr_t( __cdecl* )( uintptr_t, int ) >( orelia::base_address + offsets::malloc )( count, alignment );
		}

		static void free( void* original ) {
			reinterpret_cast< void( __cdecl* )( void* ) >( orelia::base_address + offsets::free )( original );
		}
	};

	class u_canvas : public u_object {
	public:
		declare_member( float, clip_x, 0x30 );
		declare_member( float, clip_y, 0x34 );
		declare_member( u_texture*, default_texture, 0x70 );

		void k2_draw_texture(
			u_texture* render_texture,
			fvector2d screen_position,
			fvector2d screen_size,
			fvector2d coordinate_position,
			fvector2d coordinate_size,
			flinear_color render_color,
			e_blend_mode blend_mode,
			float rotation,
			fvector2d pivot_point
		) {
			static u_function* function = 0;
			if ( !function ) {
				function = find_object< u_function* >( encrypt( L"Canvas.K2_DrawTexture" ) );
			}

			struct {
				u_texture* render_texture;
				fvector2d screen_position;
				fvector2d screen_size;
				fvector2d coordinate_position;
				fvector2d coordinate_size;
				flinear_color render_color;
				e_blend_mode blend_mode;
				float rotation;
				fvector2d pivot_point;
			} params{ render_texture, screen_position, screen_size, coordinate_position, coordinate_size, render_color, blend_mode, rotation, pivot_point };

			this->process_event( function, &params );
		}

		void k2_draw_line( fvector2d screen_position_a, fvector2d screen_position_b, float thickness, flinear_color render_color ) {
			static u_function* function = 0;
			if ( !function ) {
				function = find_object< u_function* >( encrypt( L"Canvas.K2_DrawLine" ) );
			}

			struct {
				fvector2d screen_position_a;
				fvector2d screen_position_b;
				float thickness;
				flinear_color render_color;
			} params{ screen_position_a, screen_position_b, thickness, render_color };

			this->process_event( function, &params );
		}

		void k2_draw_text( u_font* render_font, fstring render_text, fvector2d screen_position, double font_size, flinear_color render_color, bool b_centre_x, bool b_centre_y, bool b_outlined ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"Canvas.K2_DrawText" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				u_font* render_font;
				fstring render_text;
				fvector2d screen_position;
				fvector2d scale;
				flinear_color render_color;
				float kerning;
				flinear_color shadow_color;
				fvector2d shadow_offset;
				bool b_centre_x;
				bool b_centre_y;
				bool b_outlined;
				flinear_color outline_color;
			} params{ render_font, render_text, screen_position, fvector2d( 1.0, 1.0 ) , render_color , 0.f, flinear_color( ), fvector2d( ), b_centre_x , b_centre_y , b_outlined, flinear_color( 0.f, 0.f, 0.f, 1.f ) };

			this->process_event( function, &params );
		}

		void k2_draw_polygon( u_texture* render_texture, fvector2d screen_position, fvector2d radius, int32_t number_of_sides, flinear_color render_color ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"Canvas.K2_DrawPolygon" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				u_texture* render_texture;
				fvector2d screen_position;
				fvector2d radius;
				int32_t number_of_sides;
				flinear_color render_color;
			} params{ render_texture, screen_position, radius, number_of_sides, render_color };

			this->process_event( function, &params );
		}

		fvector k2_project( fvector world_location ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"Canvas.k2_Project" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				fvector world_location;
				fvector return_value;
			} params{ world_location };

			this->process_event( function, &params );

			return params.return_value;
		}

		fvector2d k2_text_size( u_font* render_font, fstring render_text, fvector2d scale ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"Canvas.K2_TextSize" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				u_font* render_font;
				fstring render_text;
				fvector2d scale;
				fvector2d return_value;
			} params{ render_font, render_text, scale };

			this->process_event( function, &params );

			return params.return_value;
		}

		fvector k2_deproject( fvector2d screen_position, fvector world_origin, fvector world_direction ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"Canvas.k2_Deproject" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				fvector2d screen_position;
				fvector world_origin;
				fvector world_direction;
				fvector return_value;
			} params{ screen_position, world_origin, world_direction };

			this->process_event( function, &params );

			return params.return_value;
		}
	};

	class u_material_interface : public u_object {
	public:
	};

	class u_material_instance : public u_material_interface {
	public:
	};

	class u_material_instance_dynamic : public u_material_instance {
	public:
		void copy_parameter_overrides( u_material_instance* material_instance ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"MaterialInstanceDynamic.CopyParameterOverrides" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				u_material_instance* material_instance;
			} params{ material_instance };

			this->process_event( function, &params );
		}

		void set_vector_parameter_value( fname parameter_name, flinear_color value ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"MaterialInstanceDynamic.SetVectorParameterValue" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				fname parameter_name;
				flinear_color value;
			} params{ parameter_name, value };

			this->process_event( function, &params );
		}

		void set_scalar_parameter_value( fname parameter_name, float value ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"MaterialInstanceDynamic.SetScalarParameterValue" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				fname parameter_name;
				float value;
			} params{ parameter_name, value };

			this->process_event( function, &params );
		}
	};

	class u_material : public u_material_interface {
	public:
		apply_member( e_blend_mode, blend_mode, 0x129 );
		apply_member_bit( 0, b_disable_depth_test, 0x1c8 );
		invert_member_bit( 0, reset_b_disable_depth_test, 0x1c8 );
		apply_member_bit( 6, wireframe, 0x1d8 );
		invert_member_bit( 6, reset_wireframe, 0x1d8 );
		apply_member_bit( 4, two_sided, 0x190 );
		invert_member_bit( 4, reset_two_sided, 0x190 );
		apply_member_bit( 10, b_use_emissive_for_dynamic_area_lighting, 0x1c8 );
	};

	class u_actor_component : public u_object {
	public:
		void set_is_replicated( bool should_replicate ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"ActorComponent.SetIsReplicated" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				bool should_replicate;
			} params{ should_replicate };

			this->process_event( function, &params );
		}
	};

	class u_scene_component : public u_actor_component {
	public:
		fvector get_forward_vector( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"SceneComponent.GetForwardVector" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				fvector return_value;
			} params{ };

			this->process_event( function, &params );

			return params.return_value;
		}

		fvector get_socket_location( fname in_socket_name ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"SceneComponent.GetSocketLocation" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				fname in_socket_name;
				fvector return_value;
			} params{ in_socket_name };

			this->process_event( function, &params );

			return params.return_value;
		}

		fvector get_bone_location( const wchar_t* socket_name );
	};

	class u_primitive_component : public u_scene_component {
	public:
		apply_member_bit( 3, b_use_as_occluder, 0x26d );
		invert_member_bit( 3, invert_use_as_occluder, 0x26d );

		void set_collision_response_to_channel( e_collision_channel channel, e_collision_response new_response ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"PrimitiveComponent.SetCollisionResponseToChannel" );
				function = find_object<u_function*>( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				e_collision_channel channel;
				e_collision_response new_response;
			} params{ channel, new_response };

			this->process_event( function, &params );
		}

		void set_custom_depth_stencil_value( int value ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"PrimitiveComponent.SetCustomDepthStencilValue" );
				function = find_object<u_function*>( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				int value;
			} params{ value };

			this->process_event( function, &params );
		}

		void set_render_custom_depth( bool b_value ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"PrimitiveComponent.SetRenderCustomDepth" );
				function = find_object<u_function*>( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				bool b_value;
			} params{ b_value };

			this->process_event( function, &params );
		}

		void invalidate_lumen_surface_cache( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"PrimitiveComponent.InvalidateLumenSurfaceCache" );
				function = find_object<u_function*>( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
			} params;

			this->process_event( function, &params );
		}

		void set_render_in_main_pass( bool value ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"PrimitiveComponent.SetRenderInMainPass" );
				function = find_object<u_function*>( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				bool bValue;
			} params;
			params.bValue = value;

			this->process_event( function, &params );
		}

		bool get_render_custom_depth( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"PrimitiveComponent.GetRenderCustomDepth" );
				function = find_object<u_function*>( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				bool ReturnValue;
			} params;

			this->process_event( function, &params );
			return params.ReturnValue;
		}

		u_material_instance_dynamic* create_dynamic_material_instance(
			int32_t element_index,
			u_material_interface* parent,
			fname optional_name
		) {
			static u_function* function = nullptr;
			if ( !function ) {
				auto fn_name = encrypt( L"PrimitiveComponent.CreateDynamicMaterialInstance" );
				function = find_object<u_function*>( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				int32_t element_index;
				u_material_interface* parent;
				fname optional_name;
				u_material_instance_dynamic* return_value;
			} params{ element_index, parent, optional_name };

			this->process_event( function, &params );

			return params.return_value;
		}

		u_material_interface* get_material( int32_t element_index ) {
			static u_function* function = nullptr;
			if ( !function ) {
				auto fn_name = encrypt( L"PrimitiveComponent.GetMaterial" );
				function = find_object<u_function*>( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				int32_t element_index;
				u_material_interface* return_value;
			} params{ element_index };

			this->process_event( function, &params );

			return params.return_value;
		}

		void  set_material( int32_t element_index, u_material_interface* material ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"PrimitiveComponent.SetMaterial" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				int32_t element_index;
				u_material_interface* material;
			} params{ element_index , material };

			this->process_event( function, &params );
		}
	};

	class u_mesh_component : public u_primitive_component {
	public:
		tarray<u_material_interface*> get_materials( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"MeshComponent.GetMaterials" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				tarray<u_material_interface*> return_value;
			} params;

			this->process_event( function, &params );

			return params.return_value;
		}
	};

	class u_base_building_static_mesh_component : public u_mesh_component {
	public:
	};

	class u_skinned_mesh_component : public u_mesh_component {
	public:
	};

	class u_skeletal_mesh_component : public u_skinned_mesh_component {
	public:
		declare_member( fbox_sphere_bounds, get_bounds, 0x100 );
	};

	class a_actor : public u_object {
	public:
		declare_member( u_scene_component*, root_component, 0x1b0 );
		declare_member( e_net_role, role, 0x164 );
		declare_member( e_net_role, remote_role, 0x60 );
		declare_member( e_net_dormancy, net_dormancy, 0x165 );
		declare_member_bit( 3, b_always_relevant, 0x58 );
		declare_member_bit( 3, b_replicates, 0x5b );
		declare_member_bit( 4, b_replicate_movement, 0x58 );
		apply_member( e_net_role, role, 0x164 );
		apply_member( e_net_role, remote_role, 0x60 );
		apply_member( e_net_dormancy, net_dormancy, 0x165 );
		apply_member_bit( 3, b_always_relevant, 0x58 );
		apply_member_bit( 3, b_replicates, 0x5b );
		apply_member_bit( 4, b_replicate_movement, 0x58 );

		void set_actor_enable_collision( bool b_new_actor_enable_collision ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"Actor.SetActorEnableCollision" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				bool b_new_actor_enable_collision;
			} params{ b_new_actor_enable_collision };

			this->process_event( function, &params );
		}

		float get_input_axis_key_value( fkey input_axis_key ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"Actor.GetInputAxisKeyValue" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				fkey input_axis_key;
				float return_value;
			} params{ input_axis_key };

			this->process_event( function, &params );

			return params.return_value;
		}

		frotator k2_get_actor_rotation( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"Actor.K2_GetActorRotation" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				frotator return_value;
			} params;

			this->process_event( function, &params );

			return params.return_value;
		}

		fvector k2_get_actor_location( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"Actor.K2_GetActorLocation" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				fvector return_value;
			} params;

			this->process_event( function, &params );

			return params.return_value;
		}

		float get_distance_to( a_actor other_actor ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"Actor.GetDistanceTo" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
			} params;

			this->process_event( function, &params );
		}

		void force_net_update( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"Actor.ForceNetUpdate" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
			} params;

			this->process_event( function, &params );
		}

		void flush_net_dormancy( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"Actor.FlushNetDormancy" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
			} params;

			this->process_event( function, &params );
		}

		void prestream_textures( float seconds, bool b_enable_streaming, int32_t cinematic_texture_groups ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"Actor.PrestreamTextures" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				float seconds;
				bool b_enable_streaming;
				int32_t cinematic_texture_groups;
			} params;

			this->process_event( function, &params );
		}

		fvector get_velocity( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"Actor.GetVelocity" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				fvector return_value;
			} params;

			this->process_event( function, &params );

			return params.return_value;
		}
	};

	class u_item_definition_base : public u_object {
	public:
		declare_member( ftext, item_name, 0x40 );
	};

	class u_fort_item_definition : public u_item_definition_base {
	public:
		declare_member( e_fort_item_type, item_type, 0xa0 );
		declare_member( e_fort_rarity, rarity, 0xa2 );

		int32_t get_max_num_stacks( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortItemDefinition.GetMaxNumStacks" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				int32_t return_value;
			} params;

			this->process_event( function, &params );

			return params.return_value;
		}

		bool is_stackable( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortItemDefinition.IsStackable" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				bool return_value;
			} params;

			this->process_event( function, &params );

			return params.return_value;
		}

		t_soft_object_ptr<u_texture_2d> get_small_preview_image( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortItemDefinition.GetSmallPreviewImage" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				t_soft_object_ptr<u_texture_2d> return_value;
			} params;

			this->process_event( function, &params );

			return params.return_value;
		}
	};

	class a_building_weakspot : public a_actor {
	public:
		declare_member_bit( 0, b_hit, 0x2d8 );
		declare_member_bit( 2, b_active, 0x2d8 );
	};

	class a_building_sm_actor : public a_actor {
	public:
		declare_member( u_base_building_static_mesh_component*, static_mesh_component, 0x8c8 );
	};

	class a_building_container : public a_building_sm_actor {
	public:
		declare_member_bit( 2, b_already_searched, 0xe02 );
	};

	class a_fort_pickup : public a_actor {
	public:
		declare_member( f_fort_item_entry, primary_pickup_item_entry, 0x370 );
	};

	class u_fort_world_item_definition : public u_fort_item_definition {
	public:
	};

	class u_fort_weapon_item_definition : public u_fort_world_item_definition {
	public:
	};

	class u_fort_weapon_ranged_item_definition : public u_fort_weapon_item_definition {
	public:
		bool is_projectile_weapon( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortWeaponRangedItemDefinition.IsProjectileWeapon" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				bool return_value;
			} params;

			this->process_event( function, &params );

			return params.return_value;
		}
	};

	class u_fort_item : u_object {
	public:
		int32_t get_total_ammo( ) {
			static u_function* function = nullptr;
			if ( !function ) {
				auto fn_name = encrypt( L"FortItem.GetTotalAmmo" );
				function = find_object<u_function*>( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				int32_t return_value;
			} params{ };

			this->process_event( function, &params );

			return params.return_value;
		}

		int32_t get_num_in_stack( ) {
			static u_function* function = nullptr;
			if ( !function ) {
				auto fn_name = encrypt( L"FortItem.GetNumInStack" );
				function = find_object<u_function*>( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				int32_t return_value;
			} params{ };

			this->process_event( function, &params );

			return params.return_value;
		}

		ftext get_item_type_name( bool b_use_pural ) {
			static u_function* function = nullptr;
			if ( !function ) {
				auto fn_name = encrypt( L"FortItem.GetItemTypeName" );
				function = find_object<u_function*>( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				bool b_use_pural;
				ftext return_value;
			} params{ b_use_pural };

			this->process_event( function, &params );

			return params.return_value;
		}
	};

	class u_fort_world_item : public u_fort_item {
	public:

	};

	struct f_quick_bar {
		int32_t current_focused_slot;
		int32_t previous_focused_slot;
		int32_t secondary_focused_slot;
		bool has_been_focused_before;
		tarray<f_quick_bar_slot*> slots;
		f_quick_bar_data data_definition;
		t_set<u_fort_item_definition*> equipped_item_definitions;
		tarray<int32_t> shared_visible_slot_indices_when_using_gamepad;
		tarray<int32_t> hidden_slot_indices;
	};

	class a_fort_quick_bars : public a_actor {
	public:
		declare_member( f_quick_bar, primary_quick_bar, 0x2b0 );
		declare_member( f_quick_bar, secondary_quick_bar, 0x350 );
		declare_member( f_quick_bar, creative_quick_bar, 0x3f0 );
	};

	class a_fort_damage_numbers_actor : public a_actor {
	public:
		declare_member( float, component_lifespan, 0x2b0 );
		declare_member( fname, player_material_parameter_name, 0x300 );
		declare_member( float, player_material_parameter_value, 0x304 );
		declare_member( fname, color_parameter_name, 0x308 );
		declare_member( flinear_color, hit_player_color, 0x30c );
		declare_member( flinear_color, critical_hit_player_color, 0x31c );
		declare_member( flinear_color, hit_enemy_color, 0x32c );
		declare_member( flinear_color, critical_hit_enemy_color, 0x33c );
		declare_member( flinear_color, hit_building_color, 0x34c );
		declare_member( flinear_color, critical_hit_building_color, 0x35c );
		declare_member( flinear_color, shield_hit_color, 0x36c );
		declare_member( flinear_color, critical_hit_shield_color, 0x37c );
		declare_member( tarray<f_fort_damage_number_color_info*>, damage_number_color_infos, 0x390 );
		declare_member( fname, animation_lifespan_parameter_name, 0x3a0 );
		declare_member( fname, is_critical_hit_parameter_name, 0x3a4 );
		declare_member( float, spacing_percentage_for_ones, 0x3a8 );
		declare_member( tarray<fname>, position_parameter_names, 0x3b0 );
		declare_member( float, distance_from_camera_before_doubling_size, 0x3c0 );
		declare_member( float, critical_hit_size_multiplier, 0x3c4 );
		declare_member( float, font_x_size, 0x3c8 );
		declare_member( float, font_y_size, 0x3cc );
		declare_member( fvector, world_location_offset, 0x3d0 );
		declare_member( float, number_of_number_rotations, 0x3e8 );
		declare_member( tarray<fname>, scale_rotation_angle_parameter_names, 0x3f0 );
		declare_member( tarray<fname>, duration_parameter_names, 0x400 );
		declare_member( float, max_score_number_distance, 0x410 );
		declare_member( fname, move_to_camera_parameter_name, 0x414 );
		declare_member( tarray<flinear_color*>, percent_damage_colors, 0x418 );
		declare_member( tarray<int32_t>, percent_damage_thresholds, 0x428 );
		declare_member( int32_t, custom_stencil_depth_value, 0x438 );
		declare_member( float, custom_bounds_scale, 0x43c );
	};

	class a_fort_ranged_weapon : public a_actor {
	public:
		void enable_alt_center_reticle( bool new_enabled ) {
			static u_function* function = nullptr;
			if ( !function ) {
				auto fn_name = encrypt( L"FortWeaponRanged.EnableAltCenterReticle" );
				function = find_object<u_function*>( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				bool new_enabled;
			} params{ new_enabled };

			this->process_event( function, &params );
		}

		void enable_alt_outer_reticle( bool new_enabled ) {
			static u_function* function = nullptr;
			if ( !function ) {
				auto fn_name = encrypt( L"FortWeaponRanged.EnableAltOuterReticle" );
				function = find_object<u_function*>( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				bool new_enabled;
			} params{ new_enabled };

			this->process_event( function, &params );
		}

		void enable_corners_reticle( bool new_enabled ) {
			static u_function* function = nullptr;
			if ( !function ) {
				auto fn_name = encrypt( L"FortWeaponRanged.EnableCornersReticle" );
				function = find_object<u_function*>( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				bool new_enabled;
			} params{ new_enabled };

			this->process_event( function, &params );
		}

		void enable_crosshair( bool enabled ) {
			this->enable_corners_reticle( !enabled );
			this->enable_alt_outer_reticle( !enabled );
			this->enable_alt_center_reticle( !enabled );
		}
	};

	class u_skeletal_mesh_component_budgeted : public u_skeletal_mesh_component {
	public:

	};

	class a_fort_weapon : public a_fort_ranged_weapon {
	public:
		declare_member( u_fort_weapon_item_definition*, weapon_data, 0x570 );
		declare_member( e_weapon_core_animation, weapon_core_animation, 0x15e8 );
		declare_member( u_particle_system, impact_physical_surface_effects, 0xb40 );
		declare_member( tarray<u_skeletal_mesh_component_budgeted*>, all_weapon_meshes, 0xdf8 );
		declare_member( f_fort_weapon_ramping_data, ramping_fire_rate_data, 0x1e78 );
		declare_member( float, projectile_gravity, offsets::projectile_gravity );
		declare_member( float, projectile_speed, offsets::projectile_speed );
		apply_member( u_sound_base*, primary_fire_sound_1p, 0x2c0 );
		apply_member( u_sound_base*, primary_fire_stop_sound_1p, 0x980 );
		apply_member( u_sound_base*, targeting_start_sound, 0xa00 );
		apply_member( u_sound_base*, targeting_end_sound, 0xa08 );
		apply_member_array( u_sound_base*, impact_physical_surface_sounds, 0xa20, 0x9C8 );
		apply_member_array( u_sound_base*, primary_fire_sound, 3, 0x2d0 );
		apply_member_array( u_sound_base*, primary_fire_stop_sound, 3, 0x980 );
		apply_member_array( u_sound_base*, secondary_fire_sound, 3, 0x9a0 );
		apply_member_array( u_sound_base*, secondary_fire_stop_sound, 3, 0x9b8 );
		apply_member_array( u_sound_base*, charge_fire_sound_1p, 3, 0x9d0 );
		apply_member_array( u_sound_base*, charge_fire_sound, 3, 0x9e8 );

		bool is_holding_shotgun( ) {
			return weapon_core_animation( ) == e_weapon_core_animation::shotgun;
		}

		u_fort_item* get_inventory_item( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortWeapon.GetInventoryItem" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				u_fort_item* return_value;
			} params;

			this->process_event( function, &params );

			return params.return_value;
		}

		float get_time_to_next_fire( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortWeapon.GetTimeToNextFire" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				float return_value;
			} params;

			this->process_event( function, &params );

			return params.return_value;
		}

		float get_range( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortWeapon.GetRange" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				float return_value;
			} params;

			this->process_event( function, &params );

			return params.return_value;
		}

		bool is_firing( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortWeapon.IsFiring" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				bool return_value;
			} params;

			this->process_event( function, &params );

			return params.return_value;
		}

		bool can_fire( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortWeapon.CanFire" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				bool return_value;
			} params;

			this->process_event( function, &params );

			return params.return_value;
		}

		fvector get_muzzle_location( int pattern_index ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortWeapon.GetMuzzleLocation" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				int pattern_index;
				fvector return_value;
			} params{ pattern_index };

			this->process_event( function, &params );

			return params.return_value;
		}

		fvector get_targeting_source_location( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortWeapon.GetTargetingSourceLocation" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				fvector return_value;
			} params;

			this->process_event( function, &params );

			return params.return_value;
		}

		int32_t get_magazine_ammo_count( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortWeapon.GetMagazineAmmoCount" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				int32_t return_value;
			} params;

			this->process_event( function, &params );

			return params.return_value;
		}

		int32_t get_bullets_per_clip( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortWeapon.GetBulletsPerClip" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				int32_t return_value;
			} params;

			this->process_event( function, &params );

			return params.return_value;
		}
	};

	class u_movement_component : public u_actor_component {
	public:
		declare_member( u_scene_component*, updated_component, 0xB8 );
		declare_member( u_primitive_component*, updated_primitive, 0xc0 );
		declare_member( fvector, velocity, 0xD0 );
		declare_member( fvector, plane_constraint_normal, 0xe8 );
		declare_member( fvector, plane_constraint_origin, 0x100 );
	};

	class u_nav_covement_component : public u_movement_component {
	public:
	};

	class u_pawn_movement_component : public u_nav_covement_component {
	public:
		void add_input_vector( fvector world_vector, bool force = false ) {
			static u_function* function = nullptr;
			if ( !function ) {
				auto fn_name = encrypt( L"PawnMovementComponent.AddInputVector" );
				function = find_object<u_function*>( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				fvector world_vector;
				bool force;
			} params{ world_vector, force };

			this->process_event( function, &params );
		}
	};

	class u_character_movement_component : public u_pawn_movement_component {
	public:
		declare_member( float, gravity_scale, 0x198 );
		declare_member( float, max_step_height, 0x19c );
		declare_member( float, jump_z_velocity, 0x1a0 );
		declare_member( float, jump_off_jump_z_factor, 0x1a4 );
		declare_member( float, walkable_floor_angle, 0x1c4 );
		declare_member( float, walkable_floor_z, 0x1c8 );
		declare_member( fvector, gravity_direction, 0x1d0 );
		declare_member( fquat, world_to_gravity_transform, 0x1f0 );
		declare_member( fquat, gravity_to_world_transform, 0x210 );
		declare_member( uint8_t, movement_mode, 0x231 );
		declare_member( uint8_t, custom_movement_mode, 0x232 );
		declare_member( float, ground_friction, 0x234 );
		declare_member( float, max_walk_speed, 0x278 );
		declare_member( float, max_walk_speed_crouched, 0x27c );
		declare_member( float, max_swim_speed, 0x280 );
		declare_member( float, max_fly_speed, 0x284 );
		declare_member( float, max_custom_movement_speed, 0x288 );
		declare_member( float, max_acceleration, 0x28c );
		declare_member( float, min_analog_walk_speed, 0x290 );
		declare_member( float, braking_friction_factor, 0x294 );
		declare_member( float, braking_friction, 0x298 );
		declare_member( float, braking_sub_step_time, 0x29c );
		declare_member( float, braking_deceleration_walking, 0x2a0 );
		declare_member( float, braking_deceleration_falling, 0x2a4 );
		declare_member( float, braking_deceleration_swimming, 0x2a8 );
		declare_member( float, braking_deceleration_flying, 0x2ac );
		declare_member( float, air_control, 0x2b0 );
		declare_member( float, air_control_boost_multiplier, 0x2b4 );
		declare_member( float, air_control_boost_velocity_threshold, 0x2b8 );
		declare_member( float, falling_lateral_friction, 0x2bc );
		declare_member( float, crouched_half_height, 0x2c0 );
		declare_member( float, buoyancy, 0x2c4 );
		declare_member( float, perch_radius_threshold, 0x2c8 );
		declare_member( float, perch_additional_height, 0x2cc );
		declare_member( frotator, rotation_rate, 0x2d0 );
		declare_member_bit( 0, b_use_separate_braking_friction, 0x2e8 );
		declare_member_bit( 1, b_dont_fall_below_jump_z_velocity_during_jump, 0x2e8 );
		declare_member_bit( 2, b_apply_gravity_while_jumping, 0x2e8 );
		declare_member_bit( 3, b_use_controller_desired_rotation, 0x2e8 );
		declare_member_bit( 4, b_orient_rotation_to_movement, 0x2e8 );
		declare_member_bit( 5, b_sweep_while_nav_walking, 0x2e8 );
		declare_member_bit( 7, b_movement_in_progress, 0x2e8 );
		apply_member( float, gravity_scale, 0x198 );
		apply_member( float, max_step_height, 0x19c );
		apply_member( float, jump_z_velocity, 0x1a0 );
		apply_member( float, jump_off_jump_z_factor, 0x1a4 );
		apply_member( float, walkable_floor_angle, 0x1c4 );
		apply_member( float, walkable_floor_z, 0x1c8 );
		apply_member( fvector, gravity_direction, 0x1d0 );
		apply_member( fquat, world_to_gravity_transform, 0x1f0 );
		apply_member( fquat, gravity_to_world_transform, 0x210 );
		apply_member( uint8_t, movement_mode, 0x231 );
		apply_member( uint8_t, custom_movement_mode, 0x232 );
		apply_member( float, ground_friction, 0x234 );
		apply_member( float, max_walk_speed, 0x278 );
		apply_member( float, max_walk_speed_crouched, 0x27c );
		apply_member( float, max_swim_speed, 0x280 );
		apply_member( float, max_fly_speed, 0x284 );
		apply_member( float, max_custom_movement_speed, 0x288 );
		apply_member( float, max_acceleration, 0x28c );
		apply_member( float, min_analog_walk_speed, 0x290 );
		apply_member( float, braking_friction_factor, 0x294 );
		apply_member( float, braking_friction, 0x298 );
		apply_member( float, braking_sub_step_time, 0x29c );
		apply_member( float, braking_deceleration_walking, 0x2a0 );
		apply_member( float, braking_deceleration_falling, 0x2a4 );
		apply_member( float, braking_deceleration_swimming, 0x2a8 );
		apply_member( float, braking_deceleration_flying, 0x2ac );
		apply_member( float, air_control, 0x2b0 );
		apply_member( float, air_control_boost_multiplier, 0x2b4 );
		apply_member( float, air_control_boost_velocity_threshold, 0x2b8 );
		apply_member( float, falling_lateral_friction, 0x2bc );
		apply_member( float, crouched_half_height, 0x2c0 );
		apply_member( float, buoyancy, 0x2c4 );
		apply_member( float, perch_radius_threshold, 0x2c8 );
		apply_member( float, perch_additional_height, 0x2cc );
		apply_member( frotator, rotation_rate, 0x2d0 );
		apply_member_bit( 0, b_use_separate_braking_friction, 0x2e8 );
		apply_member_bit( 1, b_dont_fall_below_jump_z_velocity_during_jump, 0x2e8 );
		apply_member_bit( 2, b_apply_gravity_while_jumping, 0x2e8 );
		apply_member_bit( 3, b_use_controller_desired_rotation, 0x2e8 );
		apply_member_bit( 4, b_orient_rotation_to_movement, 0x2e8 );
		apply_member_bit( 5, b_sweep_while_nav_walking, 0x2e8 );
		apply_member_bit( 7, b_movement_in_progress, 0x2e8 );
		declare_member_bit( 6, b_enable_physics_interaction, 0x2e8 );
		apply_member_bit( 6, b_enable_physics_interaction, 0x2e8 );
		declare_member( fvector, acceleration, 0x3A0 );
		apply_member( fvector, acceleration, 0x3A0 );
		declare_member( fvector, pending_input_vector, 0x3B0 );
		apply_member( fvector, pending_input_vector, 0x3B0 );
		declare_member_bit( 3, b_notify_apex_when_fall_height_reached, 0x2e9 );
		apply_member_bit( 3, b_notify_apex_when_fall_height_reached, 0x2e9 );
		declare_member_bit( 1, b_ignore_client_movement_error_checks_and_correction, 0x54d );
		apply_member_bit( 1, b_ignore_client_movement_error_checks_and_correction, 0x54d );
		declare_member_bit( 2, b_server_accept_client_authoritative_position, 0x54d );
		apply_member_bit( 2, b_server_accept_client_authoritative_position, 0x54d );
		declare_member_bit( 0, b_force_max_accel, 0x2e9 );
		apply_member_bit( 0, b_force_max_accel, 0x2e9 );
		declare_member_bit( 3, b_run_physics_with_no_controller, 0x2e9 );
		apply_member_bit( 3, b_run_physics_with_no_controller, 0x2e9 );
		declare_member_bit( 1, b_always_check_floor, 0x54e );
		apply_member_bit( 1, b_always_check_floor, 0x54e );
		declare_member_bit( 6, b_network_update_received, 0x54c );
		apply_member_bit( 6, b_network_update_received, 0x54c );
		declare_member_bit( 0, b_maintain_horizontal_ground_velocity, 0x54c );
		apply_member_bit( 0, b_maintain_horizontal_ground_velocity, 0x54c );
		declare_member_bit( 1, b_impart_base_velocity_x, 0x54c );
		apply_member_bit( 1, b_impart_base_velocity_x, 0x54c );
		declare_member_bit( 2, b_impart_base_velocity_y, 0x54c );
		apply_member_bit( 2, b_impart_base_velocity_y, 0x54c );
		declare_member_bit( 3, b_impart_base_velocity_z, 0x54c );
		apply_member_bit( 3, b_impart_base_velocity_z, 0x54c );
		declare_member_bit( 4, b_impart_base_angular_velocity, 0x54c );
		apply_member_bit( 4, b_impart_base_angular_velocity, 0x54c );
		declare_member_bit( 0, b_update_on_only_if_rendered, 0x118 );
		apply_member_bit( 0, b_update_on_only_if_rendered, 0x118 );
		declare_member_bit( 1, b_auto_update_tick_registration, 0x118 );
		apply_member_bit( 1, b_auto_update_tick_registration, 0x118 );
		declare_member_bit( 2, b_tick_before_owner, 0x118 );
		apply_member_bit( 2, b_tick_before_owner, 0x118 );
		declare_member_bit( 6, b_requested_move_use_acceleration, 0x54e );
		apply_member_bit( 6, b_requested_move_use_acceleration, 0x54e );
		apply_member( float, analog_input_modifier, 0x3d0 );
		apply_member( float, network_max_smooth_update_distance, 0x414 );
		apply_member( float, network_no_smooth_update_distance, 0x418 );
		apply_member( float, network_simulated_smooth_location_time, 0x3fc );
		apply_member( float, network_simulated_smooth_rotation_time, 0x400 );
		apply_member( uint8_t, network_smoothing_mode, 0x233 );

		bool is_falling( ) {
			return movement_mode( ) == 4;
		}

		void set_max_walk_speed( float new_speed ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"CharacterMovementComponent.SetMaxWalkSpeed" );
				function = find_object<u_function*>( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				float new_speed;
			} params{ new_speed };

			this->process_event( function, &params );
		}

		void set_max_fly_speed( float new_speed ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"CharacterMovementComponent.SetMaxFlySpeed" );
				function = find_object<u_function*>( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				float new_speed;
			} params{ new_speed };

			this->process_event( function, &params );
		}

		void set_gravity_scale( float new_scale ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"CharacterMovementComponent.SetGravityScale" );
				function = find_object<u_function*>( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				float new_scale;
			} params{ new_scale };

			this->process_event( function, &params );
		}

		void set_air_control( float new_control ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"CharacterMovementComponent.SetAirControl" );
				function = find_object<u_function*>( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				float new_control;
			} params{ new_control };

			this->process_event( function, &params );
		}

		void set_movement_mode( uint8_t new_mode, uint8_t new_custom_mode = 0 ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"CharacterMovementComponent.SetMovementMode" );
				function = find_object<u_function*>( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				uint8_t new_movement_mode;
				uint8_t new_custom_mode;
			} params{ new_mode, new_custom_mode };

			this->process_event( function, &params );
		}
	};

	class a_pawn : public a_actor {
	public:
		declare_member( a_fort_player_state_athena*, player_state, 0x2c8 );
		declare_member( a_player_controller*, controller, 0x2d8 );
		declare_member( a_player_controller*, previous_controller, 0x2E0 );
		declare_member( u_character_movement_component*, character_movement, 0x330 );
	};

	class a_building_actor : public a_actor {
	public:

	};

	class a_fort_physics_pawn : public a_actor {
	public:
	};

	class a_fort_athena_vehicle : public a_fort_physics_pawn {
	public:
		float get_health( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortAthenaVehicle.GetHealth" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				float return_value;
			} params{ };

			this->process_event( function, &params );

			return params.return_value;
		}

		float get_max_health( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortAthenaVehicle.GetMaxHealth" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				float return_value;
			} params{ };

			this->process_event( function, &params );

			return params.return_value;
		}

		bool set_vehicle_stencil_highlighted( a_actor* potential_vehicle, bool b_highlighted, int32_t stencil_value_override ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortAthenaVehicle.SetVehicleStencilHighlighted" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				a_actor* potential_vehicle;
				bool b_highlighted;
				int32_t stencil_value_override;
				bool return_value;
			} params{ potential_vehicle , b_highlighted , stencil_value_override };

			this->process_event( function, &params );

			return params.return_value;
		}

		bool can_contain_players( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortAthenaVehicle.CanContainPlayers" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				bool return_value;
			} params{ };

			this->process_event( function, &params );

			return params.return_value;
		}

		bool has_driver( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortAthenaVehicle.HasDriver" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				bool return_value;
			} params{ };

			this->process_event( function, &params );

			return params.return_value;
		}
	};

	class a_character : public a_pawn {
	public:
		declare_member( u_skeletal_mesh_component*, mesh, 0x328 );
	};

	class afgf_character : public a_character {
	public:
	};

	class a_fort_pawn : public afgf_character {
	public:
		declare_member( a_fort_weapon*, current_weapon, 0xa80 );
		declare_member( float, total_player_damage_dealt, 0x14d4 );
		declare_member( float, last_hit_time, 0x14d0 );
		declare_member( float, damage_taken_last_at_time, 0x14cc );
		declare_member( float, damage_taken_done_at_time, 0x14c8 );
		declare_member( int32_t, pawn_unique_id, 0xa7c );
		declare_member_bit( 2, b_is_invulnerable, 0x71a );

		void pawn_start_fire( char fire_mode_num ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortPawn.PawnStartFire" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				char fire_mode_num;
			} params{ fire_mode_num };

			this->process_event( function, &params );
		}

		void pawn_stop_fire( char fire_mode_num ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortPawn.PawnStopFire" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				char fire_mode_num;
			} params{ fire_mode_num };

			this->process_event( function, &params );
		}

		bool is_dead( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortPawn.IsDead" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				bool return_value;
			} params;

			this->process_event( function, &params );

			return params.return_value;
		}

		bool is_dbno( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortPawn.IsDBNO" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				bool return_value;
			} params;

			this->process_event( function, &params );

			return params.return_value;
		}
	};

	class a_fort_player_pawn : public a_fort_pawn {
	public:
		void server_set_aimbot_detection( bool b_enable_detection ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortPlayerPawn.ServerSetAimbotDetection" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				bool b_enable_detection;
			} params{ b_enable_detection };

			this->process_event( function, &params );
		}

		bool is_in_vehicle( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortPlayerPawn.IsInVehicle" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				bool return_value;
			} params;

			this->process_event( function, &params );

			return params.return_value;
		}
	};

	class a_player_camera_manager : public a_actor {
	public:

	};

	class a_controller : public a_actor {
	public:
		declare_member( a_pawn*, pawn, 0x2e8 );
		apply_member_bit( 1, b_attach_to_pawn, 0x338 );
		apply_member_bit( 7, b_can_possess_without_authority, 0x58 );

		frotator get_control_rotation( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"Controller.GetControlRotation" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				frotator return_value;
			} params{ };

			this->process_event( function, &params );

			return params.return_value;
		}

		void set_ignore_look_input( bool b_new_look_input ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"Controller.SetIgnoreLookInput" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				bool b_new_look_input;
			} params{ b_new_look_input };

			this->process_event( function, &params );
		}
	};

	class a_fort_safe_zone_indicator : public a_actor {
	public:
		a_fort_safe_zone_indicator* get( u_object* world_context_object ) {
			static u_function* function = nullptr;
			if ( !function ) {
				auto fn_name = encrypt( L"FortSafeZoneIndicator.Get" );
				function = find_object<u_function*>( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				u_object* world_context_object;
				a_fort_safe_zone_indicator* return_value;
			} params{ world_context_object };

			process_event( function, &params );

			return params.return_value;
		}
	};

	class a_fort_player_pawn_athena : public a_fort_player_pawn {
	public:
		declare_member( a_actor*, current_vehicle, 0x2b50 );
		declare_member( tarray<u_skeletal_mesh_component*>, skeletal_meshes, 0x5fe0 );
		declare_member( fvector, last_fired_location, 0x5a18 );
		declare_member( fvector, last_fired_direction, 0x5a30 );
		apply_member( frotator, player_aim_rotation, 0x69c8 );

		void replicate_aim_rotation( frotator player_aim_rotation ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"PlayerPawn_Athena_C.ReplicateAimRotation" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				frotator player_aim_rotation;
			} params{ player_aim_rotation };

			this->process_event( function, &params );
		}
	};

	class u_game_viewport_client : public u_object {
	public:
		declare_member( u_world*, world, 0x78 );
		apply_member( std::uint32_t, b_is_play_in_editor_viewport, 0xA0 );
		apply_member( std::uint32_t, b_disable_world_rendering, 0xB0 );

		bool line_trace( fvector start, fvector end, f_hit_result* out_hit ) {
			return line_trace_single(
				this->world( ),
				start,
				end,
				trace_type_query::trace_type_query_1,
				false,
				tarray<uint64_t>( ),
				draw_debug_trace::for_duration,
				false,
				flinear_color( ),
				flinear_color( ),
				0.f,
				out_hit
			);
		}

		bool line_trace_single(
			u_world* world_context_object,
			const fvector& start,
			const fvector& end,
			t_enum_ss_byte<trace_type_query> trace_channel,
			bool trace_complex,
			const tarray<uint64_t>& actors_to_ignore,
			t_enum_ss_byte<draw_debug_trace> draw_debug_type,
			bool ignore_self,
			const flinear_color& trace_color,
			const flinear_color& trace_hit_color,
			float draw_time,
			f_hit_result* out_hit
		) {
			static u_function* function = nullptr;
			if ( !function ) {
				auto enc_function = encrypt( L"KismetSystemLibrary.LineTraceSingle" );
				function = find_object<u_function*>( enc_function.decrypt( ) );
				enc_function.clear( );
			}

			struct {
				u_world* world_context_object;
				fvector start;
				fvector end;
				t_enum_ss_byte<trace_type_query> trace_channel;
				bool trace_complex;
				tarray<uint64_t> actors_to_ignore;
				t_enum_ss_byte<draw_debug_trace> draw_debug_type;
				f_hit_result out_hit;
				bool ignore_self;
				flinear_color trace_color;
				flinear_color trace_hit_color;
				float draw_time;
				unsigned char padding[ 0x8 ];
				bool return_value;
			} params;

			params.world_context_object = world_context_object;
			params.start = start;
			params.end = end;
			params.trace_channel = trace_channel;
			params.trace_complex = trace_complex;
			params.actors_to_ignore = actors_to_ignore;
			params.draw_debug_type = draw_debug_type;
			params.ignore_self = ignore_self;
			params.trace_color = trace_color;
			params.trace_hit_color = trace_hit_color;
			params.draw_time = draw_time;

			this->process_event( function, &params );

			if ( out_hit != nullptr )
				*out_hit = params.out_hit;

			return params.return_value;
		}
	};

	class u_scene_view_state : public u_object {
	public:
		declare_member( fmatrix, projection, 0x940 );
		declare_member( double, half_fov_tangent, 0x740 );

		float get_field_of_view( ) {
			return math::arctan_reciprocal_to_degrees( half_fov_tangent( ) );
		}
	};

	class a_fort_inventory : public a_actor {
	public:
		declare_member( f_fort_item_list, inventory, 0x2f0 );
	};

	class a_fort_player_marker_base : public a_actor {
	public:
		apply_member( fvector, impact_location, 0x2a8 );
		apply_member( flinear_color, color, 0x2c0 );
	};

	class u_fort_world_marker : public u_object {
	public:
		declare_member( a_fort_player_marker_base*, marker_actor, 0x130 );
	};

	class u_athena_marker_component : public u_actor_component {
	public:
		declare_member( u_fort_world_marker*, last_ping_marker, 0x680 );
	};

	class a_game_state_base : public u_object {
	public:
		declare_member( float, server_world_time, 0x2e0 );
		declare_member( tarray<a_fort_player_state_athena*>, player_array, 0x2c0 );

		bool is_in_lobby( ) { return server_world_time( ) ? false : true; }
	};

	class a_game_state : public a_game_state_base {
	public:
		declare_member( int32_t, elapsed_time, 0x308 );
	};

	class a_fort_game_state_base : public a_game_state {
	public:
	};

	class a_fort_game_state : public a_fort_game_state_base {
	public:
		declare_member( int32_t, team_count, 0x3a8 );
	};

	class a_fort_game_state_in_game : public a_fort_game_state {
	public:
	};

	class a_fort_game_state_zone : public a_fort_game_state_in_game {
	public:
		declare_member( int32_t, waiting_to_leave_zone_time_left, 0x864 );
	};

	class a_player_state : public u_object {
	public:
		declare_member( a_fort_player_pawn_athena*, pawn_private, 0x320 );
		declare_member( int32_t, player_id, 0x2ac );

		bool is_a_bot( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"PlayerState.IsABot" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				bool return_value;
			} params{ };

			this->process_event( function, &params );

			return params.return_value;
		}
	};

	class u_fort_player_state_component_habanero : public a_player_state {
	public:
		declare_member( f_ranked_progress_replicated_data, ranked_progress, 0xd0 );
	};

	class a_fort_player_state : public a_player_state {
	public:
		declare_member( int32_t, current_char_xp, 0x408 );
		declare_member( int32_t, total_player_score, 0x130c );
		declare_member( u_fort_player_state_component_habanero*, habanero_component, 0xa48 );

		fstring get_platform( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortPlayerState.GetPlatform" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				fstring return_value;
			} params{ };

			this->process_event( function, &params );

			return params.return_value;
		}
	};

	class a_fort_player_state_zone : public a_fort_player_state {
	public:
		int32_t get_spectator_count( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"FortPlayerStateZone.GetSpectatorCount" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				int32_t return_value;
			} params{ };

			this->process_event( function, &params );

			return params.return_value;
		}
	};

	class a_fort_player_state_athena : public a_fort_player_state_zone {
	public:
		declare_member( int, season_level_ui_display, 0x1270 );
		declare_member( int, kill_score, 0x126c );
		declare_member( int, team_score_placement, 0x125c );
		declare_member( int, total_player_score, 0x130c );
	};


	class a_fort_game_state_athena : public a_fort_game_state_zone {
	public:
		declare_member( int32_t, total_players, 0x17ec );
		declare_member( int32_t, players_left, 0x17f0 );
		declare_member( int32_t, players_loaded, 0x17f4 );
		declare_member( int32_t, teams_left, 0x1990 );
		declare_member( fvector2d, drop_zone_center, 0x1a18 );
		//declare_member( a_player_state*, winning_player_state, 0x1a50 );
		declare_member( tarray<ftext>, kill_feed_entry, 0x1a98 );
		declare_member( tarray<a_fort_player_state*>, spectator_array, 0x1ad0 );
		declare_member( bool, b_storm_reached_final_position, 0x1af1 );
		declare_member( tarray<char>, active_team_nums, 0x1dc0 );
	};

	class a_player_controller : public a_controller {
	public:
		declare_member( a_fort_player_pawn_athena*, acknowledged_pawn, 0x350 );
		declare_member( a_player_camera_manager*, player_camera_manager, 0x360 );
		declare_member( u_athena_marker_component*, marker_component, 0x4ba0 );
		declare_member( a_fort_quick_bars*, client_quick_bars, 0x1c98 );
		declare_member( tarray<f_fort_damage_number_info>, queued_damage_numbers, 0x1b70 );
		declare_member( a_fort_damage_numbers_actor*, damage_numbers_actor, 0x1b80 );
		apply_member( frotator, weapon_aim_offset, 0x2940 );
		apply_member( frotator, weapon_recoil_offset, 0x2958 );
		apply_member( frotator, weapon_offset_correction, 0x2970 );
		apply_member( frotator, last_weapon_offset_correction, 0x2988 );
		apply_member( float, desired_aim_target_offset, 0x2aa4 );

		bool deproject_screen_position_to_world( engine::fvector2d& screen_position, engine::fvector* world_position, engine::fvector* world_direction ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"PlayerController.DeprojectScreenPositionToWorld" );
				function = find_object<u_function*>( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				float screen_x;
				float screen_y;
				engine::fvector world_position;
				engine::fvector world_direction;
				bool return_value;
			} params{ screen_position.m_x, screen_position.m_y };

			this->process_event( function, &params );

			if ( params.return_value ) {
				*world_position = params.world_position;
				*world_direction = params.world_direction;
			}

			return params.return_value;
		}

		void add_yaw_input( float val ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"PlayerController.AddYawInput" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				float val;
			} params{ val };

			this->process_event( function, &params );
		}

		void add_pitch_input( float val ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"PlayerController.AddPitchInput" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				float val;
			} params{ val };

			this->process_event( function, &params );
		}

		fvector2d get_mouse_position( ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"PlayerController.GetMousePosition" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				float location_x;
				float location_y;
				bool return_value;
			} params;

			this->process_event( function, &params );

			return fvector2d( double( params.location_x ), double( params.location_y ) );
		}

		bool is_input_key_down( fkey key ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"PlayerController.IsInputKeyDown" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				fkey key;
				bool return_value;
			} params{ key };

			this->process_event( function, &params );

			return params.return_value;
		}

		bool was_input_key_just_pressed( fkey key ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"PlayerController.WasInputKeyJustPressed" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				fkey key;
				bool return_value;
			} params{ key };

			this->process_event( function, &params );

			return params.return_value;
		}

		bool was_input_key_just_released( fkey key ) {
			static u_function* function = 0;
			if ( !function ) {
				auto fn_name = encrypt( L"PlayerController.WasInputKeyJustReleased" );
				function = find_object< u_function* >( fn_name.decrypt( ) );
				fn_name.clear( );
			}

			struct {
				fkey key;
				bool return_value;
			} params{ key };

			this->process_event( function, &params );

			return params.return_value;
		}
	};

	class u_player : public u_object {
	public:
		declare_member( a_player_controller*, player_controller, 0x30 );
	};

	class u_fort_client_settings_record : public u_object {
	public:
		declare_member( fvector2d, gamepad_look_sensitivity, 0x618 );
		declare_member( float, gamepad_look_stick_dead_zone, 0x62c );
	};

	class u_localplayer : public u_player {
	public:
		declare_member( u_game_viewport_client*, viewport_client, 0x78 );
		declare_member( tarray<u_scene_view_state*>, view_state, 0xd0 );
		declare_member( u_fort_client_settings_record*, client_settings_record, 0x378 );

		u_scene_view_state* get_view_state( ) {
			return view_state( )[ 1 ];
		}
	};

	class u_game_instance : public u_object {
	public:
		declare_member( tarray<u_localplayer*>, localplayers, 0x38 );

		u_localplayer* get_localplayer( ) {
			return localplayers( )[ 0 ];
		}
	};

	class u_world : public u_object {
	public:
		declare_member( a_game_state_base*, game_state, 0x1a0 );
		declare_member( u_game_instance*, game_instance, 0x218 );
		declare_member( std::uintptr_t, camera_location_ptr, 0x128 );
		declare_member( std::uintptr_t, camera_rotation_ptr, 0x138 );

		c_camera_info queue_camera_info( u_scene_view_state* view_state ) {
			auto rotation_ptr = this->camera_rotation_ptr( );
			auto camera_rotation = frotator(
				*reinterpret_cast< double* >( rotation_ptr ),
				*reinterpret_cast< double* >( rotation_ptr + oxorany( 0x20 ) ),
				*reinterpret_cast< double* >( rotation_ptr + oxorany( 0x1d0 ) )
			);

			auto location_ptr = this->camera_location_ptr( );
			auto camera_location = *reinterpret_cast< engine::fvector* >( location_ptr );

			auto field_of_view = view_state->get_field_of_view( );
			return c_camera_info( camera_location, camera_rotation, field_of_view );
		}

		u_world* get_world( ) const {
			return find_object<u_world*>( encrypt( L"Frontend" ), reinterpret_cast< u_object* >( -1 ) );
		}
	};
}