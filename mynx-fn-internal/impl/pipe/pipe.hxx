#pragma once

namespace pipe {
	const std::uint32_t in_success_state = 1;
	const std::uint32_t in_progressive_state = 2;
	const std::uint32_t in_failure_state = 3;

	enum class e_command_type : std::uint32_t {
		ping_server,
		debug_print,
		message_box,
		move_mouse,
		reset_mouse
	};

	struct s_command_data {
		std::uint32_t pid{};
		std::uintptr_t address{};
		std::uintptr_t address2{};
		std::size_t size{};
		void* buffer{};
		char format[ 256 ]{};
		std::uint32_t m_x{};
		std::uint32_t m_y{};
	};

	class c_command_data {
	public:
		c_command_data( ) = default;
		c_command_data(
			e_command_type type,
			s_command_data data,
			bool priority = false )
			: m_type( type ), m_data( data ), m_completed( false ) {
		}

		bool is_completed( ) const { return m_completed; }
		void set_completed( bool state ) { m_completed = state; }

		std::uint32_t get_status( ) const { return m_status_code; }
		void set_status( std::uint32_t status ) { m_status_code = status; }

		e_command_type get_type( ) const { return m_type; }
		const s_command_data& get_data( ) const { return m_data; }

	private:
		e_command_type m_type;
		s_command_data m_data;
		bool m_completed;
		std::uint32_t m_status_code;
		std::uint64_t m_timestamp;
	};

	class c_pipe {
	public:
		c_pipe( ) = default;
		void ping_server( ) const {
			auto cmd_data = pipe::c_command_data(
				pipe::e_command_type::ping_server,
				pipe::s_command_data( GetCurrentProcessId( ) ) );
			while ( !send_cmd( cmd_data ) ) { }
		}

		bool send_cmd( c_command_data& request ) const {
			DWORD bytes_read = 0;

			if ( !WriteFile( m_pipe, &request, sizeof( request ), &bytes_read, NULL ) ) {
				return false;
			}

			if ( !ReadFile( m_pipe, &request, sizeof( request ), &bytes_read, NULL ) ) {
				return false;
			}

			return request.is_completed( );
		}

		bool connect( const char* module_name ) {
			auto pipe_name = encrypt( "\\\\.\\Pipe\\Local\\" ).decrypt( ) + std::string( module_name );
			return connect_to_pipe( pipe_name.c_str( ) );
		}

		template<typename... Args>
		bool debug_print( const char* format, Args... args ) {
			char buffer[ 256 ]{ };
			sprintf_s( buffer, format, args... );

			s_command_data data;
			data.pid = GetCurrentProcessId( );
			strcpy_s( data.format, buffer );

			c_command_data cmd(
				e_command_type::debug_print,
				data
			);

			return send_cmd( cmd ) && cmd.is_completed( );
		}

		bool message_box( const char* format ) const {
			while ( true ) {
				pipe::s_command_data in_cmd_data{ };
				char buffer[ 256 ]{ };
				sprintf_s( buffer, format );
				strcpy_s( in_cmd_data.format, buffer );

				auto out_cmd_data = pipe::c_command_data(
					pipe::e_command_type::message_box,
					in_cmd_data );
				if ( !send_cmd( out_cmd_data ) )
					return false;

				if ( out_cmd_data.get_status( ) == pipe::in_progressive_state )
					return false;
			}

			return true;
		}

		bool move_mouse( int stick_x, int stick_y ) {
			s_command_data data;
			data.pid = GetCurrentProcessId( );
			data.m_x = stick_x;
			data.m_y = stick_y;

			c_command_data cmd(
				e_command_type::move_mouse,
				data
			);

			return send_cmd( cmd ) && cmd.is_completed( );
		}

		bool reset_mouse( ) {
			s_command_data data;
			data.pid = GetCurrentProcessId( );

			c_command_data cmd(
				e_command_type::reset_mouse,
				data
			);

			return send_cmd( cmd ) && cmd.is_completed( );
		}

		bool is_valid( ) const {
			return m_pipe != reinterpret_cast< HANDLE >( oxorany( -1 ) );
		}

		bool is_active_session( ) const {
			return m_is_active;
		}

		void end_session( ) {
			m_is_active = false;
		}

	private:
		HANDLE m_pipe = nullptr;
		double m_start_time = 0.0;
		bool m_is_active = true;

		bool connect_to_pipe( const char* pipe_name ) {
			if ( !WaitNamedPipeA( pipe_name, 5000 ) ) {
				return false;
			}

			m_pipe = CreateFileA(
				pipe_name,
				GENERIC_READ | GENERIC_WRITE,
				0,
				NULL,
				OPEN_EXISTING,
				0,
				NULL
			);

			if ( m_pipe == INVALID_HANDLE_VALUE ) {
				DWORD error = GetLastError( );
				char debug_msg[ 256 ];
				sprintf_s( debug_msg, "CreateFile failed with error: %d", error );
				MessageBoxA( 0, debug_msg, "Error", 0 );
				return false;
			}

			DWORD pipe_mode = PIPE_READMODE_MESSAGE;
			if ( !SetNamedPipeHandleState( m_pipe, &pipe_mode, NULL, NULL ) ) {
				CloseHandle( m_pipe );
				m_pipe = INVALID_HANDLE_VALUE;
				return false;
			}

			return true;
		}
	};
}

inline pipe::c_pipe g_pipe;