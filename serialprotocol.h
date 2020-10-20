#ifndef TSERIALPROTOCOL_H
#define TSERIALPROTOCOL_H

#include <cstdint>

namespace BVP {

class TSerialProtocol {

public:
    TSerialProtocol(uint8_t * const buf) : mBuf(buf) {}
    virtual ~TSerialProtocol();

    virtual bool setEnable(bool enable) = 0;
    virtual bool isConnection() const = 0;
    virtual bool isRead() const = 0;
    virtual bool isWrite() const = 0;
    virtual bool pop(uint8_t &byte) = 0;
    virtual bool push(uint8_t byte) = 0;
    virtual bool setupTick(uint16_t ticktime) = 0;
    virtual uint16_t setup(uint16_t baudrate, bool parity, uint8_t stopbits) = 0;
    virtual void tick() = 0;

private:
    bool connection = false;
    uint8_t * const mBuf = nullptr;
};

} // namespace BVP

#endif // TSERIALPROTOCOL_H
