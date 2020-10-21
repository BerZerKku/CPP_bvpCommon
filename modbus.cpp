#include "modbus.h"

namespace BVP {

/// Таблица CRC для полинома 0x8005 (LSB first)
static const uint16_t crc_ibm_table[256] = {
    0x0000, 0xc0c1, 0xc181, 0x0140, 0xc301, 0x03c0, 0x0280, 0xc241,
    0xc601, 0x06c0, 0x0780, 0xc741, 0x0500, 0xc5c1, 0xc481, 0x0440,
    0xcc01, 0x0cc0, 0x0d80, 0xcd41, 0x0f00, 0xcfc1, 0xce81, 0x0e40,
    0x0a00, 0xcac1, 0xcb81, 0x0b40, 0xc901, 0x09c0, 0x0880, 0xc841,
    0xd801, 0x18c0, 0x1980, 0xd941, 0x1b00, 0xdbc1, 0xda81, 0x1a40,
    0x1e00, 0xdec1, 0xdf81, 0x1f40, 0xdd01, 0x1dc0, 0x1c80, 0xdc41,
    0x1400, 0xd4c1, 0xd581, 0x1540, 0xd701, 0x17c0, 0x1680, 0xd641,
    0xd201, 0x12c0, 0x1380, 0xd341, 0x1100, 0xd1c1, 0xd081, 0x1040,
    0xf001, 0x30c0, 0x3180, 0xf141, 0x3300, 0xf3c1, 0xf281, 0x3240,
    0x3600, 0xf6c1, 0xf781, 0x3740, 0xf501, 0x35c0, 0x3480, 0xf441,
    0x3c00, 0xfcc1, 0xfd81, 0x3d40, 0xff01, 0x3fc0, 0x3e80, 0xfe41,
    0xfa01, 0x3ac0, 0x3b80, 0xfb41, 0x3900, 0xf9c1, 0xf881, 0x3840,
    0x2800, 0xe8c1, 0xe981, 0x2940, 0xeb01, 0x2bc0, 0x2a80, 0xea41,
    0xee01, 0x2ec0, 0x2f80, 0xef41, 0x2d00, 0xedc1, 0xec81, 0x2c40,
    0xe401, 0x24c0, 0x2580, 0xe541, 0x2700, 0xe7c1, 0xe681, 0x2640,
    0x2200, 0xe2c1, 0xe381, 0x2340, 0xe101, 0x21c0, 0x2080, 0xe041,
    0xa001, 0x60c0, 0x6180, 0xa141, 0x6300, 0xa3c1, 0xa281, 0x6240,
    0x6600, 0xa6c1, 0xa781, 0x6740, 0xa501, 0x65c0, 0x6480, 0xa441,
    0x6c00, 0xacc1, 0xad81, 0x6d40, 0xaf01, 0x6fc0, 0x6e80, 0xae41,
    0xaa01, 0x6ac0, 0x6b80, 0xab41, 0x6900, 0xa9c1, 0xa881, 0x6840,
    0x7800, 0xb8c1, 0xb981, 0x7940, 0xbb01, 0x7bc0, 0x7a80, 0xba41,
    0xbe01, 0x7ec0, 0x7f80, 0xbf41, 0x7d00, 0xbdc1, 0xbc81, 0x7c40,
    0xb401, 0x74c0, 0x7580, 0xb541, 0x7700, 0xb7c1, 0xb681, 0x7640,
    0x7200, 0xb2c1, 0xb381, 0x7340, 0xb101, 0x71c0, 0x7080, 0xb041,
    0x5000, 0x90c1, 0x9181, 0x5140, 0x9301, 0x53c0, 0x5280, 0x9241,
    0x9601, 0x56c0, 0x5780, 0x9741, 0x5500, 0x95c1, 0x9481, 0x5440,
    0x9c01, 0x5cc0, 0x5d80, 0x9d41, 0x5f00, 0x9fc1, 0x9e81, 0x5e40,
    0x5a00, 0x9ac1, 0x9b81, 0x5b40, 0x9901, 0x59c0, 0x5880, 0x9841,
    0x8801, 0x48c0, 0x4980, 0x8941, 0x4b00, 0x8bc1, 0x8a81, 0x4a40,
    0x4e00, 0x8ec1, 0x8f81, 0x4f40, 0x8d01, 0x4dc0, 0x4c80, 0x8c41,
    0x4400, 0x84c1, 0x8581, 0x4540, 0x8701, 0x47c0, 0x4680, 0x8641,
    0x8201, 0x42c0, 0x4380, 0x8341, 0x4100, 0x81c1, 0x8081, 0x4040,
};

/** Вычисляет CRC для одного байта.
 *
 *  @param[in] crc Текущее значение CRC.
 *  @param[in] c Байт данных.
 *  @return
 */
static inline uint16_t
crc_ibm_byte(uint16_t crc, const uint8_t byte) {
    const unsigned char lut = (crc ^ byte) & 0xFF;

    return (crc >> 8) ^ crc_ibm_table[lut];
}

/** Вычисляет CRC для массива данных.
 *
 *  @param[in] buffer Массив данных.
 *  @param[in] len Количество данных в массиве.
 *  @param[in] crc Текущее значение CRC.
 *  @return Значение CRC.
 */
uint16_t TModbus::calcCRC(const uint8_t buf[], size_t len, uint16_t crc) {
    for(size_t i = 0; i < len; i++) {
        crc = crc_ibm_byte(crc, buf[i]);
    }

    return crc;
}

//
TModbus::TModbus() :
    mNetAddress(255),
    mState(STATE_disable),
    mTimeUs(0),
    mTimeTickUs(0),
    mTimeOneByteUs(0),
    mTimeToCompleteUs(0),
    mTimeToErrorUs(0),
    cntLostMessage(kMaxLostMessage)
{

}

//
TModbus::~TModbus() {

}

//
bool
TModbus::setEnable(bool enable) {
    assert(mParam != nullptr);
    assert(mBuf != nullptr);
    assert(mSize != 0);

    if (enable) {
        mState = STATE_idle;
        mLen = 0;
        mTimeUs = 0;
        cntLostMessage = kMaxLostMessage;
        mState = STATE_idle;
    } else {
        mState = STATE_disable;
    }

    return mState != STATE_disable;
}

//
bool
TModbus::read() {
    bool isread = false;

    if (mState == STATE_procReply) {
        isread = true;
        mState = STATE_idle;
    }

    return isread;
}

//
bool
TModbus::write() {
    bool ok;
    uint16_t len = 0;

    if (mState == STATE_idle) {
        mBuf[len++] = mNetAddress;
        mBuf[len++] = COM_readWriteRegs;
        len += addReadRegMsg(&mBuf[len], REG_READ_MIN, REG_READ_MAX-1);
        len += addWriteRegMsg(&mBuf[len], REG_WRITE_MIN, REG_WRITE_MAX-1, ok);

        if (ok) {
            uint16_t crc =  calcCRC(mBuf, len);
            mBuf[len++] = static_cast<uint8_t> (crc);
            mBuf[len++] = static_cast<uint8_t> (crc >> 8);

            assert(len < 255);

            if (len > 0) {
                mLen = len;
                mPos = 0;
                mTimeUs = 0;
                mState = STATE_reqSend;
            }
        }

    }

    return mState == STATE_reqSend;
}

//
bool
TModbus::pop(uint8_t &byte) {
    uint16_t pos = mPos;

    if (mState == STATE_reqSend) {
        if (mPos < mLen) {
            byte = mBuf[mPos++];
        } else {
            mState = STATE_waitForReply;
            mPos = 0;
            mLen = 0;
            mTimeUs = 0;
        }
    }

    return mPos > pos;
}

//
bool
TModbus::push(uint8_t byte) {
    uint16_t len = mLen;

    if (mState == STATE_waitForReply) {
        if ((len < mSize) && (mTimeUs < mTimeToErrorUs)) {
            mBuf[mLen++] = byte;
            mTimeUs = 0;
        } else {
            mState = STATE_errorReply;
        }
    }

    if (mState == STATE_errorReply) {
        mTimeUs = 0;
    }

    return mLen > len;
}

//
bool
TModbus::setNetAddress(uint16_t address) {
    if ((address > 0) && (address <= 247)) {
        mNetAddress = static_cast<uint8_t> (address);
    }

    return mNetAddress == address;
}

//
bool
TModbus::setup(uint32_t baudrate, bool parity, uint8_t stopbits) {
    uint8_t nbites = 1 + 8 + stopbits + (parity ? 1 : 0);

    // TODO Проверить времена в железе. На ПК от них толка нет.

    mTimeOneByteUs = static_cast<uint32_t> ((1000000UL / baudrate) * nbites);
    mTimeToCompleteUs = (mTimeOneByteUs * 7) / 2;   // 3.5 char
    mTimeToErrorUs = (mTimeOneByteUs * 3) / 2;      // 1.5 char

#if defined(QT_CORE_LIB)
    // На ПК таймауты не выдерживаются. Просто так не задать меньше 1 мс.
    mTimeToCompleteUs *= 10;
    mTimeToErrorUs *= 10;
#endif

    return mTimeOneByteUs;
}

//
bool TModbus::setTimeTick(uint32_t ticktimeus) {
    mTimeTickUs = ticktimeus;

    return mTimeTickUs != 0;
}

//
void
TModbus::tick() {
    if (mState == STATE_disable) {
        return;
    }

    mTimeUs += mTimeTickUs;

    if (mState == STATE_waitForReply) {
        if (mLen == 0) {
            if (mTimeUs >  kMaxTimeToResponseUs) {
                mState = STATE_idle;
            }
        } else {
            if (mTimeUs > mTimeToCompleteUs) {
                mState = STATE_procReply;
                cntLostMessage = 0;
            }
        }
    }

    if (mState == STATE_errorReply) {
        if (mTimeUs > mTimeToCompleteUs) {
            mState = STATE_idle;
        }
    }
}

//
bool
TModbus::isConnection() const {
    return cntLostMessage < kMaxLostMessage;
}

//
void
TModbus::incLostMessageCounter() {
    if (cntLostMessage < kMaxLostMessage) {
        cntLostMessage++;
    }
}

//
uint16_t
TModbus::addReadRegMsg(uint8_t buf[], uint16_t minadr, uint16_t maxadr) {
    uint16_t len = 0;

    assert(minadr > 0);
    minadr--;
    buf[len++] = static_cast<uint8_t> (minadr >> 8);
    buf[len++] = static_cast<uint8_t> (minadr);

    assert(maxadr >= minadr);
    uint8_t quantity = static_cast<uint8_t> (maxadr - minadr + 1);
    buf[len++] = static_cast<uint8_t> (quantity >> 8);
    buf[len++] = static_cast<uint8_t> (quantity);

    return len;
}

//
uint16_t
TModbus::addWriteRegMsg(uint8_t buf[], uint16_t minnum, uint16_t maxnum, bool &ok) {
    uint16_t len = 0;

    assert(minnum > 0);
    uint16_t startaddress = minnum - 1;
    buf[len++] = static_cast<uint8_t> (startaddress >> 8);
    buf[len++] = static_cast<uint8_t> (startaddress);

    assert(maxnum >= minnum);
    uint8_t quantity = static_cast<uint8_t> (maxnum - minnum + 1);
    buf[len++] = static_cast<uint8_t> (quantity >> 8);
    buf[len++] = static_cast<uint8_t> (quantity);
    buf[len++] = static_cast<uint8_t> (quantity*2);

    for(uint8_t i = 0; i < quantity; i++) {
        bool ok;
        uint16_t value = getWriteRegMsgData(minnum + i, ok);
        if (!ok) {
            break;
        }
        buf[len++] = static_cast<uint8_t> (value >> 8);
        buf[len++] = static_cast<uint8_t> (value);
    }

    return len;
}

//
uint16_t
TModbus::getWriteRegMsgData(uint16_t number, bool &ok) const {
    uint16_t value = 0;

    if ((number < REG_WRITE_MIN) && (number >= REG_WRITE_MAX)) {
        ok = false;
    } else {
        switch(static_cast<regWrite_t> (number)) {
            case REG_WRITE_enSanSbSac: {
            } break;
            case REG_WRITE_enVv16to01: {
            } break;
            case REG_WRITE_enVv32to17: {
            } break;
            case REG_WRITE_enVv48to33: {
            } break;
            case REG_WRITE_enVv64to49: {
            } break;
            case REG_WRITE_dsSanSbSac: {
            } break;
            case REG_WRITE_dsVv16to01: {
            } break;
            case REG_WRITE_dsVv32to17: {
            } break;
            case REG_WRITE_dsVv48to33: {
            } break;
            case REG_WRITE_dsVv64to49: {
            } break;
            case REG_WRITE_MAX: break;
        }
        ok = true;
    }

    return value;
}


} // namespace BVP
