add_executable(Firmware
        # Common files
        ../Common/common.cpp
        ../Common/vector_table.cpp
        # Starting procedure
        main.cpp
        # Application-specific tasks
        Tasks/ExampleTask.cpp
        )

include_directories(Tasks)

add_dependencies(Firmware STM32Cube-F1)
add_dependencies(Firmware FreeRTOS-Kernel)
add_dependencies(Firmware SBT-SDK)
