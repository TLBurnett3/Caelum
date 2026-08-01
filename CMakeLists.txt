cmake_minimum_required(VERSION 3.16)
project(CaelumProject VERSION 1.0 LANGUAGES CXX)

# Global C++ standard settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure global 3rd-party dependency search paths
list(APPEND CMAKE_PREFIX_PATH "D:/3rdPartyLibs/")

# Find global dependencies required across projects
set(OpenCV_DIR "D:/3rdPartyLibs/lib")
find_package(OpenCV REQUIRED)

# Add subdirectories (Order matters: Library first, App second)
add_subdirectory(src/CaelumLib)
add_subdirectory(src/Caelum)