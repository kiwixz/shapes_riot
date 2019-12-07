#include "utils/exception.h"
#include "utils/logger.h"
#include <cstdlib>
#include <exception>

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


void terminate()
{
    utils::Logger logger{"terminate"};
    try {
        std::exception_ptr ex = std::current_exception();
        if (ex)
            std::rethrow_exception(ex);
        else
            logger(utils::LogLevel::critical, "terminating without an active exception");
    }
    catch (const utils::Exception& ex) {
        logger(utils::LogLevel::critical, "fatal exception: {}\nfrom: {}", ex.what(), ex.from());
    }
    catch (const std::exception& ex) {
        logger(utils::LogLevel::critical, "fatal exception: {}", ex.what());
    }

    std::abort();
}

}  // namespace utils
