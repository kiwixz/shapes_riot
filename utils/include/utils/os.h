#pragma once

#include <string>

namespace utils {

using ThreadId = unsigned long long;

ThreadId thread_id();

std::string thread_name();
void set_thread_name(const std::string& name);

}  // namespace utils
