#ifndef DESTINATION_ADAPTER_H
#define DESTINATION_ADAPTER_H

#include "destination_type.h"
#include "metadata/file_metadata.h"
#include <vector>
#include <memory>
#include "metadata/json_handler.h"

class DestinationAdapter
{
private:
    std::shared_ptr<DestinationType> destination_type;
public:
    DestinationAdapter(std::shared_ptr<DestinationType> dt) : destination_type(dt){};
    bool UploadFiles(std::string download_folder, JsonHandler& json_handler);
    ~DestinationAdapter(){};
};

#endif // DESTINATION_ADAPTER_H