#include <iostream>
#include "./include/engine/connector_engine.h"
#include "./include/source/file_system_st.h"
#include "./include/source/share_point_st.h"
#include "./include/destination/file_system_dt.h"
#include "./include/metadata/json_handler.h"
#include "./include/metadata/file_metadata_utils.h"
#include "./include/config/connector_config.h"

int main(int argc, char* argv[])
{
    std::cout << "Hello, from Connector!" << std::endl;

    std::cout << "Number of arguments : " << argc << std::endl;
    if(argc < 2)
    {
        std::cout << "Error : Insufficient number of arguments!" << std::endl;
        return 1;
    }

    std::string config_file = argv[1];
    std::cout << "Configuration file = " << config_file << std::endl;
    
    ConnectorConfig conn_config(config_file);

    std::string metadata_file = conn_config.get<std::string>("metadata_file", "D:\\Assessment\\Connector1\\md\\metadata_store.json");
    std::string source_folder = conn_config.get<std::string>("source", "D:\\Assessment\\Connector1\\SourceFolder");
    std::string destination_folder = conn_config.get<std::string>("destination", "D:\\Assessment\\Connector1\\DestinationFolder");
    std::string download_folder = conn_config.get<std::string>("download_folder", "D:\\Assessment\\Connector1\\DataFolder");
    unsigned int retry_count = conn_config.get<int>("retry_count", 3);

    JsonHandler json_handler(metadata_file);

    if (!fs::exists(metadata_file)) 
    {
        std::ofstream outfile(metadata_file);
        if (!outfile)
        {
            std::cerr << "Error: Could not create file \"" << metadata_file << "\".\n";
            return 1;
        }
        outfile.close();
    }
    else
    {
        try
        {
            json_handler.loadFromFile(FileMetadataUtils::file_id_to_metadata);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return 1;
        }
    }

    std::shared_ptr<SourceType> source_type = std::make_shared<FileSystemST>(source_folder);
    //std::shared_ptr<SourceType> source_type = std::make_shared<SharePointST>();
    
    std::shared_ptr<DestinationType> destination_type = std::make_shared<FileSystemDT>(destination_folder);

    SourceAdapter sa(source_type, retry_count);
    DestinationAdapter da(destination_type, retry_count);

    ConnectorEngine connector_engine(sa, da, download_folder);
    connector_engine.run();

    json_handler.saveToFile(FileMetadataUtils::file_id_to_metadata);

    return 0;
}
