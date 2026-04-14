#ifndef UTILS_IMPL_H
#define UTILS_IMPL_H

#include <thread>

namespace ConnectorUtils
{
    template<typename Func>
    bool RetryFunction(Func func, int maxRetries, int baseDelayMs) 
    {
        for (int attempt = 1; attempt <= maxRetries; ++attempt) 
        {
            try 
            {
                if (func()) 
                    return true;
            } 
            catch (const std::exception& e) 
            {
                std::cout << "Attempt " << attempt << " failed: " << e.what() << "\n";
            }

            if (attempt < maxRetries) {
                int delay = baseDelayMs * (1 << (attempt - 1)); // 500, 1000, 2000...
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            }
        }
        return false;
    }
} // namespace ConnectorUtils

#endif // UTILS_IMPL_H
