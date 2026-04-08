#include <iostream>
#include "./include/engine/connector_engine.h"
#include "./include/source/file_system_st.h"
#include "./include/destination/file_system_dt.h"

int main(int argc, char* argv[]){
    std::cout << "Hello, from Connector!" << std::endl;

    std::cout << "Number of arguments : " << argc << std::endl;
    if(argc < 3)
        std::cout << "Error : Insufficient number of arguments!" << std::endl;
    std::string source_folder = argv[1];
    std::string destination_folder = argv[2];
    std::cout << "Source folder = " << source_folder << std::endl;
    std::cout << "Destination folder = " << destination_folder << std::endl;

    std::shared_ptr<SourceType> source_type = std::make_shared<FileSystemST>(source_folder);
    std::shared_ptr<DestinationType> destination_type = std::make_shared<FileSystemDT>(destination_folder);

    SourceAdapter sa(source_type);
    DestinationAdapter da(destination_type);

    ConnectorEngine connector_engine(sa, da);
    connector_engine.run();

    return 0;
}
