#include <impl/includes.h>

__symbolic std::int32_t __stackcall main( int argc, char** argv ) noexcept {
    //qmorph::qdisasm::qsection_assembler sec{ };
    //sec.analyze_executable_sections( );
    //sec.morph_executable_sections( true );
    //sec.wipe_idata_ilt( );
    //sec.wipe_basereloc( );
    //sec.wipe_section_headers( );
    //sec.scramble_dos_header( );
    //sec.scramble_nt_header( );

    SetConsoleTitleA( encrypt( "orelia-gateway" ) );
    //SetUnhandledExceptionFilter( exception::exception_filter );

    bool enabled = false;
    auto status = RtlAdjustPrivilege(
        22L,                                         // SE_DEBUG_PRIVILEGE
        true,                                        // Enable
        false,                                       // Current process
        reinterpret_cast< BOOLEAN* >( &enabled ) );  // Previous state
    if ( status != 0 ) {
        logging::print( encrypt( "Failed to raise privilege" ) );
        return std::getchar( );
    }

    printf( encrypt( "                           .=%%+.                           \n" ) );
    printf( encrypt( "                           .+@@*.                           \n" ) );
    printf( encrypt( "                  :*:  ..:=*%@@%*=:..  :*:                  \n" ) );
    printf( encrypt( "                .+@@@*-*@@@@@@@@@@@@*-+@@@*.                \n" ) );
    printf( encrypt( "                 .*@@@@@@@#*#@@%*#@@@@@@@*.                 \n" ) );
    printf( encrypt( "                  .*@@@%+. .+@@*. .=%@@#:                   \n" ) );
    printf( encrypt( "                 .#@@@*    .-**=.    +:.                    \n" ) );
    printf( encrypt( "                .=@@@*                                      \n" ) );
    printf( encrypt( "            .:---%@@@---:  .:--:.  :----------:.            \n" ) );
    printf( encrypt( "            .*@@@@@@@@@@*  .+@@*.  *@@@@@@@@@@#.            \n" ) );
    printf( encrypt( "            .-==========-  .:--:.  :---@@@@=---.            \n" ) );
    printf( encrypt( "                                      +@@@+.                \n" ) );
    printf( encrypt( "                     :+    .-*+-.    +@@@#.                 \n" ) );
    printf( encrypt( "                   :#@@%=. .+@@*. .=%@@@*.                  \n" ) );
    printf( encrypt( "                 .+@@@@@@@#+#@@#+#@@@@@@@+.                 \n" ) );
    printf( encrypt( "                .+@@@*-*@@@@@@@@@@@@#=*@@@*.                \n" ) );
    printf( encrypt( "                  :*:  ..:=*%@@%#=:..  :#:                  \n" ) );
    printf( encrypt( "                           .*@@*.                           \n" ) );
    printf( encrypt( "                           .+%%+.                           \n\n" ) );
    logging::print( encrypt( "Connecting to ViGEm Client..." ) );

    auto client = vigem_alloc( );
    if ( !client ) {
        logging::print( encrypt( "Failed to allocate vMulti client" ) );
        return std::getchar( );
    }
    logging::print( encrypt( "ViGEm client allocated" ) );

    const auto connect_result = vigem_connect( client );
    if ( !VIGEM_SUCCESS( connect_result ) ) {
        logging::print( encrypt( "ViGEm connection failed (Code: 0x%08X)" ), connect_result );
        vigem_free( client );
        return std::getchar( );
    }
    logging::print( encrypt( "Connected to ViGEm bus" ) );

    auto target = vigem_target_ds4_alloc( );
    if ( !target ) {
        logging::print( encrypt( "Failed to allocate virtual mouse" ) );
        vigem_disconnect( client );
        vigem_free( client );
        return std::getchar( );
    }
    logging::print( encrypt( "Virtual controller allocated" ) );

    const auto add_result = vigem_target_add( client, target );
    if ( !VIGEM_SUCCESS( add_result ) ) {
        logging::print( encrypt( "Controller attachment failed (Code: 0x%08X)" ), add_result );
        vigem_target_free( target );
        vigem_disconnect( client );
        vigem_free( client );
        return std::getchar( );
    }

    DS4_REPORT report = { 0 };
    report.bThumbLX = 255;
    report.bThumbLY = 255;
    report.bThumbRX = 128;
    report.bThumbRY = 128;
    report.wButtons = 0;
    report.bSpecial = 0;
    report.bTriggerL = 0;
    report.bTriggerR = 0;

    const auto update_result = vigem_target_ds4_update( client, target, report );
    if ( !VIGEM_SUCCESS( update_result ) ) {
        logging::print( encrypt( "HID report failed (Code: 0x%08X)" ), add_result );
        vigem_target_free( target );
        vigem_disconnect( client );
        vigem_free( client );
        return std::getchar( );
    }

    logging::print( encrypt( "Virtual controller connected\n" ) );

    logging::print( encrypt( "Initializing Winsock..." ) );
    WSADATA wsa_data;
    int result = WSAStartup( MAKEWORD( 2, 2 ), &wsa_data );
    if ( result != 0 ) {
        logging::print( encrypt( "Failed to initialize WSA" ) );
        utility::vigem_cleanup( client, target );
        return std::getchar( );
    }
    logging::print( encrypt( "Winsock initialized (v%d.%d)\n" ),
        HIBYTE( wsa_data.wVersion ), LOBYTE( wsa_data.wVersion ) );

    WCHAR lsp_name[ ] = L"@%SystemRoot%\\System32\\orelia.dll,-1000";
    bool namespace_exists = false;

    logging::print( encrypt( "Scanning for existing namespace providers..." ) );
    WSANAMESPACE_INFOEXW* namespace_info = nullptr;
    DWORD bytes_needed = 0;
    int protocol_count = WSCEnumNameSpaceProvidersEx( &bytes_needed, NULL );
    if ( protocol_count == SOCKET_ERROR && WSAGetLastError( ) == WSAEFAULT ) {
        namespace_info = ( WSANAMESPACE_INFOEXW* )malloc( bytes_needed );
        protocol_count = WSCEnumNameSpaceProvidersEx( &bytes_needed, namespace_info );
        if ( protocol_count != SOCKET_ERROR ) {
            logging::print( encrypt( "Found %d namespace providers" ), protocol_count );
            for ( int i = 0; i < protocol_count; i++ ) {
                if ( wcscmp( namespace_info[ i ].lpszIdentifier, lsp_name ) == 0 ) {
                    namespace_exists = true;
                    logging::print( encrypt( "Target namespace provider already exists." ) );
                    auto response = logging::input( "Would you like to delete and reinstall the namespace? (Y/N): " );
                    printf( encrypt( "\n" ) );

                    if ( response == "Y" || response == "y" || response == "Yes" || response == "yes" ) {
                        logging::print( encrypt( "Updating provider..." ) );

                        WSACleanup( );
                        Sleep( 1000 );

                        WCHAR system_dir[ MAX_PATH ];
                        GetSystemDirectoryW( system_dir, MAX_PATH );

                        WCHAR full_path[ MAX_PATH ];
                        swprintf_s( full_path, MAX_PATH, L"%s\\orelia.dll", system_dir );

                        if ( argc < 2 ) {
                            MessageBoxA( 0, encrypt( "Please drap and drop the signed .dll over the program." ),
                                encrypt( "orelia - gateway" ), MB_ICONWARNING | MB_OK );
                            return std::getchar( );
                        }

                        std::vector<std::uint8_t> signed_file;
                        disk::open_file( argv[ 1 ], signed_file );
                        if ( disk::write_signed_file( L"orelia.dll", signed_file.data( ), signed_file.size( ) ) ) {
                            logging::print( encrypt( "Successfully updated DLL file" ) );

                            if ( WSCUnInstallNameSpace( &namespace_info[ i ].NSProviderId ) != 0 ) {
                                logging::print( encrypt( "Could not remove old namespace record" ) );
                                utility::vigem_cleanup( client, target );
                                return std::getchar( );
                            }
                            else {
                                logging::print( encrypt( "Namespace provider record removed successfully\n" ) );
                                namespace_exists = false;
                            }
                        }
                        else {
                            logging::print( encrypt( "Failed to update DLL file" ) );
                            utility::vigem_cleanup( client, target );
                            return std::getchar( );
                        }

                        result = WSAStartup( MAKEWORD( 2, 2 ), &wsa_data );
                        if ( result != 0 ) {
                            logging::print( encrypt( "Failed to reinitialize WSA" ) );
                            utility::vigem_cleanup( client, target );
                            return std::getchar( );
                        }
                    }
                    else
                        logging::print( encrypt( "Keeping existing namespace provider\n" ) );
                    break;
                }
            }
        }
        else {
            logging::print( encrypt( "Failed to enumerate namespace providers." ) );
            utility::vigem_cleanup( client, target );
            return std::getchar( );
        }

        free( namespace_info );
    }

    if ( !namespace_exists ) {
        logging::print( encrypt( "Installing new namespace provider..." ) );
        GUID provider_id;
        WCHAR lsp_path[ ] = L"%SystemRoot%\\System32\\orelia.dll";
        auto namespace_type = NS_DNS;

        if ( CoCreateGuid( &provider_id ) != S_OK ) {
            logging::print( encrypt( "Failed to create GUID for namespace provider" ) );
            utility::vigem_cleanup( client, target );
            return std::getchar( );
        }

        auto error_code = WSCInstallNameSpace( lsp_name, lsp_path, namespace_type, 1, &provider_id );
        if ( error_code != 0 ) {
            logging::print( encrypt( "Failed to install namespace provider, error code: %d" ), error_code );
            utility::vigem_cleanup( client, target );
            return std::getchar( );
        }
        logging::print( encrypt( "Namespace provider installed successfully.\n" ) );

        if ( argc < 2 ) {
            MessageBoxA( 0, encrypt( "Please drap and drop the signed .dll over the program." ),
                encrypt( "orelia - gateway" ), MB_ICONWARNING | MB_OK );
            return std::getchar( );
        }

        std::vector<std::uint8_t> signed_file;
        disk::open_file( argv[ 1 ], signed_file );
        if ( !disk::write_signed_file( L"orelia.dll", signed_file.data( ), signed_file.size( ) ) ) {
            logging::print( encrypt( "Failed to write signed file" ) );
            utility::vigem_cleanup( client, target );
            return std::getchar( );
        }
    }

    logging::print( encrypt( "Initializing named pipe..." ) );

    pipe::c_pipe pipe;
    if ( !pipe.create( encrypt( "orelia-gateway" ) ) ) {
        logging::print( encrypt( "Failed to create named pipe" ) );
        utility::vigem_cleanup( client, target );
        return std::getchar( );
    }

    logging::print( encrypt( "IPC pipeline ready\n" ) );
    logging::print( encrypt( "Gateway operational - awaiting connections" ) );

    while ( true ) {
        if ( pipe.wait_for_connection( ) ) {
            static bool connected = false;
            if ( !connected )
                logging::print( encrypt( "Successfully connected to client. (DONT CLOSE)\n" ) );
            connected = true;

            pipe::c_command_data cmd;
            if ( !pipe.get_cmd( &cmd ) ) break;
            if ( cmd.is_completed( ) ) continue;

            switch ( cmd.get_type( ) ) {
            case pipe::e_command_type::ping_server:
            {
                cmd.set_status( pipe::in_progressive_state );
            } break;
            case pipe::e_command_type::debug_print:
            {
                logging::print( cmd.get_data( ).format );
            } break;
            case pipe::e_command_type::message_box:
            {
                MessageBoxA( 0, cmd.get_data( ).format, encrypt( "orelia - client" ), MB_ICONINFORMATION | MB_OK );
                cmd.set_status( pipe::in_progressive_state );
            } break;
            case pipe::e_command_type::move_mouse: 
            {
                DS4_REPORT report = { 0 };
                report.bThumbLX = 128;
                report.bThumbLY = 128;
                report.bThumbRX = static_cast< BYTE >( cmd.get_data( ).m_x );
                report.bThumbRY = static_cast< BYTE >( cmd.get_data( ).m_y );
                report.wButtons = 0;
                report.bSpecial = 0;
                report.bTriggerL = 0;
                report.bTriggerR = 0;

                const auto update_result = vigem_target_ds4_update( client, target, report );
                if ( !VIGEM_SUCCESS( update_result ) ) {
                    logging::print( encrypt( "HID report failed (Code: 0x%08X)" ), add_result );
                    utility::vigem_cleanup( client, target );
                    return std::getchar( );
                }
            } break;
            case pipe::e_command_type::reset_mouse:
            {
                DS4_REPORT neutral = { 0 };
                neutral.bThumbLX = 128;
                neutral.bThumbLY = 128;
                neutral.bThumbRX = 128;
                neutral.bThumbRY = 128;

                const auto update_result = vigem_target_ds4_update( client, target, neutral );
                if ( !VIGEM_SUCCESS( update_result ) ) {
                    logging::print( encrypt( "HID report failed (Code: 0x%08X)" ), add_result );
                    utility::vigem_cleanup( client, target );
                    return std::getchar( );
                }
            } break;
            }

            cmd.set_completed( true );
            pipe.send_cmd( cmd );
        }

        Sleep( 1 );
    }

    logging::print( encrypt( "Client closed connection with host." ) );
    utility::vigem_cleanup( client, target );
    return std::getchar( );
}