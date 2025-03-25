
namespace engine {
	class u_kismet_string_library : public u_blueprint_function_library {
	public:
        fstring concat_str_str( fstring a, fstring b ) {
            static u_function* function = nullptr;
            if ( !function ) {
                auto fn_name = encrypt( L"KismetStringLibrary.Concat_StrStr" );
                function = find_object<u_function*>( fn_name.decrypt( ) );
                fn_name.clear( );
            }

            struct {
                fstring a;
                fstring b;
                fstring return_value;
            } params{
                a,
                b
            };

            this->process_event( function, &params );

            return params.return_value;
        }

        bool contains( fstring search_in, fstring substring, bool use_case, bool search_from_end ) {
            static u_function* function = nullptr;
            if ( !function ) {
                auto fn_name = encrypt( L"KismetStringLibrary.Contains" );
                function = find_object<u_function*>( fn_name.decrypt( ) );
                fn_name.clear( );
            }

            struct {
                fstring search_in;
                fstring substring;
                bool use_case;
                bool search_from_end;
                bool return_value;
            } params{
                search_in,
                substring,
                use_case,
                search_from_end
            };

            this->process_event( function, &params );

            return params.return_value;
        }

        fname conv_string_to_name( fstring in_string ) {
            static u_function* function = nullptr;
            if ( !function ) {
                auto fn_name = encrypt( L"KismetStringLibrary.Conv_StringToName" );
                function = find_object<u_function*>( fn_name.decrypt( ) );
                fn_name.clear( );
            }

            struct {
                fstring in_string;
                fname return_value;
            } params{
                in_string
            };

            this->process_event( function, &params );
            return params.return_value;
        }

        fstring conv_name_to_string( fname in_name ) {
            static u_function* function = nullptr;
            if ( !function ) {
                auto fn_name = encrypt( L"KismetStringLibrary.Conv_NameToString" );
                function = find_object<u_function*>( fn_name.decrypt( ) );
                fn_name.clear( );
            }

            struct {
                fname in_name;
                fstring return_value;
            } params{
                in_name
            };

            this->process_event( function, &params );
            return params.return_value;
        }

        fstring build_string_name( fstring append_to, fstring prefix, fname in_name, fstring suffix ) {
            static u_function* function = nullptr;
            if ( !function ) {
                auto fn_name = encrypt( L"KismetStringLibrary.BuildString_Name" );
                function = find_object<u_function*>( fn_name.decrypt( ) );
                fn_name.clear( );
            }

            struct {
                fstring append_to;
                fstring prefix;
                fname in_name;
                fstring suffix;
                fstring return_value;
            } params{
                append_to,
                prefix,
                in_name,
                suffix
            };

            this->process_event( function, &params );
            return params.return_value;
        }

        fstring build_string_int( fstring append_to, fstring prefix, int in_int, fstring suffix ) {
            static u_function* function = nullptr;
            if ( !function ) {
                auto fn_name = encrypt( L"KismetStringLibrary.BuildString_Int" );
                function = find_object<u_function*>( fn_name.decrypt( ) );
                fn_name.clear( );
            }

            struct {
                fstring append_to;
                fstring prefix;
                int in_int;
                fstring suffix;
                fstring return_value;
            } params{
                append_to,
                prefix,
                in_int,
                suffix
            };

            this->process_event( function, &params );
            return params.return_value;
        }

        int32_t len( fstring s ) {
            static u_function* function = nullptr;
            if ( !function ) {
                auto fn_name = encrypt( L"KismetStringLibrary.Len" );
                function = find_object<u_function*>( fn_name.decrypt( ) );
                fn_name.clear( );
            }

            struct {
                fstring s;
                int32_t return_value;
            } params{
                s
            };

            this->process_event( function, &params );
            return params.return_value;
        }

        fstring to_lower( fstring source_string ) {
            static u_function* function = nullptr;
            if ( !function ) {
                auto fn_name = encrypt( L"KismetStringLibrary.ToLower" );
                function = find_object<u_function*>( fn_name.decrypt( ) );
                fn_name.clear( );
            }

            struct {
                fstring source_string;
                fstring return_value;
            } params{
                source_string
            };

            this->process_event( function, &params );
            return params.return_value;
        }

        fstring build_string_double( fstring append_to, fstring prefix, double in_double, fstring suffix ) {
            static u_function* function = nullptr;
            if ( !function ) {
                auto fn_name = encrypt( L"KismetStringLibrary.BuildString_Double" );
                function = find_object<u_function*>( fn_name.decrypt( ) );
                fn_name.clear( );
            }

            struct {
                fstring append_to;
                fstring prefix;
                double in_double;
                fstring suffix;
                fstring return_value;
            } params{
                append_to,
                prefix,
                in_double,
                suffix
            };

            this->process_event( function, &params );
            return params.return_value;
        }
	};
}