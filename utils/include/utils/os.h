#pragma once

#include <filesystem>
#include <string>
#include <string_view>

namespace utils {

using ThreadId = unsigned long long;

std::filesystem::path app_directory(std::string_view app_name);

ThreadId thread_id();

std::string thread_name();
void set_thread_name(const std::string& name);

}  // namespace utils
