#pragma once

namespace pipe {
    const std::uint32_t max_failure_count = 10;

	bool heartbeat( pipe::c_pipe& pipe ) {
        while ( true ) {
            static auto last_failure = 0;
            static auto start_time = std::chrono::steady_clock::now( );
            auto current_time = std::chrono::steady_clock::now( );

            auto elapsed_time = std::chrono::duration<double>( current_time - start_time ).count( );
            if ( elapsed_time >= 4.0 ) {
                if ( elapsed_time <= 4.5 ) {
                    auto cmd_data = pipe::c_command_data(
                        pipe::e_command_type::ping_server,
                        pipe::s_command_data( GetCurrentProcessId( ) ) );
                    auto result = pipe.send_cmd( cmd_data );
                    if ( !result || cmd_data.get_status( ) != pipe::in_progressive_state )
                        last_failure++;
                    else
                        last_failure = 0;
                }
                else if ( elapsed_time >= 5.0 ) {
                    start_time = std::chrono::steady_clock::now( );
                }
            }

            if ( last_failure > max_failure_count )
                break;
        }

		return false;
	}
}