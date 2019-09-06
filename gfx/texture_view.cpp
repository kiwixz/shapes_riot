#include "gfx/texture_view.h"

namespace gfx {

TextureView::TextureView(GLuint id) :
    id_{id}
{}

TextureView::operator bool() const
{
    return id_ != 0;
}

utils::ScopeExit TextureView::bind() const
{
    glBindTexture(GL_TEXTURE_2D, id_);
    return utils::ScopeExit{std::bind(glBindTexture, GL_TEXTURE_2D, 0)};
}


bool operator==(TextureView lhs, TextureView rhs)
{
    return lhs.id_ == rhs.id_;
}

bool operator!=(TextureView lhs, TextureView rhs)
{
    return !(lhs == rhs);
}

}  // namespace gfx
