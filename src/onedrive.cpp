#include <map>
#include <set>
#include <string>

#include "auth.h"
#include "cpr/cpr.h"
#include "simdjson.h"

namespace shale::onedrive {
struct session {
private:
  shale::auth::graph_token token;
  std::set<std::string> in_use;
  std::set<std::string> conflicting_update;

public:
  session(shale::auth::graph_token token) { this->token = token; }
  void mark_in_use(std::string path) { this->in_use.insert(path); }
};
} // namespace shale::onedrive