#pragma once
#include <ACAN_T4.h>
#include <ComSignal.h>
#include <Arduino.h>

// SG_ CellVoltage0 : 0|16@1+ (0.001,0) [0|5] "V" BMS
//@0 = Big-Endian
//@1 = Little Endian
//+ signed value
//- unsigned value

#define DET_REPORTERROR(Error_Sender, Error_ID) ;

typedef enum
{
    PDU_NORMAL,
    PDU_CRC8,
    PDU_COUNTER
} PduType;


typedef enum
{
    COM_BIG_ENDIAN,
    COM_LITTLE_ENDIAN
} ComSignalEndianess_type;

typedef enum
{
    COM_UNSIGNED,
    COM_SIGNED
} ComSignalSignedness_type;

class PduSignal : public ComSignal
{
public:
    PduSignal(Com_SignalType signalType, const char *name, const char *unit, const u_int32_t aliveTimeout, const u_int32_t firstAliveTimeout,
              const u_int32_t defaultValue, const u_int32_t minValue, const u_int32_t maxValue,
              const uint8_t start_bit, const uint8_t bit_len, const float scale, const float offset,
              const ComSignalEndianess_type endianess, const ComSignalSignedness_type sign) : ComSignal(signalType, name, unit, aliveTimeout, firstAliveTimeout, defaultValue, minValue, maxValue),
                                                                                              _start_bit(start_bit),
                                                                                              _bit_len(bit_len),
                                                                                              _scale(scale),
                                                                                              _offset(offset),
                                                                                              _endianess(endianess),
                                                                                              _sign(sign)
    {
    }

    /**
     * @brief Unpack all information from a PDU to the according signals
     *
     * @param msg
     */
    void unpack(const CANMessage &msg)
    {
        uint32_t pduData = 0ULL;
        // Consider endianess
        if (this->_endianess == COM_BIG_ENDIAN)
        {
            uint32_t lsbIndex = ((this->_start_bit ^ 0x7u) + this->_bit_len - 1) ^ 7u; /* calculate lsb bit index. This could be moved to generator*/
            const uint8_t *pduDataPtr = ((&msg.data[lsbIndex / 8]) - 3);               /* calculate big endian ptr to data*/
            uint8_t bitShift = lsbIndex % 8;
            for (uint32_t i = 0; i < 4; i++)
            {
                pduData = (pduData << 8) | pduDataPtr[i];
            }
            pduData >>= bitShift;
            if ((32 - bitShift) < this->_start_bit)
            {
                pduData |= (uint32_t)pduDataPtr[-1] << (32u - bitShift);
            }
        }
        else
        {
            uint32_t lsbIndex = this->_start_bit;
            const uint8_t *pduDataPtr = &msg.data[(this->_start_bit / 8)];
            uint8_t bitShift = lsbIndex % 8;
            for (int32_t i = 3; i >= 0; i--)
            {
                pduData = (pduData << 8) | pduDataPtr[i];
            }
            pduData >>= bitShift;
            if ((32 - bitShift) < this->_bit_len)
            {
                pduData |= (uint32_t)pduDataPtr[4] << (32u - bitShift);
            }
        }

        uint32_t mask = 0xFFFFFFFFUL >> (32u - this->_bit_len); /* calculate mask for SigVal */
        pduData &= mask;                                 /* clear bit out of range */
        uint32_t signmask = ~(mask >> 1u);               // mask for signed / unsigned
        switch (this->getType())
        {
        case COM_SINT8:
            if (0 < (pduData & signmask))
            {
                pduData |= signmask; /* add sign bits */
            }
            /* sign extended data can be written as uint*/
            this->set(pduData+this->_offset);
            break;
        case COM_BOOLEAN:
            this->set(pduData+this->_offset);
            break;
        case COM_UINT8:
            this->set(pduData+this->_offset);
            break;
        case COM_SINT16:
            if (0 < (pduData & signmask))
            {
                pduData |= signmask; /* add sign bits*/
            }
            /* sign extended data can be written as uint*/
            this->set(pduData+this->_offset);
            break;
        case COM_UINT16:
            this->set(pduData+this->_offset);
            break;
        case COM_SINT32:
            if (0 < (pduData & signmask))
            {
                pduData |= signmask; /* add sign bits*/
            }
            /*sign extended data can be written as uint */
            this->set(pduData+this->_offset);
            break;
        case COM_UINT32:
            this->set(pduData+this->_offset);
            break;
        case COM_FLOAT:
            this->set((uint32_t)((float)pduData * this->_scale +this->_offset));
        default:
            break;
        }
    }

    u_int8_t getBitLen()
    {
        return _bit_len;
    }

    virtual PduType getPduType()
    {
        return PDU_NORMAL;
    }

private:
    const uint8_t _start_bit;                 // Starting bit of the signal in the message
    const uint8_t _bit_len;                   // Length of the signal in bits
    const float _scale;                       // Scale factor for the signal value
    const float _offset;                      // Offset applied to the signal value
    const ComSignalEndianess_type _endianess; // Endianess of the signal value
    const ComSignalSignedness_type _sign;     // Sign of the signal value (signed or unsigned)
};
