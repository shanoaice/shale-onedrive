#include <chrono>
#include <string>

namespace shale::auth {
enum graph_endpoint { Common, US_L4, US_L5, DE, CN };

struct graph_token {
private:
  std::string access_token;
  std::string refresh_token;
  std::string scope;
  std::chrono::time_point<std::chrono::system_clock> access_token_expiry_time;
  std::chrono::time_point<std::chrono::system_clock> refresh_token_expiry_time;
  std::string base_url;
  std::string auth_url_suffix;
  std::string redirect_url;

public:
  graph_token();
  void authorize_app(std::chrono::system_clock::time_point now);
  void init(graph_endpoint endpoint, std::string tenant);
  void renew_token(std::chrono::system_clock::time_point now);
  std::string_view get_token();
  std::string export_session();
  void restore_session(std::string session_data);
};
} // namespace shale::auth