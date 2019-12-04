#pragma once

#include <string>
#include <string_view>

namespace utils {

using ThreadId = unsigned long long;

[[nodiscard]] ThreadId thread_id();

[[nodiscard]] std::string thread_name();
void set_thread_name(std::string_view name);

}  // namespace utils
