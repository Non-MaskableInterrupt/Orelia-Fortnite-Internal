#pragma once
namespace engine {
	namespace keys {
		fkey g_left_mouse;
		fkey g_right_mouse;
		fkey g_insert;
		fkey g_left_shift;
		fkey g_left_alt;
		fkey g_thumb_mouse_button;
		fkey g_thumb_mouse_button2;
		fkey g_gamepad_left_trigger;
		fkey g_w;
		fkey g_a;
		fkey g_s;
		fkey g_d;
		fkey g_spacebar;
		fkey g_mouse_scroll_up;
		fkey g_mouse_scroll_down;
		fkey g_mouse_wheel_axis;
		fkey g_f7;
		fkey g_f8;
	}

	float pow22_one_over_255_table[ 256 ]{
		0.0f, 5.07705190066176E-06f, 2.33280046660989E-05f, 5.69217657121931E-05f, 0.000107187362341234f, 0.000175123977503027f, 0.000261543754548491f, 0.000367136269815943f, 0.000492503787191433f,
		0.000638182842167022f, 0.000804658499513058f, 0.000992374304074325f, 0.0012017395234384f, 0.00143313458967186f, 0.00168691531678928f, 0.00196341621339647f, 0.00226295316070643f,
		0.00258582559623417f, 0.00293231832393836f, 0.00330270303200364f, 0.00369723957890013f, 0.00411617709328275f, 0.00455975492252602f, 0.00502820345685554f, 0.00552174485023966f,
		0.00604059365484981f, 0.00658495738258168f, 0.00715503700457303f, 0.00775102739766061f, 0.00837311774514858f, 0.00902149189801213f, 0.00969632870165823f, 0.0103978022925553f,
		0.0111260823683832f, 0.0118813344348137f, 0.0126637200315821f, 0.0134733969401426f, 0.0143105193748841f, 0.0151752381596252f, 0.0160677008908869f, 0.01698805208925f, 0.0179364333399502f,
		0.0189129834237215f, 0.0199178384387857f, 0.0209511319147811f, 0.0220129949193365f, 0.0231035561579214f, 0.0242229420675342f, 0.0253712769047346f, 0.0265486828284729f, 0.027755279978126f,
		0.0289911865471078f, 0.0302565188523887f, 0.0315513914002264f, 0.0328759169483838f, 0.034230206565082f, 0.0356143696849188f, 0.0370285141619602f, 0.0384727463201946f, 0.0399471710015256f,
		0.0414518916114625f, 0.0429870101626571f, 0.0445526273164214f, 0.0461488424223509f, 0.0477757535561706f, 0.049433457555908f, 0.0511220500564934f, 0.052841625522879f, 0.0545922772817603f,
		0.0563740975519798f, 0.0581871774736854f, 0.0600316071363132f, 0.0619074756054558f, 0.0638148709486772f, 0.0657538802603301f, 0.0677245896854243f, 0.0697270844425988f, 0.0717614488462391f,
		0.0738277663277846f, 0.0759261194562648f, 0.0780565899581019f, 0.080219258736215f, 0.0824142058884592f, 0.0846415107254295f, 0.0869012517876603f, 0.0891935068623478f, 0.0915183529989195f,
		0.0938758665255778f, 0.0962661230633397f, 0.0986891975410945f, 0.1011451642096f, 0.103634096655137f, 0.106156067812744f, 0.108711149979039f, 0.11129941482466f, 0.113920933406333f,
		0.116575776178572f, 0.119264013005047f, 0.121985713169619f, 0.124740945387051f, 0.127529777813422f, 0.130352278056234f, 0.1332085131843f, 0.136098549737202f, 0.139023453734703f,
		0.141980290685736f, 0.144972125597231f, 0.147998022982685f, 0.151058046870511f, 0.154152260812165f, 0.157280727890073f, 0.160443510725344f, 0.16364067148529f, 0.166872271890766f,
		0.170138373223312f, 0.173439036332135f, 0.176774321640903f, 0.18014428915439f, 0.183548998464951f, 0.186988508758844f, 0.190462878823409f, 0.193972167048093f, 0.19751643144034f,
		0.201095729621346f, 0.204710118836677f, 0.208359655960767f, 0.212044397502288f, 0.215764399609395f, 0.219519718074868f, 0.223310408341127f, 0.227136525505149f, 0.230998124323267f,
		0.23489525921588f, 0.238827984272048f, 0.242796353254002f, 0.24680041960155f, 0.2508402364364f, 0.254915856566385f, 0.259027332489606f, 0.263174716398492f, 0.267358060183772f,
		0.271577415438375f, 0.275832833461245f, 0.280124365261085f, 0.284452061560024f, 0.288815972797219f, 0.293216149132375f, 0.297652640449211f, 0.302125496358853f, 0.306634766203158f,
		0.311180499057984f, 0.315762743736397f, 0.32038154879181f, 0.325036962521076f, 0.329729032967515f, 0.334457807923889f, 0.339223334935327f, 0.344025661302187f, 0.348864834082879f,
		0.353740900096629f, 0.358653905926199f, 0.363603897920553f, 0.368590922197487f, 0.373615024646202f, 0.37867625092984f, 0.383774646487975f, 0.388910256539059f, 0.394083126082829f,
		0.399293299902674f, 0.404540822567962f, 0.409825738436323f, 0.415148091655907f, 0.420507926167587f, 0.425905285707146f, 0.43134021380741f, 0.436812753800359f, 0.442322948819202f,
		0.44787084180041f, 0.453456475485731f, 0.45907989242416f, 0.46474113497389f, 0.470440245304218f, 0.47617726539744f, 0.481952237050698f, 0.487765201877811f, 0.493616201311074f,
		0.49950527660303f, 0.505432468828216f, 0.511397818884879f, 0.517401367496673f, 0.523443155214325f, 0.529523223417277f, 0.535641609315311f, 0.541798355950137f, 0.547993502196972f,
		0.554227087766085f, 0.560499152204328f, 0.566809734896638f, 0.573158875067523f, 0.579546611782525f, 0.585972983949661f, 0.592438030320847f, 0.598941789493296f, 0.605484299910907f,
		0.612065599865624f, 0.61868572749878f, 0.625344720802427f, 0.632042617620641f, 0.638779455650817f, 0.645555272344934f, 0.652370105410821f, 0.659223991813387f, 0.666116968775851f,
		0.673049073280942f, 0.680020342172095f, 0.687030812154625f, 0.694080519796882f, 0.701169501531402f, 0.708297793656032f, 0.715465432335048f, 0.722672453600255f, 0.729918893352071f,
		0.737204787360605f, 0.744530171266715f, 0.751895080583051f, 0.759299550695091f, 0.766743616862161f, 0.774227314218442f, 0.781750677773962f, 0.789313742415586f, 0.796916542907978f,
		0.804559113894567f, 0.81234148989849f, 0.819963705323528f, 0.827725794455034f, 0.835527791460841f, 0.843369730392169f, 0.851251645184515f, 0.859173569658532f, 0.867135537520905f,
		0.875137582365205f, 0.883179737672745f, 0.891262036813419f, 0.899384513046529f, 0.907547199521614f, 0.915750129279253f, 0.923993335251873f, 0.932276850264543f, 0.940600707035753f,
		0.948964938178195f, 0.957369576199527f, 0.96581465350313f, 0.974300202388861f, 0.982826255053791f, 0.99139284359294f, 1.0f
	};

