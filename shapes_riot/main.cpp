#include "main_window.h"

namespace shapes_riot {
namespace {

int main(int /*argc*/, char** /*argv*/)
{
    MainWindow{}.loop();
    return 0;
}

}  // namespace
}  // namespace shapes_riot


int main(int argc, char** argv)
{
    shapes_riot::main(argc, argv);
}
