cmake_minimum_required(VERSION 3.8)
project(HardwareClasses)

set(CMAKE_CXX_STANDARD 14)

#set(SOURCE_FILES
#        Gpio/GPIOSysfs.cpp
#        Gpio/gpiointerface.cpp
#        Backlight/screenlight.cpp
#        Backlight/backlight_driver.cpp
#        Backlight/backlight_pwm.cpp
#        Backlight/backlightinterface.cpp
#        SPI/SPI.cpp
#        SPI/MCU.cpp
#       Rest/crc.cpp
#        I2C/I2C.cpp
#        I2C/ConnModule.cpp
#        I2C/I2C.cpp
#        I2C/BoardModule.cpp
#        Rest/crc.cpp
#        )

add_library(${PROJECT_NAME} ${SOURCE_FILES})

#add_definitions(-DDEMO)
include_directories( ${PROJECT_SOURCE_DIR} )
include_directories( ${PROJECT_SOURCE_DIR}/SPI )
add_subdirectory( ${PROJECT_SOURCE_DIR}/SPI )
include_directories( ${PROJECT_SOURCE_DIR}/Rest )
add_subdirectory( ${PROJECT_SOURCE_DIR}/Rest )
include_directories( ${PROJECT_SOURCE_DIR}/I2C )
add_subdirectory( ${PROJECT_SOURCE_DIR}/I2C )
include_directories( ${PROJECT_SOURCE_DIR}/Gpio )
add_subdirectory( ${PROJECT_SOURCE_DIR}/Gpio )
include_directories( ${PROJECT_SOURCE_DIR}/Backlight )
add_subdirectory( ${PROJECT_SOURCE_DIR}/Backlight )
include_directories( ${PROJECT_SOURCE_DIR}/Serial )
add_subdirectory( ${PROJECT_SOURCE_DIR}/Serial )
target_link_libraries (${PROJECT_NAME}
        SPIHardwareClasses
        RestHardwareClasses
        I2CHardwareClasses
        GpioHardwareClasses
        BacklightHardwareClasses
        SerialHardwareClasses
        )

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)