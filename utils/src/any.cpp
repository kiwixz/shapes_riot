#include "utils/any.h"

namespace utils {

Any::~Any()
{
    destroy();
}

Any::Any(const Any& other)
{
    *this = other;
}

Any& Any::operator=(const Any& other)
{
    if (this != &other) {
        destroy();
        if (other) {
            proxy_ = other.proxy_;
            proxy_(any_detail::ProxyOp::copy, const_cast<any_detail::Storage*>(&other.storage_), &storage_);  // will not modify other.storage_
        }
    }
    return *this;
}

Any::Any(Any&& other) noexcept
{
    *this = std::move(other);
}

Any& Any::operator=(Any&& other) noexcept
{
    if (this != &other) {
        destroy();
        if (other) {
            proxy_ = other.proxy_;
            proxy_(any_detail::ProxyOp::move, &other.storage_, &storage_);
        }
    }
    return *this;
}

Any::operator bool() const
{
    return storage_.operator bool();
}

void Any::destroy()
{
    if (storage_)
        proxy_(any_detail::ProxyOp::destroy, &storage_, nullptr);
}

}  // namespace utils
