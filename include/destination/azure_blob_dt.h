#ifndef AZURE_BLOB_DT_H
#define AZURE_BLOB_DT_H

#include "destination_type.h"
#include <curl/curl.h>

class AzureBlobDT : public DestinationType
{
public:
    AzureBlobDT(std::string sas_url, unsigned int rc) : DestinationType(sas_url, rc){}
    bool UploadFile(FileMetadata downloaded_file);
    bool UploadLargeFile(CURL *curl, const std::string &url, const std::string &file_path);
    bool UploadSmallFile(CURL *curl, const std::string &url, const std::string &file_path);
    ~AzureBlobDT(){};
};

#endif // AZURE_BLOB_DT_H