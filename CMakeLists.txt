cmake_minimum_required(VERSION 3.16)

project(AmazonGame VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt 6
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Create the executable from main.cpp
add_executable(AmazonGame main.cpp)

# Link the Widgets library
target_link_libraries(AmazonGame PRIVATE Qt6::Widgets)

# Required for Windows to hide the console window
set_target_properties(AmazonGame PROPERTIES WIN32_EXECUTABLE TRUE)
