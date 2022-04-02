//
// Created by hubert25632 on 09.02.2022.
//

#ifndef F1XX_PROJECT_TEMPLATE_UARTGATEKEEPER_HPP
#define F1XX_PROJECT_TEMPLATE_UARTGATEKEEPER_HPP

#include "TaskManager.hpp"
#include "UART.hpp"
#include <FreeRTOS.h>
#include <semphr.h>

using namespace SBT::Hardware;

// This is a gatekeeper task for UART. Its sole purpose is to be the only task
// allowed to interface with UART directly.

class UARTGatekeeper : public SBT::System::Task {
  // Pointer to the UART device. It can be changed just by changing this
  // definition.
  UART* const uart = &uart1;

  // This semaphore will be given by TxCallback and taken by run()
  SemaphoreHandle_t TxComplete;

  // Tx and Rx data queue
  QueueHandle_t TxQueue, RxQueue;

  // Received data buffer
  uint8_t received_data;

public:
  UARTGatekeeper();

  // initialize() will be called once when task is created
  void initialize() override;

  // run() will be called repeatedly
  void run() override;

  // Callback functions
  void TxCallback();
  void RxCallback();

  // Send a string to UART
  void SendString(std::string* s);

  // Receive one byte from UART
  uint8_t ReceiveByte();
};

#endif // F1XX_PROJECT_TEMPLATE_UARTGATEKEEPER_HPP
