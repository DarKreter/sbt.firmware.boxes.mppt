//
// Created by hubert25632 on 12.02.2022.
//

#include "CommCAN.hpp"
#include "SBT-SDK.hpp"
#include "Tasks/UARTEcho.hpp"
#include "Tasks/UARTGatekeeper.hpp"

using namespace SBT::System;

// This is "main" - entry function that is called after system initialization
void entryPoint() {
  Init();

  Comm::CAN::Init(Comm::CAN_ID::Source::DEFAULT);

  auto UARTGatekeeperTask = std::make_shared<UARTGatekeeper>();
  TaskManager::registerTask(UARTGatekeeperTask);
  TaskManager::registerTask(std::make_shared<UARTEcho>(UARTGatekeeperTask));

  Start();
}
