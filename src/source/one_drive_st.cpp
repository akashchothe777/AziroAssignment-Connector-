#include "source/one_drive_st.h"
#include <iostream>
#include <fstream>
#include <string>
#include "nlohmann/json.hpp"
#include "metadata/file_metadata_utils.h"

using json = nlohmann::json;

long GetFileSize(const std::string& filename) 
{
    try 
    {
        if (fs::exists(filename)) 
        {
            return fs::file_size(filename);
        }
    } 
    catch (const fs::filesystem_error& e) {
        std::cerr << "Error: Filesystem error: " << e.what() << std::endl;
    }
    return 0;
}

bool EnsureDirectory(const std::string& filepath) 
{
    try 
    {
        fs::path p(filepath);
        if (p.has_parent_path()) 
        {
            fs::create_directories(p.parent_path());
        }
        return true;
    } 
    catch (const fs::filesystem_error& e) 
    {
        std::cerr << "Error: Directory error: " << e.what() << std::endl;
        return false;
    }
}

size_t write_callback(void* ptr, size_t size, size_t nmemb, void* stream) 
{
    FILE* file = static_cast<FILE*>(stream);
    return fwrite(ptr, size, nmemb, file);
}

bool OneDriveST::DownloadFile(FileMetadata file_metadata, std::string output_file)
{
    std::cout << "Info: In OneDriveST::DownloadFile()" << std::endl;
    std::string url = file_metadata.source_path;

    if (!EnsureDirectory(output_file)) 
    {
        return false;
    }

    long resume_from = GetFileSize(output_file);

    FILE* file = fopen(output_file.c_str(), "ab");
    if (!file) 
    {
        std::cerr << "Error: Failed to open file" << std::endl;
        return false;
    }

    CURL* curl = curl_easy_init();
    if (!curl) 
    {
        fclose(file);
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

    // Follow redirect (Graph API requirement)
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    // Resume support
    if (resume_from > 0) 
    {
        std::string range = std::to_string(resume_from) + "-";
        curl_easy_setopt(curl, CURLOPT_RANGE, range.c_str());
        std::cout << "Info: Resuming from byte: " << resume_from << std::endl;
    }

    // Timeouts and speed checks
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30L);
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 1024L);
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 30L);

    // SSL verification
    // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);

    CURLcode res = curl_easy_perform(curl);

    long response_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    curl_easy_cleanup(curl);
    fclose(file);

    if (res != CURLE_OK) 
    {
        std::cerr << "Error: CURL error: " << curl_easy_strerror(res) << std::endl;
        return false;
    }

    // Handle HTTP responses
    if (response_code == 200 || response_code == 206) 
    {
        return true;
    } 
    else if (response_code == 416) 
    {
        std::cerr << "Error: Invalid range (file may already be complete)" << std::endl;
        return true;
    } 
    else if (response_code == 429) 
    {
        std::cerr << "Error: Throttled (429)" << std::endl;
        return false;
    } 
    else 
    {
        std::cerr << "Error: HTTP error: " << response_code << std::endl;
        return false;
    }
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

        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

        if (res == CURLE_OK) 
        {
            if (http_code == 401 || http_code == 403) 
            {
                std::cerr << "Error: Authentication failed (HTTP " << http_code << ")\n";

                // Optional: log response for debugging
                std::cerr << "Response: " << response << std::endl;

                // TODO: refresh token here
                // if (RefreshAccessToken()) retry request

                curl_easy_cleanup(curl);
                curl_slist_free_all(headers);
                return false;
            }

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

            curl_slist_free_all(headers);
        }
    } 
    else
    {
        std::cerr << "Error: Request failed: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return false;
    }
    curl_easy_cleanup(curl);

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
