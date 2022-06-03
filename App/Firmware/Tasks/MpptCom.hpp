//
// Created by hubert25632 on 26.04.22.
//

#ifndef MPPT_CONTROLLER_FIRMWARE_MPPTCOM_HPP
#define MPPT_CONTROLLER_FIRMWARE_MPPTCOM_HPP

#include "TaskManager.hpp"
#include "UartDriver.hpp"
#include "VEDirectParser.hpp"
#include <FreeRTOS.h>
#include <queue.h>

class MpptCom : public SBT::System::Task {
    UartDriver uartDriver;
    VEDirectParser veDirectParser;
    QueueHandle_t requestQueue, parameterQueue;
    volatile bool abort;

public:
    explicit MpptCom(SBT::Hardware::UART* uart);
    void initialize() override;
    void run() override;

    void SendRequest(VEDirectParser::ParameterID parameterId);
    BaseType_t ReceiveParameter(int64_t* parameterValue,
                                TickType_t xTicksToWait);
    void ResetParameterQueue();
    void Abort();
};

#endif // MPPT_CONTROLLER_FIRMWARE_MPPTCOM_HPP
