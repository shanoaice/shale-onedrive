#pragma once

#include <map>
#include <set>
#include <string>

#include "auth.h"

#ifndef SHALE_ONEDRIVE
#define SHALE_ONEDRIVE
namespace shale::onedrive {
struct session {
private:
  shale::auth::graph_token token;
  std::set<std::string> in_use;
  std::set<std::string> conflicting_update;

public:
  session(shale::auth::graph_token token);
  void mark_in_use(std::string path);
  bool check_conflict(std::string path);
};
} // namespace shale::onedrive
#endif