#include "utils/any.h"

namespace utils {

Any::operator bool() const
{
    return impl_.operator bool();
}


UniqueAny::operator bool() const
{
    return impl_.operator bool();
}

}  // namespace utils
