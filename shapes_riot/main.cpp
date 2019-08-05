#include "main_window.h"
#include "utils/exception.h"
#include <fmt/printf.h>

namespace shapes_riot {
namespace {

int main(int /*argc*/, char** /*argv*/)
{
    try {
        MainWindow{}.loop();
        return 0;
    }
    catch (const utils::Exception& ex) {
        fmt::print("fatal exception: {}\nfrom: {}\n", ex.what(), ex.from());
    }
    catch (const std::exception& ex) {
        fmt::print("fatal exception: {}\n", ex.what());
    }
    return 1;
}

}  // namespace
}  // namespace shapes_riot


int main(int argc, char** argv)
{
    shapes_riot::main(argc, argv);
}
