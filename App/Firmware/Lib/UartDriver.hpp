//
// Created by mateuszpopiel on 4/8/22.
//

#ifndef MPPT_CONTROLLER_FIRMWARE_UARTDRIVER_HPP
#define MPPT_CONTROLLER_FIRMWARE_UARTDRIVER_HPP

#include "UART.hpp"
#include <FreeRTOS.h>
#include <semphr.h>

// High-level UART driver (using DMA and semaphores)
class UartDriver {
    SBT::Hardware::UART* const uart;
    // Semaphores given by RxCallback and taken by Send
    SemaphoreHandle_t txSemaphore, rxSemaphore;
    // Callback functions
    void TxCallback();
    void RxCallback();

public:
    UartDriver() = delete;
    explicit UartDriver(SBT::Hardware::UART* uart);
    void Send(uint8_t* _data, uint8_t _numOfBytes);
    uint8_t ReceiveByte();
    void Abort();
};

#endif // MPPT_CONTROLLER_FIRMWARE_UARTDRIVER_HPP
