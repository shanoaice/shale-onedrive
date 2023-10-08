#include <string>
#include <chrono>
#include <iostream>
#include <boost/format.hpp>
#include <boost/url.hpp>
#include <cpr/cpr.h>
#include "simdjson.h"

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
        std::chrono::time_point<std::chrono::system_clock> expires;

    public:
        graph_token() {}
        graph_token(graph_endpoint endpoint, string tenant)
        {
            string app_permission = cpr::util::urlEncode("User.Read Files.ReadWrite offline_access");
            string redirect_url = cpr::util::urlEncode("https://login.microsoftonline.com/common/oauth2/nativeclient");
            auto base_url_template = boost::format("%1/%2");
            auto auth_url_suffix = (boost::format("/oauth2/v2.0/authorize?clientid=%1&response_type=code&scope=%2&redirect_uri=%3") % APP_CLIENT_ID % app_permission % redirect_url).str();
            string base_url, auth_url;
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
            std::cout << "Authorize this app visiting:\n\n"
                      << auth_url << "\n\nEnter the response URI: " << std::endl;

            string response_uri;
            std::cin >> response_uri;

            auto response_uri_parsed_result = boost::urls::parse_uri(response_uri);

            while (response_uri_parsed_result.has_error())
            {
                std::cout << "Cannot parse entered response URI,\nPlease enter again: " << std::flush;
                std::cin >> response_uri;
                response_uri_parsed_result = boost::urls::parse_uri(response_uri);
            }

            string code;
            for (auto param : response_uri_parsed_result.value().params())
            {
                if (param.key == "code")
                {
                    code = param.value;
                    break;
                }
            }

            auto time_now = std::chrono::system_clock::now();
            string token_url = base_url + "/oauth2/v2.0/token";
            cpr::Response token_r = cpr::Post(cpr::Url{token_url},
                                              cpr::Payload{
                                                {"client_id", APP_CLIENT_ID},
                                                {"code", code},
                                                {"grant_type", "authorization_code"},
                                                {"scope", app_permission},
                                                {"redirect_uri", redirect_url}});
            string token_response_plain = std::move(token_r.text);
            token_response_plain.reserve(simdjson::SIMDJSON_PADDING);
            simdjson::ondemand::parser json_parser;
            simdjson::ondemand::document token_response = json_parser.iterate(token_response_plain);

            int64_t expires_in = token_response["expires_in"].get<int64_t>();

            this->access_token = token_response["access_token"].get<string>();
            this->refresh_token = token_response["refresh_token"].get<string>();
            this->expires = time_now + std::chrono::seconds(expires_in);
        }
    };
}