	namespace math {
		constexpr double pi = 3.14159265358979323846;

		bool is_valid_ptr( void* ptr ) {
			return ptr && ptr > ( void* )0x10000 && ptr < ( void* )0x7FFFFFFFFFFF;
		}

		frotator normalize_euler_angles( frotator& result ) {
			result.m_yaw = ( kismet::g_math_library->atan2( result.m_pitch * oxorany( -1.0 ), result.m_yaw ) * ( oxorany( 180.0 ) / pi ) ) * oxorany( -1.0 );
			result.m_yaw *= oxorany( -1.0 );

			result.m_pitch = kismet::g_math_library->asin( result.m_roll ) * ( oxorany( 180.0 ) / pi );
			result.m_roll = oxorany( 0.0 );
			return result;
		}

		float arctan_reciprocal_to_degrees( double half_tangent ) {
			auto result = kismet::g_math_library->atan( oxorany( 1.0 ) / half_tangent ) * oxorany( 2.f );
			result *= ( oxorany( 180.f ) / pi );
			return result;
		}

		fmatrix get_rotation_matrix( const frotator& rotation ) {
			fmatrix matrix = {};

			const double pitch = rotation.m_pitch * pi / 180.0;
			const double yaw = rotation.m_yaw * pi / 180.0;
			const double roll = rotation.m_roll * pi / 180.0;

			const double sp = kismet::g_math_library->sin( pitch );
			const double cp = kismet::g_math_library->cos( pitch );
			const double sy = kismet::g_math_library->sin( yaw );
			const double cy = kismet::g_math_library->cos( yaw );
			const double sr = kismet::g_math_library->sin( roll );
			const double cr = kismet::g_math_library->cos( roll );

			matrix.m_x_plane.m_x = cp * cy;
			matrix.m_x_plane.m_y = cp * sy;
			matrix.m_x_plane.m_z = sp;

			matrix.m_y_plane.m_x = sr * sp * cy - cr * sy;
			matrix.m_y_plane.m_y = sr * sp * sy + cr * cy;
			matrix.m_y_plane.m_z = -sr * cp;

			matrix.m_z_plane.m_x = -( cr * sp * cy + sr * sy );
			matrix.m_z_plane.m_y = cy * sr - cr * sp * sy;
			matrix.m_z_plane.m_z = cr * cp;

			matrix.m_w_plane.m_w = 1.0;

			return matrix;
		}

		float deg2rad( float degrees ) {
			return degrees * static_cast< float >( pi ) / 180.0f;
		}

		float rad2deg( float radians ) {
			return radians * 180.0f / static_cast< float >( pi );
		}

