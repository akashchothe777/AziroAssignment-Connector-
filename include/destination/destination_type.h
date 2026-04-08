#ifndef DESTINATION_TYPE_H
#define DESTINATION_TYPE_H

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

class DestinationType
{
    std::string address;

    public:
    virtual bool UploadFile(fs::path file_path) = 0;
};

#endif // DESTINATION_TYPE_H