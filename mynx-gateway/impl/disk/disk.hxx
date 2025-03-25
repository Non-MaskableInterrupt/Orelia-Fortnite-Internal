namespace disk {
    void open_file( const std::string& file, std::vector<uint8_t>& data ) {
        std::ifstream fstr( file, std::ios::binary );
        fstr.unsetf( std::ios::skipws );
        fstr.seekg( 0, std::ios::end );

        const auto file_size = fstr.tellg( );

        fstr.seekg( NULL, std::ios::beg );
        data.reserve( static_cast< uint32_t >( file_size ) );
        data.insert( data.begin( ), std::istream_iterator<uint8_t>( fstr ), std::istream_iterator<uint8_t>( ) );
    }

    bool write_signed_file( const wchar_t* file_name, unsigned char* signed_file, size_t file_size ) noexcept {
        wchar_t system_path[ MAX_PATH ];
        if ( !GetSystemDirectoryW( system_path, MAX_PATH ) )
            return false;

        std::wstring full_path = system_path;
        full_path += L"\\";
        full_path += file_name;

        HANDLE h_file = CreateFileW(
            full_path.c_str( ),
            GENERIC_WRITE,
            0,
            nullptr,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH,
            nullptr
        );

        if ( h_file == INVALID_HANDLE_VALUE )
            return false;

        DWORD bytes_written;
        bool success = WriteFile(
            h_file,
            signed_file,
            static_cast< DWORD >( file_size ),
            &bytes_written,
            nullptr
        );

        CloseHandle( h_file );
        return success && ( bytes_written == file_size );
    }
}