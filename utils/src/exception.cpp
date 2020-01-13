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
            logger(utils::LogLevel::fatal, "no active exception");
    }
    catch (const utils::Exception& ex) {
        logger(utils::LogLevel::fatal, "fatal exception: {}\nfrom: {}", ex.what(), ex.from());
    }
    catch (const std::exception& ex) {
        logger(utils::LogLevel::fatal, "fatal exception: {}", ex.what());
    }
    catch (...) {
        logger(utils::LogLevel::fatal, "fatal exception of unknown type");
    }

    std::string trace = "stacktrace:";
    std::vector<StackFrame> frames = stacktrace();
    for (size_t i = 0; i < frames.size(); ++i) {
        const StackFrame& f = frames[i];
        trace += fmt::format("\n#{:02d}  {}  {}  {}  {}:{}", i, f.module, f.function, f.address, f.file, f.line);
    }
    logger(utils::LogLevel::fatal, trace);

    std::abort();
}

}  // namespace utils
