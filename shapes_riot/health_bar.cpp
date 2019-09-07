#include "health_bar.h"

namespace shapes_riot {

HealthBar::HealthBar(double hp) :
    hp_{hp}
{}

gfx::DrawList HealthBar::draw() const
{
    return gfx::DrawList();
}

void HealthBar::tick(double /*delta*/, double hp)
{
    hp_ = hp;
}

}  // namespace shapes_riot
