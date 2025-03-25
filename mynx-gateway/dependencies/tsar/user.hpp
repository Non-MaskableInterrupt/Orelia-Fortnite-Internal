#pragma once

#include <chrono>
#include <optional>
#include <string>


#include "xor.hpp"
#include "tsar.hpp"

namespace tsar
{
    /// <summary>
    /// Represents a subscription in the TSAR API.
    /// </summary>
    struct subscription_t
    {
        /// <summary>
        /// The subscription identifier.
        /// </summary>
        std::string id;

        /// <summary>
        /// Timestamp of when the subscription expires
        /// </summary>
        std::optional< std::chrono::system_clock::time_point > expires;

        /// <summary>
        /// The tier of the user's subscription, set by the app's purchase flow. Default is 0.
        /// </summary>
        std::uint32_t tier;
    };

    /// <summary>
    /// Represents a user in the TSAR system.
    /// </summary>
    class user
    {
        std::string session, session_key;

        result_t< nlohmann::json > api_query( const std::string_view endpoint ) const noexcept;

        template< typename T >
        result_t< T > api_query( const std::string_view endpoint ) const noexcept;

       public:
        std::string id;
        std::optional< std::string > username, avatar;

        subscription_t subscription;

        /// <summary>
        /// Creates a new user from the specified JSON data. Throws if the JSON data is invalid.
        /// </summary>
        /// <param name="json">The json data.</param>
        explicit user( const nlohmann::json& json );

        /// <summary>
        /// The default constructor.
        /// </summary>
        explicit user() = default;

        /// <summary>
        /// Performs a heartbeat request to the TSAR API for the current session.
        /// </summary>
        result_t< void > heartbeat() const noexcept;
    };

    template< typename T >
    inline result_t< T > user::api_query( const std::string_view endpoint ) const noexcept
    {
        const auto result = api_query( endpoint );

        if ( result )
        {
            try
            {
                return T{ *result };
            }
            catch ( const error& e )
            {
                return std::unexpected( e );
            }
        }

        return std::unexpected( result.error() );
    }
}  // namespace tsar