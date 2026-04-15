#ifndef DESTINATION_ADAPTER_H
#define DESTINATION_ADAPTER_H

#include "destination_type.h"
#include "metadata/file_metadata.h"
#include <vector>
#include <memory>

class DestinationAdapter
{
private:
    std::shared_ptr<DestinationType> destination_type;
    unsigned int retry_count;
public:
    DestinationAdapter(std::shared_ptr<DestinationType> dt, unsigned int rc) : destination_type(dt), retry_count(rc){};
    bool UploadFiles(std::string download_folder);
    ~DestinationAdapter(){};
};

#endif // DESTINATION_ADAPTER_H