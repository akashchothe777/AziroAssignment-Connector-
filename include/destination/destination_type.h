#ifndef DESTINATION_TYPE_H
#define DESTINATION_TYPE_H

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

class DestinationType
{
    protected:
    std::string address;

    public:
    DestinationType(std::string ads) : address(ads){}
    virtual bool UploadFile(fs::path downloaded_file) = 0;
    virtual ~DestinationType(){};
};

#endif // DESTINATION_TYPE_H