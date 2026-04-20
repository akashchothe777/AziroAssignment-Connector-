#include "utils/utils.h"

static const char b64_table[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

namespace ConnectorUtils
{
    std::string Base64Encode(const std::string &input) 
    {
        std::string output;
        int val = 0, valb = -6;

        for (unsigned char c : input) 
        {
            val = (val << 8) + c;
            valb += 8;
            while (valb >= 0) 
            {
                output.push_back(b64_table[(val >> valb) & 0x3F]);
                valb -= 6;
            }
        }

        if (valb > -6)
            output.push_back(b64_table[((val << 8) >> (valb + 8)) & 0x3F]);

        while (output.size() % 4)
            output.push_back('=');

        return output;
    }

    bool has_ckpt_extension(const fs::path& p)
    {
        const std::string suffix = ".upload.ckpt";

        std::string name = p.filename().string();

        if (name.size() < suffix.size())
            return false;

        return name.compare(name.size() - suffix.size(), suffix.size(), suffix) == 0;
    }
}   // namespace ConnectorUtils
