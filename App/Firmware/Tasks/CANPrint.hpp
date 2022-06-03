//
// Created by hubert25632 on 24.04.22.
//

#ifndef MPPT_CONTROLLER_FIRMWARE_CANPRINT_HPP
#define MPPT_CONTROLLER_FIRMWARE_CANPRINT_HPP

#include "CommCAN.hpp"
#include "TaskManager.hpp"
#include "UART.hpp"
#include <FreeRTOS.h>
#include <queue.h>

using namespace SBT::Hardware;
using namespace SBT::System::Comm;

class CANPrint : public SBT::System::Task {
    UART* const uart = &uart3;
    QueueHandle_t messageQueue;

    void canCallback(CAN::RxMessage rxMessage);

public:
    CANPrint();
    void initialize() override;
    void run() override;
};

#endif // MPPT_CONTROLLER_FIRMWARE_CANPRINT_HPP
