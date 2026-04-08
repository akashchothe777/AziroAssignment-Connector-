cmake_minimum_required(VERSION 3.10.0)
project(Connector VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(Connector
             main.cpp
             src/destination/destination_adapter.cpp
             src/destination/file_system_dt.cpp
             src/engine/connector_engine.cpp
             src/source/file_system_st.cpp
             src/source/source_adapter.cpp
             src/metadata/file_metadata_utils.cpp)

include(CTest)
enable_testing()

