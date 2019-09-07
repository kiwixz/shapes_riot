#include "health_bar.h"

namespace shapes_riot {

HealthBar::HealthBar(int hp, int max_hp) :
    hp_{hp}, max_hp_{max_hp}
{}

gfx::DrawList HealthBar::draw() const
{
    return gfx::DrawList();
}

void HealthBar::tick(double /*delta*/, int hp)
{
    hp_ = hp;
}

}  // namespace shapes_riot
