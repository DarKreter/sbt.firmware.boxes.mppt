if (DEFINED ENV{SBT_MPPT_CANTEST})
    set(MPPT_CANTEST Tasks/CANPrint.cpp)
endif ()

add_executable(Firmware
        # Common files
        ../Common/common.cpp
        ../Common/vector_table.cpp
        # Starting procedure
        main.cpp
        # Application-specific tasks
        Lib/UartDriver.cpp
        Lib/VEDirectParser.cpp
        Tasks/MpptCom.cpp
        Tasks/MpptCAN.cpp
        ${MPPT_CANTEST}
        )

include_directories(Tasks Lib)

add_dependencies(Firmware STM32Cube-F1)
add_dependencies(Firmware FreeRTOS-Kernel)
add_dependencies(Firmware SBT-SDK)
