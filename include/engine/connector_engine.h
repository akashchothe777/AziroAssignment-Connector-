#ifndef CONNECTOR_ENGINE_H
#define CONNECTOR_ENGINE_H

#include "../source/source_adapter.h"
#include "../destination/destination_adapter.h"
#include "../metadata/file_metadata.h"

class ConnectorEngine
{
    std::string data_folder{"D:\\Assessment\\Connector1\\DataFolder"};
    SourceAdapter source_adapter;
    DestinationAdapter destination_adapter;
    FileMetadata file_metadata{};
public:
    void run();
    ConnectorEngine(SourceAdapter sa, DestinationAdapter da) :
                        source_adapter(sa), destination_adapter(da)
    {};
    ~ConnectorEngine(){};
};

#endif // CONNECTOR_ENGINE_H