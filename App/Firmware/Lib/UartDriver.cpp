//
// Created by mateuszpopiel on 4/8/22.
//

#include "UartDriver.hpp"

constexpr uint32_t VICTRON_BAUDRATE = 19200;

using namespace SBT::Hardware;

UartDriver::UartDriver(SBT::Hardware::UART* uart)
    : uart(uart), txSemaphore(xSemaphoreCreateBinary()),
      rxSemaphore(xSemaphoreCreateBinary())
{
    uart->SetBaudRate(VICTRON_BAUDRATE);
    uart->ChangeModeToDMA();
    uart->Initialize();
    // Register callback functions
    uart->RegisterCallback(UART::CallbackType::TxComplete, this,
                           &UartDriver::TxCallback);
    uart->RegisterCallback(UART::CallbackType::RxComplete, this,
                           &UartDriver::RxCallback);
}

void UartDriver::Send(uint8_t* _data, uint8_t _numOfBytes)
{
    uart->Send(_data, _numOfBytes);
    xSemaphoreTake(txSemaphore, portMAX_DELAY);
}

uint8_t UartDriver::ReceiveByte()
{
    uint8_t receivedByte;
    uart->Receive(&receivedByte, 1);
    xSemaphoreTake(rxSemaphore, portMAX_DELAY);
    return receivedByte;
}

void UartDriver::Abort()
{
    if(!uart->IsTxComplete()) {
        uart->AbortTx();
        xSemaphoreGive(txSemaphore);
    }
    if(!uart->IsRxComplete()) {
        uart->AbortRx();
        xSemaphoreGive(rxSemaphore);
    }
}

void UartDriver::TxCallback()
{
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(txSemaphore, &pxHigherPriorityTaskWoken);
    // Perform a context switch if necessary
    portYIELD_FROM_ISR(pxHigherPriorityTaskWoken)
}

void UartDriver::RxCallback()
{
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(rxSemaphore, &pxHigherPriorityTaskWoken);
    // Perform a context switch if necessary
    portYIELD_FROM_ISR(pxHigherPriorityTaskWoken)
}
