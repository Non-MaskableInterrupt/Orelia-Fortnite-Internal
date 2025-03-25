#pragma once

namespace trap
{
	class c_trap
	{
	public:
		c_trap( ) = default;

		bool setup( )
		{
			process::c_process proc;
			auto m_base_address = proc.get_process_module( encrypt( L"DiscordHook64.dll" ) );
			if ( !m_base_address )
				return false;
			return true;
		}

		int init( void ) {
			return reinterpret_cast< int( __cdecl* )( void ) >( m_base_address + oxorany( 0x76680 ) )( );
		}

		int apply_queued( void ) {
			return reinterpret_cast< int( __cdecl* )( void ) >( m_base_address + oxorany( 0x79790 ) )( );
		}

		int queue_enable_hook( void* target ) {
			return reinterpret_cast< int( __cdecl* )( void* ) >( m_base_address + oxorany( 0x79690 ) )( target );
		}

		int create_hook( void* target, void* detor, void** original ) {
			return reinterpret_cast< int( __cdecl* )( void*, void*, void** ) >( m_base_address + oxorany( 0x78D40 ) )( target, detor, original );
		}

		std::uintptr_t get_discordhook64( ) const
		{
			return m_base_address;
		}

	private:
		std::uintptr_t m_base_address = 0;
	};
}