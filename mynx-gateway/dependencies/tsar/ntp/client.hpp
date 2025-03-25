/*
 * ntp_client.hpp
 *
 * This is a modified version of plusangel's NTPClient class.
 *
 * GitHub: https://github.com/plusangel/NTP-client
 */

#pragma once
#define WIN32_LEAN_AND_MEAN

#include <string>
#ifdef _WIN32
#include <WinSock2.h>
using socket_t = SOCKET;
#else
#include <netinet/in.h>
using socket_t = int;
#endif

#include "../xor.hpp"

#include "../error.hpp"

namespace tsar::ntp
{
    /// <summary>
    /// A struct that defines the NTP packet format.
    /// </summary>
    struct packet_t
    {
        /// <summary>
        /// Eight bits. li, vn, and mode.
        /// li.   Two bits.   Leap indicator.
        /// vn.   Three bits. Version number of the protocol.
        /// mode. Three bits. Client will pick mode 3 for client.
        /// </summary>
        std::uint8_t li_vn_mode;

        /// <summary>
        /// Eight bits. Stratum level of the local clock.
        /// </summary>
        std::uint8_t stratum;

        /// <summary>
        /// Eight bits. Maximum interval between successive messages.
        /// </summary>
        std::uint8_t poll;

        /// <summary>
        /// Eight bits. Precision of the local clock.
        /// </summary>
        std::uint8_t precision;

        /// <summary>
        /// 32 bits. Total round trip delay time.
        /// </summary>
        std::uint32_t rootDelay;

        /// <summary>
        /// 32 bits. Max error aloud from primary clock source.
        /// </summary>
        std::uint32_t root_dispersion;

        /// <summary>
        /// 32 bits. Reference clock identifier.
        /// </summary>
        std::uint32_t ref_id;

        /// <summary>
        /// 32 bits. Reference time-stamp seconds.
        /// </summary>
        std::uint32_t ref_timestamp_sec;

        /// <summary>
        /// 32 bits. Reference time-stamp fraction of a second.
        /// </summary>
        std::uint32_t ref_timestamp_sec_frac;

        /// <summary>
        /// 32 bits. Originate time-stamp seconds.
        /// </summary>
        std::uint32_t orig_timestamp_sec;

        /// <summary>
        /// 32 bits. Originate time-stamp fraction of a second.
        /// </summary>
        std::uint32_t orig_timestamp_sec_frac;

        /// <summary>
        /// 32 bits. Received time-stamp seconds.
        /// </summary>
        std::uint32_t received_timestamp_sec;

        /// <summary>
        /// 32 bits. Received time-stamp fraction of a second.
        /// </summary>
        std::uint32_t received_timestamp_sec_frac;

        /// <summary>
        /// 32 bits and the most important field the client cares about. Transmit time-stamp seconds.
        /// </summary>
        std::uint32_t transmited_timestamp_sec;

        /// <summary>
        /// 32 bits. Transmit time-stamp fraction of a second.
        /// </summary>
        std::uint32_t transmited_timestamp_sec_frac;
    };

    /// <summary>
    /// A class that defines the NTP client.
    /// </summary>
    class client final
    {
        /// <summary>
        /// Converts from hostname to ip address.
        /// </summary>
        /// <param name="hostname">Name of the host.</param>
        /// <returns>IP address. Return empty string if can't find the ip.</returns>
        std::string hostname_to_ip( const std::string_view hostname );

        /// <summary>
        /// Build the connection. Set all the params for the socket_client.
        /// </summary>
        result_t< void > build_connection() noexcept;

        /// <summary>
        /// Close the connection. Set -1 to socket_fd.
        /// </summary>
        void close_socket();

        /// <summary>
        /// NTP server IP address
        /// </summary>
        std::string hostname;

        /// <summary>
        /// NTP server port
        /// </summary>
        std::uint16_t port;

        /// <summary>
        /// Socket file descriptor
        /// </summary>
        socket_t socket_fd;

        /// <summary>
        /// Server address data structure
        /// </summary>
        struct sockaddr_in socket_client;

        /// <summary>
        /// Delta between epoch time and ntp time
        /// </summary>
        static constexpr unsigned long long NTP_TIMESTAMP_DELTA{ 2208988800ull };

       public:
        explicit client( const std::string_view host, std::uint16_t port );
        ~client();

        /// <summary>
        /// Transmits an NTP request to the defined server and returns the timestamp.
        /// </summary>
        /// <returns>The number of seconds since 1970. Return 0 if fail</returns>
        result_t< time_t > request_time();
    };
}  // namespace tsar::ntp