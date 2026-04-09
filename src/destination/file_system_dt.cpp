#include "destination/file_system_dt.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;


bool FileSystemDT::UploadFile(fs::path file_path)
{
    if (!fs::exists(address)) 
    {
        fs::create_directories(address);
    }

    fs::path destPath = fs::path(address) / file_path.filename();

    try
    {
        fs::copy_file(file_path, destPath, fs::copy_options::overwrite_existing);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return false;
}