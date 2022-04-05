//
// Created by hubert25632 on 09.02.2022.
//

#include "ExampleTask.hpp"

// Create a task named "UARTEcho" with priority 3 and custom stack size 256.
ExampleTask::ExampleTask() : SBT::System::Task("ExampleTask", 3, 256) {}

// This task does not need any initialization
void ExampleTask::initialize() {}

void ExampleTask::run() {}
