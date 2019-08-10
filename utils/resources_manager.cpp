#include "utils/resources_manager.h"

namespace utils {

void ResourcesManager::cleanup()
{
    for (auto it = store_.begin(); it != store_.end();)
        if (it->second.expired())
            it = store_.erase(it);
        else
            ++it;
}

}  // namespace utils
