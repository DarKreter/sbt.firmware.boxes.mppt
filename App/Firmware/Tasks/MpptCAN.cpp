//
// Created by hubert25632 on 27.04.22.
//

#include "MpptCAN.hpp"

// Parameters to be requested in loop
constexpr std::array<VEDirectParser::ParameterID, 6> parameters = {
    VEDirectParser::ParameterID::PanelPower,
    VEDirectParser::ParameterID::PanelCurrent,
    VEDirectParser::ParameterID::PanelVoltage,
    VEDirectParser::ParameterID::ChargerInternalTemperature,
    VEDirectParser::ParameterID::ChargerCurrent,
    VEDirectParser::ParameterID::ChargerVoltage};

// Parameters to be requested in loop with low frequency
constexpr std::array<VEDirectParser::ParameterID, 2> yield_parameters{
    VEDirectParser::ParameterID::YieldToday,
    VEDirectParser::ParameterID::MaximumPowerToday};

MpptCAN::MpptCAN(std::shared_ptr<MpptCom> mpptCom, CAN_ID::Source canSourceID)
    : SBT::System::PeriodicTask("MpptCAN", 3, 100, 128),
      mpptCom(std::move(mpptCom)), canSourceID(canSourceID),
      paramIter(parameters.begin()), cycleCounter(0)
{
}

void MpptCAN::initialize() {}

void MpptCAN::run()
{
    // To preserve the currently expected parameter ID
    const VEDirectParser::ParameterID parameterId = *paramIter;

    int64_t param;

    mpptCom->ResetParameterQueue();
    mpptCom->SendRequest(parameterId);

    // Try to receive the requested parameter. If it does not arrive within the
    // specified timeout, abort and retry.
    if(mpptCom->ReceiveParameter(&param, 100) == pdFALSE) {
        mpptCom->Abort();
        paramIter = parameters.begin();
        return;
    }
    else
        paramIter++;

    // Continue iteration or switch to the other parameter group
    if(paramIter == parameters.end())
        paramIter = (++cycleCounter < 40 ? parameters.begin()
                                         : yield_parameters.begin());
    else if(paramIter == yield_parameters.end()) {
        paramIter = parameters.begin();
        cycleCounter = 0;
    }

    switch(parameterId) {
    case VEDirectParser::ParameterID::PanelPower: {
        pvData.panelPower = param;
        break;
    }
    case VEDirectParser::ParameterID::PanelCurrent: {
        pvData.panelCurrent = param;
        break;
    }
    case VEDirectParser::ParameterID::PanelVoltage: {
        pvData.panelVoltage = param;

        // PanelVoltage is the last to be requested in this group in a single
        // cycle. Construct a CAN frame and send it.
        Pack_PV_DATA(&pvData, canPayload);
        CAN::Send(canSourceID, CAN_ID::Message::PV_DATA, canPayload);
        break;
    }
    case VEDirectParser::ParameterID::ChargerInternalTemperature: {
        chargerData.internalTemperature = static_cast<int16_t>(param);
        break;
    }
    case VEDirectParser::ParameterID::ChargerCurrent: {
        chargerData.batteryCurrent = param;
        break;
    }
    case VEDirectParser::ParameterID::ChargerVoltage: {
        chargerData.batteryVoltage = param;

        // ChargerVoltage is the last to be requested in this group in a single
        // cycle. Construct a CAN frame and send it.
        Pack_MPPT_CHARGER_DATA(&chargerData, canPayload);
        CAN::Send(canSourceID, CAN_ID::Message::MPPT_CHARGER_DATA, canPayload);
        break;
    }
    case VEDirectParser::ParameterID::YieldToday: {
        yieldData.yieldToday = param;
        break;
    }
    case VEDirectParser::ParameterID::MaximumPowerToday: {
        yieldData.maximumPowerToday = param;

        // MaximumPowerToday is the last to be requested in this group in a
        // single cycle. Construct a CAN frame and send it.
        Pack_YIELD_DATA(&yieldData, canPayload);
        CAN::Send(canSourceID, CAN_ID::Message::YIELD_DATA, canPayload);
        break;
    }
    }
}
