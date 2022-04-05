//
// Created by hubert25632 on 12.02.2022.
//

#include "CommCAN.hpp"
#include "ExampleTask.hpp"
#include "SBT-SDK.hpp"

using namespace SBT::System;

// This is "main" - entry function that is called after system initialization
void entryPoint() {
  Init();

  Comm::CAN::Init(Comm::CAN_ID::Source::DEFAULT);

  TaskManager::registerTask(std::make_shared<ExampleTask>());

  Start();
}
