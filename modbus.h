#ifndef MODBUS_H
#define MODBUS_H

#include <cstdint>
#include <cstddef>
#include "serialprotocol.h"

namespace BVP {

class TModbus : public TSerialProtocol {

    enum com_t {
        COM_readHoldingRegs = 0x17  /// Команда чтения/записи регистров
    };

public:
    TModbus(uint8_t * const buf);
    ~TModbus() override;

    bool setEnable(bool enable) override;
    bool isConnection() const override;
    bool isRead() const override;
    bool isWrite() const override;
    bool pop(uint8_t &byte) override;
    bool push(uint8_t byte) override;
    uint16_t setup(uint16_t baudrate, bool parity, uint8_t stopbits) override;
    bool setupTick(uint16_t ticktimeus) override;
    void tick() override;

private:
    /// Время передачи/приема одного байта данных.
    uint16_t oneByteTimeUs;
    /// Количество тиков для определения принятого сообщения.
    uint16_t ticksToComplete;
    /// Количество тиков для определения ошибки принятия сообщения.
    uint16_t ticksToError;
};

} // namespace BVP

#endif // MODBUS_H
