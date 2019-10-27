#include "utils/paths.h"
#include "utils/exception.h"
#include "utils/scope_exit.h"

#ifdef _WIN32
#    include <shlobj.h>
#else
#    include <pwd.h>
#    include <unistd.h>
#    include <cstdlib>
#    include <vector>
#endif

namespace utils {

std::filesystem::path get_kiwixz_home(std::string_view app_name)
{
    static std::filesystem::path base_path = [] {
#ifdef _WIN32
        wchar_t* base;
        if (SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_CREATE, nullptr, &base) != S_OK)
            throw MAKE_EXCEPTION("could not get appdata");
        ScopeExit free_base{[&] {
            CoTaskMemFree(base);
        }};
        return std::filesystem::path{base} / "kiwixz";
#else
        const char* base;
        std::vector<char> buffer;
        if (!(base = std::getenv("HOME"))) {
            buffer.resize(4096);
            passwd pw;
            passwd* result;
            while (getpwuid_r(getuid(), &pw, buffer.data(), buffer.size(), &result) == ERANGE)
                buffer.resize(buffer.size() * 2);
            if (!result)
                throw MAKE_EXCEPTION("could not get HOME nor passwd of user");
            base = result->pw_dir;  // pointee is in buffer
            if (!base)
                throw MAKE_EXCEPTION("user has no home");
        }
        return std::filesystem::path{base} / ".kiwixz";
#endif
    }();

    std::filesystem::path path = base_path / app_name;
    std::filesystem::create_directories(path);
    return path;
}

}  // namespace utils
