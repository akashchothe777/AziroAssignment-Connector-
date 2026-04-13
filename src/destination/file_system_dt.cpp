#include "destination/file_system_dt.h"
#include <filesystem>
#include <iostream>
#include "metadata\file_metadata_utils.h"

namespace fs = std::filesystem;


bool FileSystemDT::UploadFile(fs::path file_path)
{
    std::cout << "In FileSystemDT::UploadFile()" << std::endl;

    bool result = false;
    if (!fs::exists(address)) 
    {
        fs::create_directories(address);
    }

    fs::path destPath = fs::path(address) / file_path.filename();

    try
    {
        if(fs::copy_file(file_path, destPath, fs::copy_options::overwrite_existing))
        {
            result = true;
            FileMetadataUtils::UpdateFileMetadata(file_path, destPath);

            fs::remove(file_path);
            std::cout << "Copied file " << file_path << " to the destination folder "
                << address << std::endl;
        }
        else
        {
            std::cerr << "Copying file failed" << '\n';
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return result;
}