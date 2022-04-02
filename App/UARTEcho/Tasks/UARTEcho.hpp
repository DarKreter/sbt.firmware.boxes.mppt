//
// Created by hubert25632 on 09.02.2022.
//

#ifndef F1XX_PROJECT_TEMPLATE_UARTECHO_HPP
#define F1XX_PROJECT_TEMPLATE_UARTECHO_HPP

#include "Tasks/UARTGatekeeper.hpp"

// This is a very simple task which receives bytes from UART and reports their
// reception using a human-readable message.

class UARTEcho : public SBT::System::Task {
  // Pointer to the UART gatekeeper task
  const std::shared_ptr<UARTGatekeeper> gatekeeperTask;

public:
  // The constructor in this task takes a pointer to the UART gatekeeper task as
  // an argument
  UARTEcho() = delete;
  UARTEcho(std::shared_ptr<UARTGatekeeper> gatekeeperTask);

  // initialize() will be called once when task is created
  void initialize() override;

  // run() will be called repeatedly
  void run() override;
};

#endif // F1XX_PROJECT_TEMPLATE_UARTECHO_HPP
