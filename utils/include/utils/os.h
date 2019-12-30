#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace utils {

using ThreadId = unsigned long long;

std::filesystem::path app_directory(std::string_view app_name);

ThreadId thread_id();

std::string thread_name();
void set_thread_name(const std::string& name);

std::vector<std::string> stacktrace();

}  // namespace utils
