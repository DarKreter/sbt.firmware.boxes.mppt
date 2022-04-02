//
// Created by hubert25632 on 09.02.2022.
//

#include "UARTEcho.hpp"
#include "Libs/byte2hex.hpp"

// Create a task named "UARTEcho" with priority 3 and custom stack size 256.
UARTEcho::UARTEcho(std::shared_ptr<UARTGatekeeper> gatekeeperTask)
    : SBT::System::Task("UARTEcho", 3, 256),
      gatekeeperTask(std::move(gatekeeperTask)) {}

// This task does not need any initialization
void UARTEcho::initialize() {}

void UARTEcho::run() {
  // Receive a byte using the gatekeeper task
  uint8_t b = gatekeeperTask->ReceiveByte();

  // Send the message using the gatekeeper task
  gatekeeperTask->SendString(
      new std::string("Data received: 0x" + byte2hex(b) + "\r\n"));
}
