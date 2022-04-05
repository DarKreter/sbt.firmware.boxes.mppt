//
// Created by hubert25632 on 09.02.2022.
//

#ifndef F1XX_PROJECT_TEMPLATE_UARTECHO_HPP
#define F1XX_PROJECT_TEMPLATE_UARTECHO_HPP

#include "TaskManager.hpp"

class ExampleTask : public SBT::System::Task {

public:
  ExampleTask();

  // initialize() will be called once when task is created
  void initialize() override;

  // run() will be called repeatedly
  void run() override;
};

#endif // F1XX_PROJECT_TEMPLATE_UARTECHO_HPP
