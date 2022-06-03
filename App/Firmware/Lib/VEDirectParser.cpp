//
// Created by mateuszpopiel on 4/15/22.
//

#include "VEDirectParser.hpp"

constexpr uint8_t SIZE = 20;
constexpr uint8_t VEDirectExpectedChecksum = 0x55;

VEDirectParser::VEDirectParser()
    : uintMessage(new uint8_t[SIZE]), hasStarted(false), rxCurrLen(0),
      rxBuffer(new uint8_t[SIZE])
{
}

void VEDirectParser::ASCIItoUint()
{
    // Clear uintMessage
    for(uint8_t i = 0; i < SIZE; ++i) {
        uintMessage[i] = 0;
    }

    // Translate the frame from hex-noted nibbles to unsigned bytes skipping the
    // frame start delimiter (':'). Command ID is treated separately as it is
    // denoted as a single nibble.
    uintCommand = rxBuffer[1] - (rxBuffer[1] > '9' ? ('A' - 10) : '0');
    size_t i = 2;
    for(; rxBuffer[i] != '\n'; i++)
        uintMessage[i / 2 - 1] |=
            (rxBuffer[i] - (rxBuffer[i] > '9' ? ('A' - 10) : '0'))
            << (i % 2 ? 0 : 4);
    uintLength = i / 2 - 1;
}

bool VEDirectParser::checkMessage()
{
    // SIZE is length of table, aTsize we get from Victron
    if(rxCurrLen >= SIZE) {
        return false;
    }
    if(rxBuffer[0] == ':' && rxBuffer[rxCurrLen - 1] == '\n') {
        // Compute uintMessage
        ASCIItoUint();
        // Check control sum
        return checkControlSum();
    }
    else {
        return false;
    }
}

bool VEDirectParser::checkControlSum()
{
    // The sum of all message bytes (and apparently also a command ID) must
    // equal 0x55
    uint8_t sum = uintCommand;
    for(size_t i = 0; i < uintLength; i++)
        sum += uintMessage[i];
    return sum == VEDirectExpectedChecksum;
}

/** Returns true when received correct frame **/
bool VEDirectParser::readByteByByte(uint8_t _rxByte)
{
    // Frame always begins with ':'
    if(_rxByte == ':') {
        hasStarted = true;
        rxCurrLen = 0;
        rxBuffer[rxCurrLen++] = _rxByte;
        return false;
    }
    // If frame didn't start we wait for ':'
    if(!hasStarted) {
        return false;
    }
    // Frame has started, check if it's not too long
    if(rxCurrLen > SIZE) {
        // It's too long
        hasStarted = false;
        return false;
    }
    // Now make that _rxByte part of the frame
    rxBuffer[rxCurrLen++] = _rxByte;
    // Is it the last _rxByte of this frame?
    if(_rxByte == '\n') {
        // Frame received! Prepare to receive the next one.
        hasStarted = false;
        // Check if this frame makes sense
        return checkMessage();
    }
    return false;
}

uint8_t VEDirectParser::getCommand() const { return uintCommand; }

VEDirectParser::ParameterID VEDirectParser::getParameterID()
{
    uint16_t paramID;
    memcpy(&paramID, uintMessage, 2);
    return static_cast<ParameterID>(paramID);
}

uint8_t VEDirectParser::getFlag() { return uintMessage[2]; }

void VEDirectParser::Abort() { hasStarted = false; }
