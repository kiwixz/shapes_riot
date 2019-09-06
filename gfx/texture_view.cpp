#include "gfx/texture_view.h"

namespace gfx {

TextureView::TextureView(GLuint id) :
    id_{id}
{}

utils::ScopeExit TextureView::bind() const
{
    glBindTexture(GL_TEXTURE_2D, id_);
    return utils::ScopeExit{std::bind(glBindTexture, GL_TEXTURE_2D, 0)};
}

}  // namespace gfx
