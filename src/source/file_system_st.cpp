#include "source/file_system_st.h"
#include <iostream>
#include <filesystem>
#include "metadata/file_metadata_utils.h"
#include "utils/utils.h"

namespace fs = std::filesystem;

bool FileSystemST::DownloadFile(FileMetadata file, std::string download_folder)
{
    std::cout << "In FileSystemST::DownloadFile()" << std::endl;
    bool returnValue{false};

    std::string source_file = file.source_path;
    std::string dest_file = download_folder + "\\" + file.unique_id;

    if(std::filesystem::copy_file(
            source_file,
            dest_file,
            std::filesystem::copy_options::overwrite_existing
        ))
    {
        std::cout << "File copied from Source = " << source_file
                    << " to Destination = " << dest_file << std::endl;
        FileMetadataUtils::UpdateLocalFilePath(file.unique_id, dest_file);
        
        returnValue = true;
    }
    else
    {
        std::cout << "Copying files failed" << std::endl;
    }

    return returnValue;
}

std::vector<FileMetadata> FileSystemST::GetFilesDetails()
{
    std::cout << "In FileSystemST::GetFilesDetails()" << std::endl;
    std::vector<FileMetadata> files;
    for (const auto& entry : fs::recursive_directory_iterator(address)) 
    {
        if(entry.is_directory())
            continue;

        FileMetadata file;

        std::string uid = ConnectorUtils::GenerateFileUniqueId(entry.path().string());
        file.unique_id = uid;
        file.source_path = entry.path().string();
        fs::path relativePath = fs::relative(entry.path().string(), address);
        file.name = relativePath.string();
        file.size = fs::file_size(entry.path());
        file.last_modified_time = FileMetadataUtils::GetLastModifiedDateTime(entry.path());

        files.push_back(file);
    }

    return files;
}
