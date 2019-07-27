#include "main_menu.h"
#include <glad/glad.h>

namespace shapes_riot {

MainMenu::MainMenu(ScreenStack& screens) :
    screens_{&screens}
{}

void MainMenu::tick(double delta)
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void MainMenu::on_focus()
{}

void MainMenu::on_unfocus()
{}

}  // namespace shapes_riot
