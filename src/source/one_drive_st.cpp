#include "source/one_drive_st.h"
#include <iostream>
#include <fstream>
#include <string>
#include "nlohmann/json.hpp"
#include "metadata/file_metadata_utils.h"

using json = nlohmann::json;

size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream) 
{
    std::ofstream* out = static_cast<std::ofstream*>(stream);
    out->write(static_cast<char*>(ptr), size * nmemb);
    return size * nmemb;
}

bool OneDriveST::DownloadFile(FileMetadata file_metadata, std::string download_folder)
{
    std::cout << "Info: In OneDriveST::DownloadFile()" << std::endl;
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    std::string outputPath{download_folder};

    outputPath = outputPath + "\\" + file_metadata.unique_id;
    std::string url = file_metadata.source_path;

    std::ofstream file(outputPath, std::ios::binary);

    if (!file.is_open()) {
        curl_easy_cleanup(curl);
        return false;
    }

    struct curl_slist* headers = NULL;

    // Add Authorization header (Bearer token)
    std::string authHeader = "Authorization: Bearer " + access_token;
    headers = curl_slist_append(headers, authHeader.c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);

    // Optional but useful
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    CURLcode res = curl_easy_perform(curl);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    file.close();

    if(res == CURLE_OK)
        FileMetadataUtils::UpdateLocalFilePath(file_metadata.unique_id, outputPath);

    return (res == CURLE_OK);
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) 
{
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

bool OneDriveST::GetFileDetailsRecursively(std::vector<FileMetadata>& files_metadata, std::string folder_id)
{
    std::string url_address = "https://graph.microsoft.com/v1.0/me/drive/items/" + folder_id + "/children";

    CURLcode res{};
    std::string response{};
    CURL* curl = nullptr;

    curl = curl_easy_init();
    if (curl) 
    {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + access_token).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url_address.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);

        if (res == CURLE_OK) 
        {
            auto jsonData = json::parse(response);

            for (auto& item : jsonData["value"]) 
            {
                if (item.contains("folder"))
                {
                    std::vector<FileMetadata> sub_files_metadata;
                    GetFileDetailsRecursively(sub_files_metadata, item["id"]);

                    std::string name = item["name"];
                    // prepend folder name here in the file name
                    std::for_each(sub_files_metadata.begin(), sub_files_metadata.end(), [=](auto& itr){
                        itr.name = name + "/" + itr.name;
                    });
                    files_metadata.insert(files_metadata.end(), sub_files_metadata.begin(), sub_files_metadata.end());
                }
                else
                {
                    std::cout << "Id: " << item["id"] << std::endl;
                    std::cout << "WebUrl: " << item["webUrl"] << std::endl;
                    std::cout << "Name: " << item["name"] << std::endl;
                    std::cout << "Size: " << item["size"] << std::endl;
                    std::cout << "Modified: " << item["lastModifiedDateTime"] << std::endl;
                    std::cout << "--------------------------" << std::endl;
                    FileMetadata file_details;
                    file_details.unique_id = item["id"];
                    file_details.source_path = item["@microsoft.graph.downloadUrl"];
                    file_details.name = item["name"];
                    file_details.last_modified_time = item["lastModifiedDateTime"];
                    file_details.size = item["size"];

                    files_metadata.push_back(file_details);
                }
            }
        }
    } 
    else
    {
        std::cerr << "Error: Request failed: " << curl_easy_strerror(res) << std::endl;
        return false;
    }

    return true;
}

std::string ExtractFolderID(std::string url)
{
    size_t start = url.find("items/") + 6;   // move past "items/"
    size_t end   = url.find("/children");

    // Extract folder ID
    std::string folderId = url.substr(start, end - start);

    return folderId;
}

std::vector<FileMetadata> OneDriveST::GetFilesDetails()
{
    std::cout << "Info: In OneDriveST::GetFilesDetails()" << std::endl;
    std::vector<FileMetadata> files_metadata;

    GetFileDetailsRecursively(files_metadata, ExtractFolderID(address));

    return files_metadata;
}
