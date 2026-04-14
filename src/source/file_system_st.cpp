#include "source/file_system_st.h"
#include <iostream>
#include <filesystem>
#include "metadata/file_metadata_utils.h"

namespace fs = std::filesystem;

bool FileSystemST::DownloadFile(std::string file_name, std::string download_folder)
{
    std::cout << "In FileSystemST::DownloadFile()" << std::endl;
    bool returnValue{false};

    std::string source_file = address + "//" + file_name;
    std::string dest_file = download_folder + "//" + file_name;

    if(std::filesystem::copy_file(
            source_file,
            dest_file,
            std::filesystem::copy_options::overwrite_existing
        ))
    {
        std::cout << "File copied from Source = " << source_file
                    << " to Destination = " << dest_file << std::endl;
        returnValue = true;
    }
    else
    {
        std::cout << "Copying files failed" << std::endl;
    }

    return returnValue;
}

std::vector<FileDetails> FileSystemST::GetFilesDetails()
{
    std::cout << "In FileSystemST::GetFilesDetails()" << std::endl;
    std::vector<FileDetails> files;
    for (const auto& entry : fs::directory_iterator(address))
    {
        FileDetails file;
        file.file_path = entry.path().string();
        file.name = entry.path().filename().string();
        file.size = fs::file_size(entry.path());
        file.last_modified_time = FileMetadataUtils::GetLastModifiedDateTime(entry.path());

        files.push_back(file);
    }

    return files;
}