#include <chrono>
#include <dependencies/skcrypt/skcrypt.h>
#include <iostream>

namespace logging
{
    template<typename... Args>
    inline void print(
        const char* format,
        Args... args
    ) {
        auto now = std::chrono::system_clock::now( );
        std::time_t time = std::chrono::system_clock::to_time_t( now );

        tm local_tm;
        localtime_s( &local_tm, &time );

        printf( encrypt( "[%02d/%02d/%04d %02d:%02d:%02d] " ),
            local_tm.tm_mon + 1,
            local_tm.tm_mday,
            local_tm.tm_year + 1900,
            local_tm.tm_hour,
            local_tm.tm_min,
            local_tm.tm_sec );

        printf( format, args... );
        printf( encrypt( "\n" ) );
    }

    inline void print(
        const char* format
    ) {
        auto now = std::chrono::system_clock::now( );
        std::time_t time = std::chrono::system_clock::to_time_t( now );

        tm local_tm;
        localtime_s( &local_tm, &time );

        printf( encrypt( "[%02d/%02d/%04d %02d:%02d:%02d] %s\n" ),
            local_tm.tm_mon + 1,
            local_tm.tm_mday,
            local_tm.tm_year + 1900,
            local_tm.tm_hour,
            local_tm.tm_min,
            local_tm.tm_sec,
            format );
    }

    template<typename... Args>
    inline std::string input(
        const char* prompt_format,
        Args... args
    ) {
        auto now = std::chrono::system_clock::now( );
        std::time_t time = std::chrono::system_clock::to_time_t( now );

        tm local_tm;
        localtime_s( &local_tm, &time );

        printf( encrypt( "[%02d/%02d/%04d %02d:%02d:%02d] " ),
            local_tm.tm_mon + 1,
            local_tm.tm_mday,
            local_tm.tm_year + 1900,
            local_tm.tm_hour,
            local_tm.tm_min,
            local_tm.tm_sec );

        printf( prompt_format, args... );
        fflush( stdout );

        std::string input_str;
        std::getline( std::cin, input_str );

        return input_str;
    }

    inline std::string input(
        const char* prompt
    ) {
        auto now = std::chrono::system_clock::now( );
        std::time_t time = std::chrono::system_clock::to_time_t( now );

        tm local_tm;
        localtime_s( &local_tm, &time );

        printf( encrypt( "[%02d/%02d/%04d %02d:%02d:%02d] %s" ),
            local_tm.tm_mon + 1,
            local_tm.tm_mday,
            local_tm.tm_year + 1900,
            local_tm.tm_hour,
            local_tm.tm_min,
            local_tm.tm_sec,
            prompt );

        fflush( stdout );

        std::string input_str;
        std::getline( std::cin, input_str );

        return input_str;
    }
}