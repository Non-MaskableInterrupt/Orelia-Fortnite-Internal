#include <expected>
#include <system_error>
#include "../xor.hpp"

namespace tsar::ntp
{
    /// <summary>
    /// The error codes for the NTP client.
    /// </summary>
    enum class error_code_t
    {
        /// <summary>
        /// The NTP client failed to build a connection to the server.
        /// </summary>
        failed_to_build_connection_t,

        /// <summary>
        /// The NTP client failed to resolve the hostname.
        /// </summary>
        failed_to_resolve_hostname_t,

        /// <summary>
        /// The NTP client failed to send a packet to the server.
        /// </summary>
        failed_to_send_packet_t,

        /// <summary>
        /// Failed to receive a packet from the server.
        /// </summary>
        failed_to_receive_packet_t
    };

    /// <summary>
    /// The error category for the NTP client.
    /// </summary>
    class error_category : public std::error_category
    {
       public:
        /// <summary>
        /// Gets the singleton instance of the error category.
        /// </summary>
        static const error_category& get() noexcept;

        /// <summary>
        /// Gets the name of the error category.
        /// </summary>
        const char* name() const noexcept override;

        /// <summary>
        /// Gets the message for the specified error code.
        /// </summary>
        std::string message( int ev ) const override;
    };
}  // namespace tsar::ntp