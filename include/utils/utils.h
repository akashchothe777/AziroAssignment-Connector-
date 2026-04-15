#ifndef UTILS_H
#define UTILS_H

#include <functional>
#include <string>

namespace ConnectorUtils
{
    template<typename Func>
    bool RetryFunction(Func func, int maxRetries, int baseDelayMs = 500);

    std::string GenerateFileUniqueId(const std::string &filePath);
} // namespace ConnectorUtils

#include "utils/utils_impl.h"

#endif // UTILS_H
