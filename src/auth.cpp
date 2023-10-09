#include <string>
#include <chrono>
#include <iostream>
#include <boost/format.hpp>
#include <boost/url.hpp>
#include "cpr/cpr.h"
#include "simdjson.h"
#include "yyjson.h"

#include "constants.h"

typedef std::string string;

namespace shale::auth
{
    enum graph_endpoint
    {
        Common,
        US_L4,
        US_L5,
        DE,
        CN
    };

    struct graph_token
    {
    private:
        string access_token;
        string refresh_token;
        string scope{cpr::util::urlEncode("User.Read Files.ReadWrite offline_access")};
        std::chrono::time_point<std::chrono::system_clock> access_token_expiry_time;
        std::chrono::time_point<std::chrono::system_clock> refresh_token_expiry_time;
        string base_url;
        string auth_url_suffix;
        string redirect_url{cpr::util::urlEncode("https://login.microsoftonline.com/common/oauth2/nativeclient")};

    public:
        graph_token()
        {
            auth_url_suffix = (boost::format("/oauth2/v2.0/authorize?clientid=%1&response_type=code&scope=%2&redirect_uri=%3") % APP_CLIENT_ID % scope % redirect_url).str();
        }
        void authorize_app(std::chrono::system_clock::time_point now)
        {
            std::cout << "Authorize this app visiting:\n\n"
                      << base_url + auth_url_suffix << "\n\nEnter the response URI: " << std::endl;

            string response_uri;
            std::cin >> response_uri;

            auto response_uri_parsed_result = boost::urls::parse_uri(response_uri);

            // if url parsing fails, e.g. if user made an error copying/pasting the response URI
            // keep re-asking the user for a correct response URI instead of bailing out
            while (response_uri_parsed_result.has_error())
            {
                std::cout << "Cannot parse entered response URI,\nPlease enter again: " << std::flush;
                std::cin >> response_uri;
                response_uri_parsed_result = boost::urls::parse_uri(response_uri);
            }

            // obtain the auth code from the response URI
            string code;
            for (auto param : response_uri_parsed_result.value().params())
            {
                if (param.key == "code")
                {
                    code = param.value;
                    break;
                }
            }

            auto time_now = now;
            string token_url = base_url + "/oauth2/v2.0/token";
            // call api to get the access token
            cpr::Response token_r = cpr::Post(cpr::Url{token_url},
                                              cpr::Payload{
                                                  {"client_id", APP_CLIENT_ID},
                                                  {"code", code},
                                                  {"grant_type", "authorization_code"},
                                                  {"scope", scope},
                                                  {"redirect_uri", redirect_url}});
            string token_response_plain = std::move(token_r.text);
            token_response_plain.reserve(simdjson::SIMDJSON_PADDING);
            simdjson::ondemand::parser json_parser;
            // parse the response
            simdjson::ondemand::document token_response = json_parser.iterate(token_response_plain);

            int64_t expires_in = token_response["expires_in"].get_int64();

            this->access_token = string(token_response["access_token"].get_string().value());
            this->refresh_token = string(token_response["refresh_token"].get_string().value());
            // calculate the expiration time to be checked when calling get_token();
            this->access_token_expiry_time = time_now + std::chrono::seconds(expires_in);
            this->refresh_token_expiry_time = time_now + std::chrono::days(90);
        }
        graph_token(graph_endpoint endpoint, string tenant)
        {
            string app_permission = cpr::util::urlEncode("User.Read Files.ReadWrite offline_access");
            string redirect_url = cpr::util::urlEncode("https://login.microsoftonline.com/common/oauth2/nativeclient");
            auto base_url_template = boost::format("%1/%2");
            auto auth_url_suffix = (boost::format("/oauth2/v2.0/authorize?clientid=%1&response_type=code&scope=%2&redirect_uri=%3") % APP_CLIENT_ID % app_permission % redirect_url).str();
            string base_url, auth_url;
            // construct auth url that user visits to grant access to their account
            switch (endpoint)
            {
            case Common:
                base_url = (base_url_template % GLOBAL_GRAPH_ENDPOINT % tenant).str();
                auth_url = base_url + auth_url_suffix;
                break;
            case US_L4:
                base_url = (base_url_template % US_L4_GRAPH_ENDPOINT % tenant).str();
                auth_url = base_url + auth_url_suffix;
                break;
            case US_L5:
                base_url = (base_url_template % US_L5_GRAPH_ENDPOINT % tenant).str();
                auth_url = base_url + auth_url_suffix;
                break;
            case DE:
                base_url = (base_url_template % DE_GRAPH_ENDPOINT % tenant).str();
                auth_url = base_url + auth_url_suffix;
                break;
            case CN:
                base_url = (base_url_template % CN_GRAPH_ENDPOINT % tenant).str();
                auth_url = base_url + auth_url_suffix;
                break;
            }
            this->base_url = base_url;

            authorize_app(std::chrono::system_clock::now());
        }
        void renew_token(std::chrono::system_clock::time_point now)
        {
            string refresh_url = this->base_url + "/oauth2/v2.0/token";
            cpr::Response token_r = cpr::Post(cpr::Url{refresh_url},
                                              cpr::Payload{
                                                  {"client_id", APP_CLIENT_ID},
                                                  {"grant_type", "refresh_token"},
                                                  {"refresh_token", this->refresh_token}});
            string token_response_plain = std::move(token_r.text);
            token_response_plain.reserve(simdjson::SIMDJSON_PADDING);
            simdjson::ondemand::parser json_parser;
            // parse the response
            simdjson::ondemand::document token_response = json_parser.iterate(token_response_plain);

            this->access_token = string(token_response["access_token"].get_string().value());
            int64_t expires_in = token_response["expires_in"].get_int64();
            this->access_token_expiry_time = now + std::chrono::seconds(expires_in);
            this->refresh_token = string(token_response["refresh_token"].get_string().value());
            this->refresh_token_expiry_time = now + std::chrono::days(90);
        }
        std::string_view get_token()
        {
            auto time_now = std::chrono::system_clock::now();
            // check if the token has expired
            // if not, return the string_view of the token directly
            if (this->access_token_expiry_time > time_now)
            {
                return std::string_view{this->access_token};
            }
            // if it has expired, call api to renew the token
            renew_token(time_now);

            return std::string_view{this->access_token};
        }
    };
}
