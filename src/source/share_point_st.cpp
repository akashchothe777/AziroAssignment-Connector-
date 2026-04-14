#include "source/share_point_st.h"
#include <curl/curl.h>
#include <iostream>
#include <string>
#include "../../third_party/nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

bool SharePointST::DownloadFile(std::string file_name, std::string download_folder)
{
    std::cout << "In SharePointST::DownloadFile()" << std::endl;
    CURL* curl = curl_easy_init();

    return true;
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) 
{
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

std::vector<FileDetails> SharePointST::GetFilesDetails()
{
    std::cout << "In SharePointST::GetFilesDetails()" << std::endl;
    std::vector<FileDetails> returnData;

    string access_token = "ACCESS_TOKEN";
    string url = "https://graph.microsoft.com/v1.0/sites/{site-id}/drive/root/children";

    CURL* curl;
    CURLcode res;
    string response;

    curl = curl_easy_init();
    if (curl) 
    {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + access_token).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);

        if (res == CURLE_OK) 
        {
            auto jsonData = json::parse(response);

            for (auto& item : jsonData["value"]) 
            {
                cout << "Name: " << item["name"] << endl;
                cout << "Size: " << item["size"] << endl;
                cout << "Created: " << item["createdDateTime"] << endl;
                cout << "Modified: " << item["lastModifiedDateTime"] << endl;
                cout << "--------------------------" << endl;
                FileDetails file_details;
                file_details.file_path = item["name"];
                file_details.name = item["name"];
                file_details.last_modified_time = item["lastModifiedDateTime"];
                file_details.size = item["size"];
                returnData.push_back(file_details);

            }
        } 
        else
        {
            cerr << "Request failed: " << curl_easy_strerror(res) << endl;
        }

        curl_easy_cleanup(curl);
    }

    return returnData;
}