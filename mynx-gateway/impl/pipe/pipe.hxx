
namespace pipe
{
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

	struct s_command_data
	{
		std::uint32_t pid{};
		std::uintptr_t address{};
		std::uintptr_t address2{};
		std::size_t size{};
		void* buffer{};
		char format[ 256 ]{};
		std::uint32_t m_x{};
		std::uint32_t m_y{};
	};

	class c_command_data
	{
	public:
		c_command_data( ) = default;
		c_command_data(
			e_command_type type,
			s_command_data data,
			bool priority = false )
			: m_type( type ), m_data( data ), m_completed( false )
		{
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

	class c_pipe
	{
	public:
		c_pipe( ) = default;

		bool send_cmd( c_command_data& command ) const {
			DWORD bytes_written = 0;
			if ( !WriteFile( ipc, &command, sizeof( command ), &bytes_written, NULL ) )
				return false;
			return true;
		}

		bool get_cmd( c_command_data* data ) const {
			DWORD bytes_read = 0;
			if ( !ReadFile( ipc, data, sizeof( c_command_data ), &bytes_read, NULL ) )
				return false;
			return true;
		}

		bool create_buffer( const char* pipe_name ) {
			SECURITY_DESCRIPTOR sd;
			SECURITY_ATTRIBUTES sa;

			InitializeSecurityDescriptor( &sd, SECURITY_DESCRIPTOR_REVISION );
			SetSecurityDescriptorDacl( &sd, TRUE, NULL, FALSE );

			sa.nLength = sizeof( SECURITY_ATTRIBUTES );
			sa.lpSecurityDescriptor = &sd;
			sa.bInheritHandle = FALSE;

			ipc = CreateNamedPipeA(
				pipe_name,
				PIPE_ACCESS_DUPLEX,
				PIPE_TYPE_MESSAGE |
				PIPE_READMODE_MESSAGE |
				PIPE_WAIT,
				1,
				4096,
				4096,
				0,
				&sa
			);

			return ipc != INVALID_HANDLE_VALUE;
		}

		bool create( const char* module_name ) {
			auto pipe_name = encrypt( "\\\\.\\Pipe\\Local\\" ).decrypt( ) + std::string( module_name );
			logging::print( encrypt( "Creating pipeline at path: %s" ), pipe_name.c_str( ) );

			return create_buffer( pipe_name.c_str( ) );
		}

		bool is_connected( ) const
		{
			if ( !ipc || ipc == INVALID_HANDLE_VALUE )
				return false;

			DWORD bytes_available = 0;
			DWORD bytes_left = 0;
			DWORD instances = 0;

			if ( !PeekNamedPipe( ipc, NULL, 0, NULL, &bytes_available, &bytes_left ) )
			{
				DWORD error = GetLastError( );
				return error != ERROR_BROKEN_PIPE && error != ERROR_PIPE_NOT_CONNECTED;
			}

			return true;
		}

		bool wait_for_connection( ) const
		{
			if ( ConnectNamedPipe( ipc, NULL ) )
				return true;

			return GetLastError( ) == ERROR_PIPE_CONNECTED;
		}

	private:
		HANDLE ipc = nullptr;
	};
}