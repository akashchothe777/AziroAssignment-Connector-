#include "engine/connector_engine.h"
#include "metadata/file_metadata_utils.h"
#include <iostream>

void ConnectorEngine::run(JsonHandler& json_handler)
{
    std::cout << "Info: In ConnectorEngine::run()" << std::endl;

    // Get the details of files available at the source path
    auto available_files = source_adapter.GetFileList();

    // Get list of files to be downloaded if it is modified or a new file
    auto files_to_download = FileMetadataUtils::GetListOfFilesToDownload(available_files);
    json_handler.SaveToFile(FileMetadataUtils::file_id_to_metadata);

    // Download the files from source to the local download folder
    source_adapter.DownloadFiles(files_to_download, download_folder);
    json_handler.SaveToFile(FileMetadataUtils::file_id_to_metadata);

    // Upload the downloaded files from local download folder to destination
    destination_adapter.UploadFiles(download_folder, json_handler);
}