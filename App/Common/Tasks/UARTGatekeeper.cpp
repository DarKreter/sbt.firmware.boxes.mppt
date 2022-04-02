//
// Created by hubert25632 on 09.02.2022.
//

#include "UARTGatekeeper.hpp"

// Create a task named "UARTGatekeeper" with priority 4
UARTGatekeeper::UARTGatekeeper() : SBT::System::Task("UARTGatekeeper", 4) {
  // Create a binary semaphore
  TxComplete = xSemaphoreCreateBinary();

  // Create a transmit queue. It will be able to hold 4 (at most) pointers to
  // std::string type objects
  TxQueue = xQueueCreate(4, sizeof(std::string*));

  // Create a reception queue. It will be able to hold 4 (at most) unsigned
  // bytes.
  RxQueue = xQueueCreate(4, sizeof(uint8_t));
}

void UARTGatekeeper::initialize() {
  // Use UART in DMA mode
  uart->ChangeModeToDMA();

  // Some parameters of UART can be changed before initialization using Set*()
  // functions (such as SetBaudRate()). See UART.hpp for more details.

  // Initialize the driver
  uart->Initialize();

  // Register interrupt callback functions. They will be called when a specific
  // event such as successful transmission or reception occurs.
  uart->RegisterCallback(UART::CallbackType::TxComplete, this,
                         &UARTGatekeeper::TxCallback);
  uart->RegisterCallback(UART::CallbackType::RxComplete, this,
                         &UARTGatekeeper::RxCallback);

  // Receive one byte of data. UART has been set to DMA mode so this function
  // will not block the execution of this task.
  uart->Receive(&received_data, 1);
}

void UARTGatekeeper::run() {
  std::string* s;

  // Receive a string from the transmit queue. If the queue is empty, block
  // indefinitely.
  xQueueReceive(TxQueue, &s, portMAX_DELAY);

  // Send a string to UART
  uart->Send(reinterpret_cast<uint8_t*>(s->data()), s->length());

  // Block indefinitely until the transmission is complete
  xSemaphoreTake(TxComplete, portMAX_DELAY);

  // Free memory used by the string
  delete s;
}

void UARTGatekeeper::TxCallback() {
  BaseType_t pxHigherPriorityTaskWoken = pdFALSE;

  // Give to a TxComplete semaphore unblocking the task waiting for it
  xSemaphoreGiveFromISR(TxComplete, &pxHigherPriorityTaskWoken);

  // Perform a context switch if necessary
  portYIELD_FROM_ISR(pxHigherPriorityTaskWoken)
}

void UARTGatekeeper::RxCallback() {
  BaseType_t pxHigherPriorityTaskWoken = pdFALSE;

  // Send a received byte to the reception queue. If the queue is full, drop the
  // byte.
  xQueueSendFromISR(RxQueue, &received_data, &pxHigherPriorityTaskWoken);

  // Receive the next byte
  uart->Receive(&received_data, 1);

  // Perform a context switch if necessary
  portYIELD_FROM_ISR(pxHigherPriorityTaskWoken)
}

void UARTGatekeeper::SendString(std::string* s) {
  // Put a string on a transmission queue. If the queue is full, block
  // indefinitely.
  xQueueSend(TxQueue, &s, portMAX_DELAY);
}

uint8_t UARTGatekeeper::ReceiveByte() {
  uint8_t b;

  // Receive a byte from the reception queue. If the queue is empty, block
  // indefinitely.
  xQueueReceive(RxQueue, &b, portMAX_DELAY);

  return b;
}
