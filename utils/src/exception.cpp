#include "utils/exception.h"

#include <cstdlib>
#include <exception>

#include "utils/logger.h"
#include "utils/os.h"

namespace utils {

Exception::Exception(std::string&& from, std::string_view what) :
    std::runtime_error{what.data()},
    from_{std::make_shared<const std::string>(std::move(from))}
{}

const std::string& Exception::from() const
{
    return *from_;
}


void terminate() noexcept
{
    utils::Logger logger{"terminate"};
    try {
        std::exception_ptr ex = std::current_exception();
        if (ex)
            std::rethrow_exception(ex);
        else
            logger(utils::LogLevel::critical, "no active exception");
    }
    catch (const utils::Exception& ex) {
        logger(utils::LogLevel::critical, "fatal exception: {}\nfrom: {}", ex.what(), ex.from());
    }
    catch (const std::exception& ex) {
        logger(utils::LogLevel::critical, "fatal exception: {}", ex.what());
    }
    catch (...) {
        logger(utils::LogLevel::critical, "fatal exception of unknown type");
    }

    std::string trace = "stacktrace:";
    std::vector<std::string> frames = stacktrace();
    for (size_t i = 0; i < frames.size(); ++i)
        trace += fmt::format("\n#{:02d}  {}", i, frames[i]);
    logger(utils::LogLevel::critical, trace);

    std::abort();
}

}  // namespace utils
