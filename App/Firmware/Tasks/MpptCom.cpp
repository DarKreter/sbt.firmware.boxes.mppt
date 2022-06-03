//
// Created by hubert25632 on 26.04.22.
//

#include "MpptCom.hpp"
#include <unordered_map>

constexpr uint8_t VEDirectGetCommand = 7;

// Predefined requests
const std::unordered_map<VEDirectParser::ParameterID, std::string> requests = {
    // Solar panel data
    {VEDirectParser::ParameterID::PanelPower, ":7BCED00A5\n"},
    {VEDirectParser::ParameterID::PanelCurrent, ":7BDED00A4\n"},
    {VEDirectParser::ParameterID::PanelVoltage, ":7BBED00A6\n"},
    // Charger data
    {VEDirectParser::ParameterID::ChargerInternalTemperature, ":7DBED0086\n"},
    {VEDirectParser::ParameterID::ChargerCurrent, ":7D7ED008A\n"},
    {VEDirectParser::ParameterID::ChargerVoltage, ":7D5ED008C\n"},
    // Yield data
    {VEDirectParser::ParameterID::YieldToday, ":7D3ED008E\n"},
    {VEDirectParser::ParameterID::MaximumPowerToday, ":7D2ED008F\n"}};

MpptCom::MpptCom(SBT::Hardware::UART* uart)
    : SBT::System::Task("MpptCom", 5, 128), uartDriver(uart)
{
    requestQueue = xQueueCreate(1, sizeof(VEDirectParser::ParameterID));
    parameterQueue = xQueueCreate(1, sizeof(int64_t));
}

void MpptCom::initialize() {}

void MpptCom::run()
{
    // Receive a request from the request queue
    VEDirectParser::ParameterID parameterId;
    xQueueReceive(requestQueue, &parameterId, portMAX_DELAY);
    std::string req = requests.at(parameterId);

    // Send the request and wait for the answer. Abort if requested.
    abort = false;
    uartDriver.Send(reinterpret_cast<uint8_t*>(req.data()), req.length());
    if(abort)
        return;
    do {
        while(!veDirectParser.readByteByByte(uartDriver.ReceiveByte()))
            if(abort)
                return;
    } while(!(veDirectParser.getCommand() == VEDirectGetCommand &&
              veDirectParser.getParameterID() == parameterId));

    // Get the parameter value returned by the MPPT and send it to the parameter
    // queue
    int64_t param;
    if(veDirectParser.getFlag()) {
        param = INT64_MIN;
    }
    else {
        switch(parameterId) {
        case VEDirectParser::ParameterID::PanelPower:
            param = veDirectParser.getParameterValue<uint32_t>();
            break;
        case VEDirectParser::ParameterID::PanelCurrent:
        case VEDirectParser::ParameterID::PanelVoltage:
        case VEDirectParser::ParameterID::ChargerCurrent:
        case VEDirectParser::ParameterID::ChargerVoltage:
        case VEDirectParser::ParameterID::YieldToday:
        case VEDirectParser::ParameterID::MaximumPowerToday:
            param = veDirectParser.getParameterValue<uint16_t>();
            break;
        case VEDirectParser::ParameterID::ChargerInternalTemperature:
            param = veDirectParser.getParameterValue<int16_t>();
            break;
        }
    }

    if(!abort)
        xQueueSend(parameterQueue, &param, 0);
}

void MpptCom::SendRequest(VEDirectParser::ParameterID parameterId)
{
    xQueueSend(requestQueue, &parameterId, 0);
}

BaseType_t MpptCom::ReceiveParameter(int64_t* parameterValue,
                                     TickType_t xTicksToWait)
{
    return xQueueReceive(parameterQueue, parameterValue, xTicksToWait);
}

void MpptCom::ResetParameterQueue() { xQueueReset(parameterQueue); }

void MpptCom::Abort()
{
    abort = true;
    uartDriver.Abort();
    veDirectParser.Abort();
}
