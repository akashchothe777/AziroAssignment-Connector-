#include "destination/azure_blob_dt.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include "metadata/file_metadata_utils.h"
#include <curl/curl.h>

namespace fs = std::filesystem;

std::string GetSasUrlForFileUpload(std::string sasUrl, std::string file_relative_path)
{
    size_t pos = sasUrl.find('?');
    std::string baseUrl = sasUrl.substr(0, pos);
    std::string sasToken = sasUrl.substr(pos);

    return (baseUrl + "/" + file_relative_path + sasToken);
}

bool AzureBlobDT::UploadFile(fs::path downloaded_file)
{
    std::cout << "Info: In AzureBlobDT::UploadFile()" << std::endl;

    FileMetadata file_metadata = FileMetadataUtils::GetFileMetadataOfLocalFile(downloaded_file.filename().string());

    std::string sas_url_file = GetSasUrlForFileUpload(address, file_metadata.name);

    // Open file
    std::ifstream file(downloaded_file, std::ios::binary);
    if (!file)
    {
        std::cerr << "Error: Cannot open file: " << downloaded_file << std::endl;
        return false;
    }

    // Read file into buffer
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)),
                              std::istreambuf_iterator<char>());

    CURL* curl = curl_easy_init();
    if (!curl)
    {
        std::cerr << "Error: curl init failed." << std::endl;
        return false;
    }

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "x-ms-blob-type: BlockBlob");
    headers = curl_slist_append(headers, "Content-Type: application/octet-stream");

    curl_easy_setopt(curl, CURLOPT_URL, sas_url_file.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buffer.data());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, buffer.size());

    // Optional: timeout
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 300L);

    CURLcode res = curl_easy_perform(curl);

    long response_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    if (res != CURLE_OK || (response_code != 201 && response_code != 200))
    {
        std::cerr << "Error: Upload failed!" << std::endl;
        std::cerr << "Error: CURL error: " << curl_easy_strerror(res) << std::endl;
        std::cerr << "Error: HTTP response: " << response_code << std::endl;
        
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        return false;
    }
    else
    {
        FileMetadataUtils::UpdateFileMetadataForUpload(file_metadata.unique_id, file_metadata.name);
    }

    std::cout << "Upload successful!" << std::endl;

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    
    return true;
}