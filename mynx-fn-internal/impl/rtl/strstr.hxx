#pragma once

namespace std
{
    inline
    const char* strstr(
        const char* string,
        const char* string_sub
    )
    {
        while ( *string != '\0' )
        {
            if ( strcmp( string, string_sub ) == 0 )
                return string;
            string++;
        }
        return {};
    }

    inline
    const wchar_t* wcsstr(
        const wchar_t* wchar_str,
        const wchar_t* wchar_substr
    )
    {
        while ( *wchar_str != L'\0' )
        {
            if ( wcscmp( wchar_str, wchar_substr ) == 0 )
                return wchar_str;
            wchar_str++;
        }
        return {};
    }
}