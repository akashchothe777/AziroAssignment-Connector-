#ifndef DESTINATION_TYPE_H
#define DESTINATION_TYPE_H

#include <string>
#include <filesystem>
#include "metadata/file_metadata.h"

namespace fs = std::filesystem;

class DestinationType
{
    protected:
    std::string address;
    unsigned int retry_count;

    public:
    DestinationType(std::string ads, unsigned int rc) : address(ads), retry_count(rc){}
    virtual bool UploadFile(FileMetadata downloaded_file) = 0;
    virtual ~DestinationType(){};
};

#endif // DESTINATION_TYPE_H