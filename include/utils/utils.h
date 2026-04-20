#ifndef UTILS_H
#define UTILS_H

#include <functional>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace ConnectorUtils
{
    template<typename Func>
    bool RetryFunction(Func func, int maxRetries, int baseDelayMs = 500);

    std::string Base64Encode(const std::string &input);

    bool has_ckpt_extension(const fs::path& p);
} // namespace ConnectorUtils

#include "utils/utils_impl.h"

#endif // UTILS_H
