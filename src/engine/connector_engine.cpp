#include "engine/connector_engine.h"
#include "metadata/file_metadata_utils.h"
#include <iostream>

void ConnectorEngine::run()
{
    std::cout << "In ConnectorEngine::run()" << std::endl;

    auto available_files = source_adapter.GetFileList();

    auto files_to_download = FileMetadataUtils::GetListOfFilesToDownload(available_files);

    source_adapter.DownloadFiles(files_to_download, download_folder);

    destination_adapter.UploadFiles(download_folder);
}