add_executable(UARTEcho
        # Common files
        ../Common/common.cpp
        ../Common/vector_table.cpp
        # Starting procedure
        main.cpp
        # Common libraries
        ../Common/Libs/byte2hex.cpp
        # Common tasks
        ../Common/Tasks/UARTGatekeeper.cpp
        # Application-specific tasks
        Tasks/UARTEcho.cpp
        )

add_dependencies(UARTEcho STM32Cube-F1)
add_dependencies(UARTEcho FreeRTOS-Kernel)
add_dependencies(UARTEcho SBT-SDK)
