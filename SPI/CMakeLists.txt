cmake_minimum_required(VERSION 3.8)
project(SPIHardwareClasses)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES
        SPI.cpp
        MCU.cpp
        spipacketdemo.cpp
        spipacketplatform.cpp
        spipacket.cpp
        )
add_library(${PROJECT_NAME} ${SOURCE_FILES})


include_directories( ${CMAKE_SOURCE_DIR} )
target_link_libraries (${PROJECT_NAME}
        RestHardwareClasses
        )