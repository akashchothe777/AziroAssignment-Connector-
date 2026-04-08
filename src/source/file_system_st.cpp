#include "../../include/source/file_system_st.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

bool FileSystemST::DownloadFile(std::string file_name, std::string data_folder)
{
    std::cout << "In FileSystemST::DownloadFile()" << std::endl;

    std::string source_file = address + "\\" + file_name;
    std::string dest_file = data_folder + "\\" + file_name;

    std::cout << "Source = " << source_file
                << "\tDestination = " << dest_file << std::endl;

    std::filesystem::copy_file(
            source_file,
            dest_file,
            std::filesystem::copy_options::overwrite_existing
        );

    return false;
}

std::vector<FileDetails> FileSystemST::GetFilesDetails()
{
    std::vector<FileDetails> files;
    for (const auto& entry : fs::directory_iterator("D://Assessment//Connector1//SourceFolder")) 
    {
        FileDetails file;
        static unsigned long file_id = 1;
        file.id = file_id++;
        file.name = entry.path().filename().string();
        file.size = fs::file_size(entry.path());

        files.push_back(file);
    }

    return files;
}