cmake_minimum_required(VERSION 3.1.0)

set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/App/Common/STM32F103CBTx_FLASH.ld")

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/toolchain-arm-none-eabi.cmake)

project(mppt-controller-firmware
        VERSION 1.0.0
        DESCRIPTION "MPPT project"
        HOMEPAGE_URL ""
        LANGUAGES CXX C
        )

add_subdirectory(Lib)
add_subdirectory(Core)
add_subdirectory(App)
#add_subdirectory(Tests)
