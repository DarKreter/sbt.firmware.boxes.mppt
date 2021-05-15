cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/App/Startup/stm32f031x6.ld")

set(CPU_OPTIONS -mthumb -mcpu=cortex-m3)

add_compile_options(
    ${CPU_OPTIONS}
    -Werror
    -Wall
    -Wextra
    -pedantic
    -ffunction-sections
    -fdata-sections
    -Os
)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSTM32F103x6 -Wl,-gc-sections -Wl,--export-dynamic" CACHE INTERNAL "C++ Compiler options")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSTM32F103x6 -Wl,-gc-sections -Wl,--export-dynamic"  CACHE INTERNAL "C Compiler options")

set(CMAKE_CXX_FLAGS "-DSTM32F103xB -Wl,-gc-sections -Wl,--export-dynamic" CACHE INTERNAL "C++ Compiler options")
set(CMAKE_C_FLAGS "-DSTM32F103xB -Wl,-gc-sections -Wl,--export-dynamic"  CACHE INTERNAL "C Compiler options")

SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)

set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections -Wl,--export-dynamic --specs=nano.specs --specs=nosys.specs -T${LINKER_SCRIPT} -nostartfiles -mthumb -mcpu=cortex-m3" CACHE INTERNAL "Linker options")

project(f1xx-project-template
        VERSION 1.0.0
        DESCRIPTION "Template project"
        HOMEPAGE_URL ""
        LANGUAGES CXX C
        )

add_subdirectory(Lib)
add_subdirectory(Core)
add_subdirectory(App)
#add_subdirectory(Tests)
