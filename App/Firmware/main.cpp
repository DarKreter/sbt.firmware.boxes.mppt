//
// Created by hubert25632 on 12.02.2022.
//

#include "CommCAN.hpp"
#include "SBT-SDK.hpp"

#ifdef SBT_MPPT_CANTEST
#include "CANPrint.hpp"
#endif

#ifndef SBT_CAN_ID_MPPT1
#define SBT_CAN_ID_MPPT1 MPPT_1
#endif
#ifndef SBT_CAN_ID_MPPT2
#define SBT_CAN_ID_MPPT2 MPPT_2
#endif

#include "MpptCAN.hpp"

using namespace SBT::System;

// This is "main" - entry function that is called after system initialization
void entryPoint()
{
    Init();

    auto mpptCom1 = std::make_shared<MpptCom>(&SBT::Hardware::uart1);
    auto mpptCom2 = std::make_shared<MpptCom>(&SBT::Hardware::uart2);

    TaskManager::registerTask(mpptCom1);
    TaskManager::registerTask(mpptCom2);

    TaskManager::registerTask(
        std::make_shared<MpptCAN>(mpptCom1, CAN_ID::Source::SBT_CAN_ID_MPPT1));
    TaskManager::registerTask(
        std::make_shared<MpptCAN>(mpptCom2, CAN_ID::Source::SBT_CAN_ID_MPPT2));

#ifdef SBT_MPPT_CANTEST
    TaskManager::registerTask(std::make_shared<CANPrint>());
#endif

    Start();
}
