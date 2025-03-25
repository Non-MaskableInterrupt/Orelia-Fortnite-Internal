
namespace utility {
    void vigem_cleanup( PVIGEM_CLIENT client, PVIGEM_TARGET target ) {
        logging::print( encrypt( "Releasing system resources" ) );

        if ( target ) {
            vigem_target_remove( client, target );
            vigem_target_free( target );
        }

        if ( client ) {
            vigem_disconnect( client );
            vigem_free( client );
        }

        WSACleanup( );
    }
}