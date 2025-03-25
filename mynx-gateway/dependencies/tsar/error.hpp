#pragma once

#include <expected>
#include <system_error>

#include "xor.hpp"

#include "ntp/error.hpp"

namespace tsar
{
    /// <summary>
    /// The error codes for the TSAR API.
    /// </summary>
    enum class error_code_t
    {
        /// <summary>
        /// The App ID provided is not in the right format. Must be in UUID (00000000-0000-0000-0000-000000000000) format.
        /// </summary>
        invalid_app_id_t,

        /// <summary>
        /// The Client Key provided is not in the right format. Must be in Base64 (MFk...qA==) format.
        /// </summary>
        invalid_client_key_t,

        /// <summary>
        /// Failed to get the user's HWID.
        /// </summary>
        failed_to_get_hwid_t,

        /// <summary>
        /// Failed to open the user's default browser.
        /// </summary>
        failed_to_open_browser_t,

        /// <summary>
        /// Request to the TSAR server failed, server may be down.
        /// </summary>
        request_failed_t,

        /// <summary>
        /// The APP ID passed does not match to a TSAR APP.
        /// </summary>
        app_not_found_t,

        /// <summary>
        /// The APP you're trying to use is paused by the developer.
        /// </summary>
        app_paused_t,

        /// <summary>
        /// The TSAR API returned a 401: Unauthorized status code.
        /// This means that the user's HWID did not match to a subscription object.
        /// </summary>
        unauthorized_t,

        /// <summary>
        /// TSAR server had an error and did not return an OK status.
        /// </summary>
        server_error_t,

        /// <summary>
        /// The request passed to the TSAR server was bad.
        /// </summary>
        bad_request_t,

        /// <summary>
        /// The current user has been rate limited.
        /// </summary>
        rate_limited_t,

        /// <summary>
        /// Failed to parse returned body into JSON.
        /// </summary>
        failed_to_parse_body_t,

        /// <summary>
        /// Failed to get the `data` field from the parsed JSON body.
        /// </summary>
        failed_to_get_data_t,

        /// <summary>
        /// Failed to get the `signature` field from the parsed JSON body.
        /// </summary>
        failed_to_get_signature_t,

        /// <summary>
        /// Failed to decode the `data` field from the parsed JSON body.
        /// </summary>
        failed_to_decode_data_t,

        /// <summary>
        /// Failed to decode the `signature` field from the parsed JSON body.
        /// </summary>
        failed_to_decode_signature_t,

        /// <summary>
        /// Failed to decode the client key from base64.
        /// </summary>
        failed_to_decode_public_key_t,

        /// <summary>
        /// Failed to decode the session key from base64.
        /// </summary>
        failed_to_decode_session_key_t,

        /// <summary>
        /// Failed to parse the `data` field into JSON.
        /// </summary>
        failed_to_parse_data_t,

        /// <summary>
        /// Failed to get the `timestamp` field.
        /// </summary>
        failed_to_get_timestamp_t,

        /// <summary>
        /// Failed to parse the `timestamp` field into uint64_t.
        /// </summary>
        failed_to_parse_timestamp_t,

        /// <summary>
        /// Local HWID and HWID returned from server dont match.
        /// </summary>
        hwid_mismatch_t,

        /// <summary>
        /// The response is old. Data may have been tampered with.
        /// </summary>
        old_response_t,

        /// <summary>
        /// Signature is not authentic. Data may have been tampered with.
        /// </summary>
        invalid_signature_t,

        /// <summary>
        /// An unexpected error occurred.
        /// </summary>
        unexpected_error_t,
        
        /// <summary>
        /// The program hash is not authorized.
        /// </summary>
        hash_unauthorized_t,
    };

    /// <summary>
    /// The error category for the TSAR API.
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

    /// <summary>
    /// Represents an error in the TSAR API.
    /// </summary>
    class error : public std::system_error
    {
       public:
        /// <summary>
        /// Creates a new error with the specified error code.
        /// </summary>
        /// <param name="code">The code.</param>
        explicit error( std::error_code code ) noexcept;

        /// <summary>
        /// Creates a new error with the specified error code value.
        /// </summary>
        /// <param name="code">The error code value.</param>
        explicit error( error_code_t code ) noexcept;

        /// <summary>
        /// Creates a new error with the specified error code value.
        /// </summary>
        /// <param name="code">The error code value.</param>
        explicit error( ntp::error_code_t code ) noexcept;

        /// <summary>
        /// Checks if the error code is equal to the specified code.
        /// </summary>
        bool operator==( const error_code_t& c ) const noexcept;

        /// <summary>
        /// Checks if the error code is not equal to the specified code.
        /// </summary>
        bool operator!=( const error_code_t& c ) const noexcept;
    };

    /// <summary>
    /// The result type for the TSAR API.
    /// </summary>
    template< typename T >
    using result_t = std::expected< T, error >;

}  // namespace tsar