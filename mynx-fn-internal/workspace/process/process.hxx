#pragma once
#include <cstdint>

namespace process {
    template< typename type_t, typename field_t >
    inline
        constexpr const type_t* containing_record(
            const field_t* address,
            const field_t type_t::* field
        ) {
        auto* const base = static_cast< const type_t* >( nullptr );

        const auto offset = reinterpret_cast< std::uintptr_t >( &( base->*field ) );

        return reinterpret_cast< const type_t* >(
            reinterpret_cast< std::uintptr_t >( address ) - offset
            );
    }

    template< typename type_t, typename field_t >
    inline
        constexpr type_t* containing_record(
            field_t* address,
            const field_t type_t::* field
        ) {
        return const_cast< type_t* >(
            containing_record<type_t, field_t>(
            const_cast< const field_t* >( address ),
            field
            )
            );
    }

    class c_process {
    public:
        c_process( ) : m_peb( reinterpret_cast< peb_t* >( __readgsqword( oxorany( 0x60 ) ) ) ) { }

        bool is_valid( const wchar_t* module_name ) const {
            return find_module_entry( module_name ) != nullptr;
        }

        std::uintptr_t get_process_module( const wchar_t* module_name ) {
            const auto module = find_module_entry( module_name );
            return module ? reinterpret_cast< std::uintptr_t >( module->m_dll_base ) : 0;
        }

        std::uintptr_t get_main_module( ) const {
            return m_peb ? reinterpret_cast< std::uintptr_t >( m_peb->m_image_base_address ) : 0;
        }

        std::size_t get_module_size( const wchar_t* module_name ) const {
            const auto module = find_module_entry( module_name );
            return module ? module->m_size_of_image : 0;
        }

        struct module_info {
            std::uintptr_t base_address;
            std::size_t size;
            std::wstring name;
            std::wstring path;
        };

        std::vector<module_info> get_module_list( ) const {
            std::vector<module_info> modules;

            if ( !m_peb || !m_peb->m_ldr )
                return modules;

            const auto list_head = &m_peb->m_ldr->m_in_memory_order_module_list;
            auto current = list_head->m_flink;

            while ( current != list_head ) {
                const auto entry = containing_record(
                    current,
                    &ldr_data_table_entry_t::m_in_memory_order_links
                );

                if ( entry && entry->m_base_dll_name.m_buffer ) {
                    module_info info;
                    info.base_address = reinterpret_cast< std::uintptr_t >( entry->m_dll_base );
                    info.size = entry->m_size_of_image;
                    info.name = std::wstring(
                        entry->m_base_dll_name.m_buffer,
                        entry->m_base_dll_name.m_length / sizeof( wchar_t )
                    );
                    info.path = std::wstring(
                        entry->m_full_dll_name.m_buffer,
                        entry->m_full_dll_name.m_length / sizeof( wchar_t )
                    );

                    modules.push_back( std::move( info ) );
                }

                current = current->m_flink;
            }

            return modules;
        }

        std::uintptr_t get_export( const wchar_t* module_name, const char* export_name ) {
            const auto module_base = get_process_module( module_name );
            return module_base ? get_export_address( module_base, export_name ) : 0;
        }

        std::wstring get_module_file_name( const wchar_t* module_name ) const {
            const auto module = find_module_entry( module_name );
            if ( !module || !module->m_full_dll_name.m_buffer )
                return L"";

            return std::wstring(
                module->m_full_dll_name.m_buffer,
                module->m_full_dll_name.m_length / sizeof( wchar_t )
            );
        }

        std::wstring get_command_line( ) const {
            if ( !m_peb || !m_peb->m_process_parameters )
                return L"";

            const auto params = m_peb->m_process_parameters;
            if ( !params->m_command_line.m_buffer )
                return L"";

            return std::wstring(
                params->m_command_line.m_buffer,
                params->m_command_line.m_length / sizeof( wchar_t )
            );
        }

        bool is_address_in_module( std::uintptr_t address, const wchar_t* module_name ) const {
            const auto module = find_module_entry( module_name );
            if ( !module )
                return false;

            const auto base = reinterpret_cast< std::uintptr_t >( module->m_dll_base );
            return ( address >= base ) && ( address < ( base + module->m_size_of_image ) );
        }

        std::uintptr_t get_module_by_address( std::uintptr_t address ) const {
            if ( !m_peb || !m_peb->m_ldr )
                return 0;

            const auto list_head = &m_peb->m_ldr->m_in_memory_order_module_list;
            auto current = list_head->m_flink;

            while ( current != list_head ) {
                const auto module = containing_record(
                    current,
                    &ldr_data_table_entry_t::m_in_memory_order_links
                );

                if ( module ) {
                    const auto base = reinterpret_cast< std::uintptr_t >( module->m_dll_base );
                    const auto size = module->m_size_of_image;

                    if ( address >= base && address < ( base + size ) )
                        return base;
                }

                current = current->m_flink;
            }

            return 0;
        }

    private:
        peb_t* m_peb;

        const ldr_data_table_entry_t* find_module_entry( const wchar_t* module_name ) const {
            if ( !m_peb || !m_peb->m_ldr )
                return nullptr;

            const auto list_head = &m_peb->m_ldr->m_in_memory_order_module_list;
            auto current = list_head->m_flink;

            while ( current != list_head ) {
                const auto module = containing_record(
                    current,
                    &ldr_data_table_entry_t::m_in_memory_order_links
                );

                if ( module && module->m_base_dll_name.m_buffer ) {
                    if ( std::wcsstr( module->m_base_dll_name.m_buffer, module_name ) )
                        return module;
                }

                current = current->m_flink;
            }

            return nullptr;
        }

        std::uintptr_t get_export_address( std::uintptr_t module_base, const char* export_name ) const {
            const auto dos_header = reinterpret_cast< IMAGE_DOS_HEADER* >( module_base );
            if ( dos_header->e_magic != IMAGE_DOS_SIGNATURE )
                return 0;

            const auto nt_headers = reinterpret_cast< IMAGE_NT_HEADERS64* >( module_base + dos_header->e_lfanew );
            if ( nt_headers->Signature != IMAGE_NT_SIGNATURE )
                return 0;

            const auto& export_dir = nt_headers->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ];
            if ( !export_dir.VirtualAddress || !export_dir.Size )
                return 0;

            const auto export_table = reinterpret_cast< IMAGE_EXPORT_DIRECTORY* >( module_base + export_dir.VirtualAddress );
            const auto names = reinterpret_cast< DWORD* >( module_base + export_table->AddressOfNames );
            const auto ordinals = reinterpret_cast< WORD* >( module_base + export_table->AddressOfNameOrdinals );
            const auto functions = reinterpret_cast< DWORD* >( module_base + export_table->AddressOfFunctions );

            for ( DWORD i = 0; i < export_table->NumberOfNames; ++i ) {
                const auto name = reinterpret_cast< const char* >( module_base + names[ i ] );
                if ( std::strcmp( name, export_name ) == 0 ) {
                    const auto ordinal = ordinals[ i ];
                    if ( ordinal >= export_table->NumberOfFunctions )
                        return 0;

                    return module_base + functions[ ordinal ];
                }
            }

            return 0;
        }
    };
}