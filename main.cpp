#include <iostream>
#include "engine/connector_engine.h"
#include "source/file_system_st.h"
#include "source/one_drive_st.h"
#include "destination/file_system_dt.h"
#include "destination/azure_blob_dt.h"
#include "metadata/json_handler.h"
#include "metadata/file_metadata_utils.h"
#include "config/connector_config.h"

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        std::cerr << "Error: Insufficient number of arguments!" << std::endl;
        return 1;
    }

    std::string config_file = argv[1];
    std::cout << "Info: Configuration file = " << config_file << std::endl;
    
    ConnectorConfig conn_config(config_file);

    std::string metadata_file = conn_config.get<std::string>("metadata_file", "D:\\Assessment\\Connector1\\md\\metadata_store.json");
    std::cout << "Info: File to store metadata = " << metadata_file << std::endl;
    std::string download_folder = conn_config.get<std::string>("download_folder", "D:\\Assessment\\Connector1\\DownloadFolder");
    std::cout << "Info: Folder to download files temporary = " << download_folder << std::endl;
    unsigned int retry_count = conn_config.get<int>("retry_count", 3);
    std::cout << "Info: Retry count = " << retry_count << std::endl;

    std::string source_url = conn_config.get<std::string>("source_url", "");
    std::cout << "Info: Source URL = " << source_url << std::endl;

    std::string source_access_token = conn_config.get<std::string>("source_access_token", "");
    //std::cout << "Info: Source access token = " << source_access_token << std::endl;

    std::string destination_sas_url = conn_config.get<std::string>("destination_sas_url", "");
    std::cout << "Info: Destination Sas URL = " << destination_sas_url << std::endl;

    JsonHandler json_handler(metadata_file);

    if (!fs::exists(metadata_file)) 
    {
        std::ofstream outfile(metadata_file);
        if (!outfile)
        {
            std::cerr << "Error: Could not create file \"" << metadata_file << "\"." << std::endl;
            return 1;
        }
        outfile.close();
    }
    else
    {
        try
        {
            json_handler.LoadFromFile(FileMetadataUtils::file_id_to_metadata);
        }
        catch(const std::exception& e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
    }

    // std::shared_ptr<SourceType> source_type = std::make_shared<FileSystemST>(source_url);
    std::shared_ptr<SourceType> source_type = std::make_shared<OneDriveST>(source_url, source_access_token);
    
    //std::shared_ptr<DestinationType> destination_type = std::make_shared<FileSystemDT>(destination_sas_url);
    std::shared_ptr<DestinationType> destination_type = std::make_shared<AzureBlobDT>(destination_sas_url, retry_count);

    SourceAdapter sa(source_type, retry_count);
    DestinationAdapter da(destination_type);

    ConnectorEngine connector_engine(sa, da, download_folder);

    try
    {
       connector_engine.run(json_handler);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    json_handler.SaveToFile(FileMetadataUtils::file_id_to_metadata);

    return 0;
}
