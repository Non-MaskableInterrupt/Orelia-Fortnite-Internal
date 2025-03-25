#include <cstdlib>
#include <impl/includes.h>
#include <pplwin.h>

unsigned __stdcall entry_point( void* ) {
    pipe::c_pipe pipe;
    while ( !pipe.connect( encrypt( "orelia-gateway" ) ) ) { }
    pipe.ping_server( );

    process::c_process process;
    orelia::base_address = process.get_main_module( );
    if ( !orelia::base_address ) 
        return oxorany( true );

    pipe.debug_print(
        encrypt( "Base address: %llx" ),
        orelia::base_address
    );

    auto module_size = process.get_module_size(
        encrypt( L"FortniteClient-Win64-Shipping.exe" )
    );
    if ( !module_size )
        return oxorany( true );

    pipe.debug_print(
        encrypt( "Module size: %i\n" ),
        module_size
    );

    pipe.message_box( encrypt( "Press OK when your ready for injection. Note do NOT close the gateway while activily using software" ) );
    pipe.ping_server( );

    engine::u_world world;
    engine::init::init_libraries( );
    engine::init::init_classes( );
    engine::init::init_keys( );

    auto gworld = world.get_world( );
    if ( !gworld ) return oxorany( true );
    pipe.debug_print(
        encrypt( "GWorld: %llx" ),
        reinterpret_cast< std::uintptr_t >( gworld )
    );

    auto game_instance = gworld->game_instance( );
    if ( !game_instance ) return oxorany( true );
    pipe.debug_print(
        encrypt( "OwningGameInstance: %llx" ),
        reinterpret_cast< std::uintptr_t >( game_instance )
    );

    auto local_player = game_instance->get_localplayer( );
    if ( !local_player ) return oxorany( true );
    pipe.debug_print(
        encrypt( "LocalPlayer: %llx" ),
        reinterpret_cast< std::uintptr_t >( local_player )
    );

    auto viewport_client = local_player->viewport_client( );
    if ( !viewport_client ) return oxorany( true );
    pipe.debug_print(
        encrypt( "ViewportClient: %llx" ),
        reinterpret_cast< std::uintptr_t >( viewport_client )
    );

    static vmt::c_vmt vmt;
    if ( !vmt.setup( viewport_client, offsets::draw_transition ) ) {
        pipe.debug_print( encrypt( "VMT setup failed" ) );
        return oxorany( true );
    }

    pipe.debug_print(
        encrypt( "DrawTransition: %llx\n" ),
        vmt.get_function( offsets::draw_transition )
    );

    g_pipe = pipe;
    auto status = vmt.create_hook(
        draw_transition::draw_transition,
        &draw_transition::draw_transition_original
    );

    if ( !status ) {
        pipe.debug_print(
            encrypt( "CreateHook failed with: %i" ),
            status
        );

        return oxorany( true );
    }

    auto new_func = vmt.get_function( offsets::draw_transition );
    pipe.debug_print(
        encrypt( "New function ptr: %llx" ),
        new_func
    );

    auto old_func = vmt.get_original( offsets::draw_transition );
    pipe.debug_print(
        encrypt( "Old function ptr: %llx" ),
        old_func
    );

    bool is_hooked = vmt.is_hooked_function(
        offsets::draw_transition,
        draw_transition::draw_transition
    );

    pipe.debug_print(
        encrypt( "Hook verification: %s\n" ),
        is_hooked ? "success" : "failed"
    );

    return pipe::heartbeat( pipe );
}

BOOL APIENTRY DllMain( unsigned long long a1, int a2, unsigned long long a3 ) {
    if ( a2 == oxorany( process_attach ) ) {
        process::c_process process;
        if ( !process.is_valid( encrypt( L"FortniteClient-Win64-Shipping.exe" ) ) )
            return oxorany( true );

        auto handle = _beginthreadex( 0, 0, entry_point, 0, 0, 0 );
        CloseHandle( reinterpret_cast< HANDLE >( handle ) );
    }

    return oxorany( true );
}