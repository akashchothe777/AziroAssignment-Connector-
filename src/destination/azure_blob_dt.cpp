#include "destination/azure_blob_dt.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include "metadata/file_metadata_utils.h"
#include <curl/curl.h>
#include <thread>
#include "utils/utils.h"

const size_t SMALL_FILE_LIMIT = 32 * 1024 * 1024; // 32MB
const size_t BLOCK_SIZE = 8 * 1024 * 1024;        // 8MB

namespace fs = std::filesystem;

std::string GetSasUrlForFileUpload(std::string sasUrl, std::string file_relative_path)
{
    size_t pos = sasUrl.find('?');
    std::string baseUrl = sasUrl.substr(0, pos);
    std::string sasToken = sasUrl.substr(pos);

    return (baseUrl + "/" + file_relative_path + sasToken);
}

bool PerformWithRetry(CURL *curl, unsigned int retry_count) 
{
    for (unsigned int i = 0; i < retry_count; i++) 
    {
        CURLcode res = curl_easy_perform(curl);
        
        long response_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

        if ((res == CURLE_OK) && (response_code == 201))
            return true;

        std::cerr << "\nError: Retry " << i + 1 << " failed: "
                  << curl_easy_strerror(res) << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1 << i));
    }
    return false;
}

bool AzureBlobDT::UploadSmallFile(CURL *curl, const std::string &url, const std::string &file_path) 
{
    std::cout << "Info: In AzureBlobDT::UploadSmallFile()" << std::endl;

    std::ifstream file(file_path, std::ios::binary);
    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string data = buffer.str();

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "x-ms-blob-type: BlockBlob");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());

    bool ok = PerformWithRetry(curl, retry_count);

    curl_slist_free_all(headers);
    return ok;
}

bool UploadBlock(CURL *curl,
                const std::string &url,
                const std::string &block_id,
                const char *data,
                size_t size,
                unsigned int retry_count) 
{
    std::string full_url = url + "&comp=block&blockid=" + block_id;

    curl_easy_setopt(curl, CURLOPT_URL, full_url.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, size);

    return PerformWithRetry(curl, retry_count);
}

bool CommitBlocks(CURL *curl,
                    const std::string &url,
                    const std::vector<std::string> &block_ids,
                    unsigned int retry_count) 
{
    // Build XML safely (NO BOM, NO STREAM INSERTION ISSUES)
    std::string xml;
    xml.reserve(1024 + block_ids.size() * 64);

    xml += "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    xml += "<BlockList>\n";

    for (const auto &id : block_ids) {
        xml += "  <Latest>";
        xml += id;
        xml += "</Latest>\n";
    }

    xml += "</BlockList>";

    // Ensure no hidden BOM
    if (xml.size() >= 3 &&
        (unsigned char)xml[0] == 0xEF &&
        (unsigned char)xml[1] == 0xBB &&
        (unsigned char)xml[2] == 0xBF) {
        xml.erase(0, 3);
    }

    std::string full_url = url + "&comp=blocklist";

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/xml");
    headers = curl_slist_append(headers, "x-ms-version: 2023-01-03");

    curl_easy_setopt(curl, CURLOPT_URL, full_url.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, xml.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, xml.size());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    return PerformWithRetry(curl, retry_count);
}

void save_checkpoint(const std::string &file, const std::vector<std::string> &blocks) 
{
    std::ofstream out(file);
    for (auto &b : blocks) out << b << std::endl;
}

std::vector<std::string> load_checkpoint(const std::string &file) 
{
    std::vector<std::string> blocks;
    std::ifstream in(file);
    std::string line;
    while (std::getline(in, line)) blocks.push_back(line);
    return blocks;
}

bool AzureBlobDT::UploadLargeFile(CURL *curl, const std::string &url, const std::string &file_path) 
{
    std::cout << "Info: In AzureBlobDT::UploadLargeFile()" << std::endl;
    bool returnValue{false};

    std::ifstream file(file_path, std::ios::binary);
    std::vector<char> buffer(BLOCK_SIZE);

    std::string ckpt_file = file_path + ".upload.ckpt";
    std::vector<std::string> block_ids = load_checkpoint(ckpt_file);

    size_t index = block_ids.size();

    file.seekg(index * BLOCK_SIZE);

    while (file) 
    {
        file.read(buffer.data(), BLOCK_SIZE);
        std::streamsize bytes = file.gcount();
        if (bytes <= 0) break;

        std::stringstream ss;
        ss << std::setw(6) << std::setfill('0') << index;
        std::string block_id = ConnectorUtils::Base64Encode(ss.str());

        if (!UploadBlock(curl, url, block_id, buffer.data(), bytes, retry_count)) 
        {
            std::cerr << "Error: Block upload failed" << std::endl;
            return false;
        }

        block_ids.push_back(block_id);
        save_checkpoint(ckpt_file, block_ids);

        std::cout << "Info: Uploaded block " << index << std::endl;
        index++;
    }

    if(CommitBlocks(curl, url, block_ids, retry_count))
    {
        fs::remove(ckpt_file);
        returnValue = true;
    }

    return returnValue;
}

int progress_callback(void*, curl_off_t total, curl_off_t now, curl_off_t, curl_off_t) 
{
    if (total > 0) 
    {
        int percent = (int)((now * 100) / total);
        std::cout << "\rInfo: Progress: " << percent << "%" << std::flush;
    }
    return 0;
}

bool AzureBlobDT::UploadFile(FileMetadata file_metadata)
{
    std::cout << "Info: In AzureBlobDT::UploadFile()" << std::endl;

    std::string sas_url_file = GetSasUrlForFileUpload(address, file_metadata.name);

    std::ifstream f(file_metadata.local_path, std::ios::binary | std::ios::ate);
    size_t size = f.tellg();

    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progress_callback);

    bool success = false;

    if (size <= SMALL_FILE_LIMIT) 
    {
        std::cout << "Into: Uploading small file..." << std::endl;
        success = UploadSmallFile(curl, sas_url_file, file_metadata.local_path);
    } 
    else 
    {
        std::cout << "Into: Uploading large file in blocks..." << std::endl;
        success = UploadLargeFile(curl, sas_url_file, file_metadata.local_path);
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return success;
}
