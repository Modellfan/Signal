#pragma once
#include <ACAN_T4.h>
#include <CANSignal.h>
#include <Arduino.h>
#include <CRC8.h>

CRC8 crc8;

const uint8_t finalxor[12] = {0xCF, 0xF5, 0xBB, 0x81, 0x27, 0x1D, 0x53, 0x69, 0x02, 0x38, 0x76, 0x4C};

//Final xor
//MSG XOR
//0x090 --
//0X100 0xB9
//0x110 --
//0x120 0xAA
//0x130 0x2D
//0x140 0xEE ?
//0x150 0xEE ?

uint8_t final_xor_candidates[256];

template <typename T>
class CANCRCSignal : public CANSignal<T>
{
public:
    /**
     * @brief Construct a new CANCounterSignal object
     *
     * @param name
     * @param unit
     * @param aliveTimeout
     * @param defaultValue
     * @param minValue
     * @param maxValue
     * @param start_bit
     * @param bit_len
     * @param scale
     * @param offset
     * @param endianess
     * @param sign
     */
    CANCRCSignal(const char *name, const char *unit, u_int32_t aliveTimeout, T defaultValue, T minValue, T maxValue,
                 const uint8_t start_bit, const uint8_t bit_len, const float scale,
                 const float offset, const sigEndianess endianess, const sigSign sign) : CANSignal<T>(name, unit, aliveTimeout, defaultValue, minValue, maxValue,
                                                                                                      start_bit, bit_len, scale,
                                                                                                      offset, endianess, sign)
    {
    }

    boolean unpackAndCheck(const CANMessage &msg)
    {
        if ((this->getBitLen()) == 8)
        {
            this->update(msg);

            this->getcheck(msg, this->get());
            // Serial.println("New");
            //  Serial.print("CRC should be according to message ");
            //  Serial.print(this->get(),HEX);
            //  Serial.print(" but is ");
            //  Serial.println(this->getcheck(msg), HEX);
        }
        else
        {
            handleError("Only 8 Bit CRC implemented")
        }
        return false;
    }

private:
    uint8_t getcheck(const CANMessage &msg, uint8_t crc)
    {
        int CMU = (msg.id & 0x00F);
        // Serial.println(CMU);
        uint8_t canmes[11];
        uint8_t meslen = msg.len + 1; // remove one for crc and add two for id bytes
        canmes[1] = msg.id;
        canmes[0] = msg.id >> 8;

        for (int i = 0; i < (msg.len - 1); i++)
        {
            canmes[i + 2] = msg.data[i];
        }

        for (int i = 0; i < 256; i++)
        {
            uint8_t crc_test = crc8.get_crc8(canmes, meslen, i);
            if (crc != crc_test)
            {
                final_xor_candidates[i] = 1;
            }
            if (final_xor_candidates[i] == 0)
            {
                Serial.print(i, HEX);
            }
            else
            {
                Serial.print("_");
            }
            Serial.print(",");
        }
        Serial.println();
        return (crc8.get_crc8(canmes, meslen, finalxor[CMU]));
    }
};
