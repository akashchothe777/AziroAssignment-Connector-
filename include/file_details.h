#ifndef FILE_DETAILS_H
#define FILE_DETAILS_H

#include <string>

struct FileDetails
{
    unsigned long id;
    std::string name;
    std::string last_modified_time;
    unsigned long long size;
};

#endif