#ifndef AZURE_BLOB_DT_H
#define AZURE_BLOB_DT_H

#include "destination_type.h"

class AzureBlobDT : public DestinationType
{
public:
    AzureBlobDT(std::string sas_url) : DestinationType(sas_url){}
    bool UploadFile(fs::path downloaded_file);
    ~AzureBlobDT(){};
};

#endif // AZURE_BLOB_DT_H