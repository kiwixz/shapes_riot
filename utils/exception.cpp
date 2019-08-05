#include "utils/exception.h"

namespace utils {

Exception::Exception(std::string_view what) :
    std::runtime_error{what.data()}
{}

Exception::Exception(std::string from, std::string_view what) :
    std::runtime_error{what.data()},
    from_{std::move(from)}
{}

const std::string& Exception::from() const
{
    return from_;
}

}  // namespace utils
