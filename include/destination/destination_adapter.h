#ifndef DESTINATION_ADAPTER_H
#define DESTINATION_ADAPTER_H

#include "destination_type.h"
#include "../file_details.h"
#include <vector>
#include <memory>

class DestinationAdapter
{
private:
    std::shared_ptr<DestinationType> destination_type;
public:
    bool UploadFiles(std::string data_folder);
    DestinationAdapter(std::shared_ptr<DestinationType> dt) : destination_type(dt){};
    ~DestinationAdapter(){};
};

#endif // DESTINATION_ADAPTER_H