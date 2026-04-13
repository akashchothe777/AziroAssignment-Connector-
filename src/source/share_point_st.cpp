#include "source/share_point_st.h"
#include <curl/curl.h>
#include <iostream>

bool SharePointST::DownloadFile(std::string file_name, std::string data_folder)
{
    std::cout << "LibCurl is initialising." << std::endl;

    CURL* curl = curl_easy_init();

    std::cout << "LibCurl is initialised." << std::endl;

    return true;
}

std::vector<FileDetails> SharePointST::GetFilesDetails()
{
    std::vector<FileDetails> xyz;

    return xyz;
}