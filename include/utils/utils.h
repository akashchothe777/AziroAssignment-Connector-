#ifndef UTILS_H
#define UTILS_H

#include <functional>
#include <string>

namespace ConnectorUtils
{
    template<typename Func>
    bool RetryFunction(Func func, int maxRetries, int baseDelayMs = 500);
} // namespace ConnectorUtils

#include "utils/utils_impl.h"

#endif // UTILS_H
