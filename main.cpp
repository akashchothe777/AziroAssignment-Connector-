#include <iostream>
#include "./include/engine/connector_engine.h"
#include "./include/source/file_system_st.h"
#include "./include/destination/file_system_dt.h"
#include "./include/metadata/json_handler.h"
#include "./include/metadata/file_metadata_utils.h"

int main(int argc, char* argv[]){
    std::cout << "Hello, from Connector!" << std::endl;

    std::cout << "Number of arguments : " << argc << std::endl;
    if(argc < 3)
        std::cout << "Error : Insufficient number of arguments!" << std::endl;
    std::string source_folder = argv[1];
    std::string destination_folder = argv[2];
    std::cout << "Source folder = " << source_folder << std::endl;
    std::cout << "Destination folder = " << destination_folder << std::endl;

    std::string json_file{"D:\\Assessment\\Connector1\\md\\metadata_store.json"};
    JsonHandler json_handler(json_file);

    if (!fs::exists(json_file)) 
    {
        std::ofstream outfile(json_file);
        if (!outfile) {
            std::cerr << "Error: Could not create file \"" << json_file << "\".\n";
            return 1;
        }
        outfile << "This is a newly created file.\n"; // Optional initial content
        outfile.close();
    }
    else
    {
        json_handler.loadFromFile(FileMetadataUtils::files_metadata);
    }

    std::shared_ptr<SourceType> source_type = std::make_shared<FileSystemST>(source_folder);
    std::shared_ptr<DestinationType> destination_type = std::make_shared<FileSystemDT>(destination_folder);

    SourceAdapter sa(source_type);
    DestinationAdapter da(destination_type);

    ConnectorEngine connector_engine(sa, da);
    connector_engine.run();

    json_handler.saveToFile(FileMetadataUtils::files_metadata);

    return 0;
}
