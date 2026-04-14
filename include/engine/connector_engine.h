#ifndef CONNECTOR_ENGINE_H
#define CONNECTOR_ENGINE_H

#include "../source/source_adapter.h"
#include "../destination/destination_adapter.h"
#include "../metadata/file_metadata.h"

class ConnectorEngine
{
    SourceAdapter source_adapter;
    DestinationAdapter destination_adapter;
    std::string download_folder;
public:
    void run();
    ConnectorEngine(SourceAdapter sa, DestinationAdapter da, std::string df) :
                        source_adapter(sa), destination_adapter(da), download_folder(df)
    {};
    ~ConnectorEngine(){};
};

#endif // CONNECTOR_ENGINE_H