#include <gtest/gtest.h>
#include "file_details.h"
#include "metadata/file_metadata_utils.h"

TEST(TestFileMetadataUtils, TestIsModifiedTrue) 
{
    // Arrange
    FileDetails file = {"D:/xyz.txt", "xyz.txt", "2026-04-13T11:20:38", 100};
    FileMetadataUtils::filepath_to_metadata["D:/xyz.txt"] = FileMetadata("D:/xyz.txt",
    "xyz.txt", "2026-04-13T11:20:38", 100, "2026-04-13T11:20:38", "D:/dest/xyz.txt");

    // Act
    bool is_modified = FileMetadataUtils::IsModified(file);

    // Assert
    EXPECT_FALSE(is_modified);
}

TEST(TestFileMetadataUtils, TestIsModifiedFalse) 
{
    // Arrange
    FileDetails file = {"D:/xyz.txt", "xyz.txt", "2026-04-13T11:20:38", 100};
    FileMetadataUtils::filepath_to_metadata["D:/xyz.txt"] = FileMetadata("D:/xyz.txt",
    "xyz.txt", "2026-04-13T11:20:39", 100, "2026-04-13T11:20:38", "D:/dest/xyz.txt");

    // Act
    bool is_modified = FileMetadataUtils::IsModified(file);

    // Assert
    EXPECT_TRUE(is_modified);
}

TEST(TestFileMetadataUtils, TestIsNewFalse) 
{
    // Arrange
    FileDetails file = {"D:/xyz.txt", "xyz.txt", "2026-04-13T11:20:38", 100};
    FileMetadataUtils::filepath_to_metadata["D:/xyz.txt"] = FileMetadata("D:/xyz.txt",
    "xyz.txt", "2026-04-13T11:20:39", 100, "2026-04-13T11:20:38", "D:/dest/xyz.txt");

    // Act
    bool is_new = FileMetadataUtils::IsNew(file);

    // Assert
    EXPECT_FALSE(is_new);
}

TEST(TestFileMetadataUtils, TestIsNewTrue) 
{
    // Arrange
    FileDetails file = {"D:/pqr.txt", "pqr.txt", "2026-04-13T11:20:38", 100};
    FileMetadataUtils::filepath_to_metadata["D:/xyz.txt"] = FileMetadata("D:/xyz.txt",
    "xyz.txt", "2026-04-13T11:20:39", 100, "2026-04-13T11:20:38", "D:/dest/xyz.txt");

    // Act
    bool is_new = FileMetadataUtils::IsNew(file);

    // Assert
    EXPECT_TRUE(is_new);
}

TEST(TestFileMetadataUtils, TestGetLastModifiedDateTime) 
{
    // Arrange
    std::filesystem::path file_path{"D:/Assessment/Connector1/tests/input_file/xyz.txt"};
    std::string expected_time{"2026-04-13T11:58:05"};

    // Act
    std::string last_modified_time = FileMetadataUtils::GetLastModifiedDateTime(file_path);

    // Assert
    EXPECT_STREQ(last_modified_time.c_str(), expected_time.c_str());
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
