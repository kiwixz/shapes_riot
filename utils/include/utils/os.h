#pragma once

#include <string>

namespace utils {

using ThreadId = unsigned long long;

[[nodiscard]] ThreadId thread_id();

[[nodiscard]] std::string thread_name();
void set_thread_name(const std::string& name);

}  // namespace utils
