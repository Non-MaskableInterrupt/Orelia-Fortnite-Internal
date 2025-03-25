#pragma once

namespace std
{
    inline
    int32_t strcmp(
        const char* string,
        const char* string_cmp
    )
    {
        while ( *string != '\0' )
        {
            if ( *string != *string_cmp )
                break;
            string++;
            string_cmp++;
        }
        return *string - *string_cmp;
    }

    inline
    int32_t wcscmp(
        const wchar_t* wstring,
        const wchar_t* wstring_cmp
    )
    {
        while ( *wstring != L'\0' )
        {
            if ( *wstring != *wstring_cmp )
                break;
            wstring++;
            wstring_cmp++;
        }
        return *wstring - *wstring_cmp;
    }
}