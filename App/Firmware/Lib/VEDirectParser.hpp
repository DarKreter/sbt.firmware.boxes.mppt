//
// Created by mateuszpopiel on 4/15/22.
//

#ifndef MPPT_CONTROLLER_FIRMWARE_VEDIRECTPARSER_HPP
#define MPPT_CONTROLLER_FIRMWARE_VEDIRECTPARSER_HPP

#include <cstdint>
#include <cstring>

class VEDirectParser {
    uint8_t* uintMessage;
    uint8_t uintCommand;
    size_t uintLength;

    bool hasStarted;
    uint8_t rxCurrLen;
    uint8_t* rxBuffer;

    void ASCIItoUint();
    bool checkControlSum();
    bool checkMessage();

public:
    enum class ParameterID : uint16_t {
        PanelPower = 0xEDBC,
        PanelCurrent = 0xEDBD,
        PanelVoltage = 0xEDBB,
        ChargerInternalTemperature = 0xEDDB,
        ChargerCurrent = 0xEDD7,
        ChargerVoltage = 0xEDD5,
        YieldToday = 0xEDD3,
        MaximumPowerToday = 0xEDD2
    };

    explicit VEDirectParser();
    bool readByteByByte(uint8_t _rxByte);

    [[nodiscard]] uint8_t getCommand() const;
    ParameterID getParameterID();
    uint8_t getFlag();

    // This template allows acquisition of an arbitrary-type parameter from a
    // GetResponse frame
    template <class T> T getParameterValue()
    {
        T parameterValue;
        // The parameter value follows 2 bytes of parameter ID and 1 byte of
        // flags
        memcpy(&parameterValue, uintMessage + 3, sizeof(T));
        return parameterValue;
    }

    void Abort();
};

#endif // MPPT_CONTROLLER_FIRMWARE_VEDIRECTPARSER_HPP