		double clamp( double value, double min, double max ) {
			return std::clamp( value, min, max );
		}

		flinear_color from_rgb( int r, int g, int b ) {
			flinear_color result;
			result.m_r = pow22_one_over_255_table[ r ];
			result.m_g = pow22_one_over_255_table[ g ];
			result.m_b = pow22_one_over_255_table[ b ];
			result.m_a = 1.f;
			return result;
		}
	}

	namespace init {
		fkey create_key_binding( const wchar_t* key_name ) {
			return fkey{
				fname{
					kismet::g_string_library->conv_string_to_name( key_name )
				},
				0
			};
		}

		void init_libraries( ) {
			kismet::g_math_library = u_object::find_object<u_kismet_math_library*>( L"Engine.Default__KismetMathLibrary" );
			kismet::g_string_library = u_object::find_object<u_kismet_string_library*>( L"Engine.Default__KismetStringLibrary" );
			kismet::g_system_library = u_object::find_object<u_kismet_system_library*>( L"Engine.Default__KismetSystemLibrary" );
			kismet::g_fort_library = u_object::find_object<u_fort_kismet_library*>( L"FortniteGame.Default__FortKismetLibrary" );
			kismet::g_gameplay_statics = u_object::find_object<u_gameplay_statics*>( L"Engine.Default__GameplayStatics" );
		}

		void init_classes( ) {
			classes::g_fort_weapon = u_object::find_object<u_class*>( L"FortniteGame.FortWeaponRanged" );
			classes::g_fort_pickup = u_object::find_object<u_class*>( L"FortniteGame.FortPickup" );
			classes::g_container = u_object::find_object<u_class*>( L"FortniteGame.BuildingContainer" );
			classes::g_weakspot = u_object::find_object<u_class*>( L"FortniteGame.BuildingWeakspot" );
			classes::g_vehicle = u_object::find_object<u_class*>( L"FortniteGame.FortAthenaVehicle" );
			classes::g_player_controller = u_object::find_object<u_class*>( L"Engine.Controller" );
			classes::g_building = u_object::find_object<u_class*>( L"FortniteGame.BuildingActor" );
			classes::g_item_definition = u_object::find_object<u_class*>( L"FortniteGame.FortItemDefinition" );
			classes::g_material_instance = u_object::find_object<u_class*>( L"Engine.MaterialInstance" );
		}

		void init_keys( ) {
			const std::pair<fkey*, const wchar_t*> key_mappings[ ] = {
				{&keys::g_left_mouse, L"LeftMouseButton"},
				{&keys::g_right_mouse, L"RightMouseButton"},
				{&keys::g_insert, L"Insert"},
				{&keys::g_left_shift, L"LeftShift"},
				{&keys::g_left_alt, L"LeftAlt"},
				{&keys::g_gamepad_left_trigger, L"Gamepad_LeftTrigger"},
				{&keys::g_w, L"W"},
				{&keys::g_a, L"A"},
				{&keys::g_s, L"S"},
				{&keys::g_d, L"D"},
				{&keys::g_spacebar, L"Spacebar"},
				{&keys::g_thumb_mouse_button2, L"ThumbMouseButton2"},
				{&keys::g_mouse_wheel_axis, L"MouseWheelAxis"},
				{&keys::g_f7, L"F7"},
				{&keys::g_f8, L"F8"}
			};

			for ( const auto& [key_ptr, key_name] : key_mappings )
				*key_ptr = create_key_binding( key_name );
		}
	}

	fvector frotator::get_forward_vector( ) {
		return kismet::g_math_library->get_forward_vector( *this );
	}

	template <class t>
	tarray<t> u_object::get_actors_of_class( u_class* object_class ) {
		return kismet::g_gameplay_statics->get_all_actors_of_class<t>(
			this,
			object_class
		);
	}

	bool u_object::object_is_a( u_class* object_class ) {
		return kismet::g_gameplay_statics->object_is_a(
			this,
			object_class
		);
	}

	template <class type>
	type* u_object::load_object( const wchar_t* command, a_player_controller* player_controller ) {
		kismet::g_system_library->execute_console_command(
			this,
			command,
			player_controller
		);

		return find_object<type*>( command );
	}

	fstring u_object::get_object_name( ) {
		return kismet::g_system_library->get_object_name( this );
	}

	frotator fvector::to_rotator( ) {
		return kismet::g_math_library->conv_vector_to_rotator( *this );
	}

	double fvector::distance_to( const fvector& other ) {
		return kismet::g_math_library->vector_distance( other, *this ) * 0.01;
	}

	double fvector2d::distance_to( const fvector2d& other ) {
		return kismet::g_math_library->vector_distance(
			engine::fvector( other.m_x, other.m_y, 0.0 ),
			engine::fvector( this->m_x, this->m_y, 0.0 )
		) * 0.01;
	}

	fvector u_scene_component::get_bone_location( const wchar_t* socket_name ) {
		return get_socket_location(
			kismet::g_string_library->conv_string_to_name( socket_name )
		);
	}
}