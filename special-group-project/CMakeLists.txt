cmake_minimum_required(VERSION 3.22)
project(special_group_project)

# Set the C++ standard to C++23
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define the path for Paho MQTT library
set(PAHO_MQTT_DIR "./paho.mqtt.c")

# Include directories for Paho MQTT
include_directories(${PAHO_MQTT_DIR}/src)  # Sửa đường dẫn để chỉ tới thư mục src

# Link directories for Paho MQTT
link_directories(${PAHO_MQTT_DIR}/build/lib)

# Path to the nlohmann/json library
set(NLOHMANN_JSON_DIR "./json")

# Include directories for nlohmann/json
include_directories(${NLOHMANN_JSON_DIR}/include)

set(LIBPQXX "./libpqxx")
# Include directories for libpqxx
add_subdirectory(${LIBPQXX} build-pqxx)

include_directories(${PQXX_INCLUDE_DIRS})
link_directories(${PQXX_LIBRARY_DIRS})

# Declare the executable target
add_executable(special_group_project main.cpp Devices.cpp Devices.h Gateways.cpp Gateways.h Messages.cpp Messages.h
        Postgre_connection.cpp
        Postgre_connection.h)

# Link libraries (Ensure nlohmann_json is linked)
target_link_libraries(special_group_project PRIVATE pqxx paho-mqtt3c pthread)

