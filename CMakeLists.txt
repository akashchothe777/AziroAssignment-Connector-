cmake_minimum_required(VERSION 3.10.0)
project(Connector VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    "D:\\Assessment\\Connector1\\include")
# "C:\\curl\\include")

add_executable(Connector
             main.cpp
             src/destination/destination_adapter.cpp
             src/destination/file_system_dt.cpp
             src/engine/connector_engine.cpp
             src/source/file_system_st.cpp
             src/source/share_point_st.cpp
             src/source/source_adapter.cpp
             src/metadata/file_metadata_utils.cpp)

# target_link_directories(Connector PRIVATE "C:/curl/lib")

# target_link_libraries(Connector PRIVATE libcurl)

add_executable(unit_tests test.cpp)

target_link_libraries(unit_tests gtest gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:DebugDLL>")
add_subdirectory(third_party/gtest/googletest)
enable_testing()

