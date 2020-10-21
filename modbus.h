#ifndef MODBUS_H
#define MODBUS_H

#include <cstdint>
#include <cstddef>
#include "serialprotocol.h"

namespace BVP {

class TModbus : public TSerialProtocol {

    /// Список состояний протокола.
    enum state_t {
        STATE_disable = 0,
        STATE_idle,
        STATE_reqSend,
        STATE_waitForReply,
        STATE_procReply,
        STATE_errorReply,
        //
        STATE_MAX
    };

    /// Список команд протокола.
    enum com_t {
        COM_readHoldingRegs = 0x03, /// Команда чтения регистров.
        COM_readWriteRegs = 0x17    /// Команда чтения/записи регистров.
    };


    /// Максимальное время для получения ответа.
    const uint32_t kMaxTimeToResponseUs = 200000UL;
    /// Максимальное количество сообщений без ответа до потери связи.
    const uint8_t kMaxLostMessage = 5;

public:
    TModbus();
    ~TModbus() override;

    /**
     * @brief setEnable
     * @param enable
     * @return
     */
    bool setEnable(bool enable) override;

    /// Проверяет наличие считанного сообщения.
    bool read() override;
    bool write() override;
    bool pop(uint8_t &byte) override;
    bool push(uint8_t byte) override;
    bool setNetAddress(uint16_t address) override;
    bool setup(uint32_t baudrate, bool parity, uint8_t stopbits) override;
    bool setTimeTick(uint32_t ticktimeus) override;
    void tick() override;
    bool isConnection() const override;

private:
    /// Адрес опрашиваемого устройства.
    uint8_t mNetAddress;
    /// Текущее состояние.
    state_t mState;
    /// Текущее время
    uint32_t mTimeUs;
    /// Период вызова функции tick.
    uint32_t mTimeTickUs;
    /// Время передачи/приема одного байта данных.
    uint32_t mTimeOneByteUs;
    /// Количество тиков для определения принятого сообщения.
    uint32_t mTimeToCompleteUs;
    /// Количество тиков для определения ошибки принятия сообщения.
    uint32_t mTimeToErrorUs;
    /// Количество сообщений без ответа.
    uint8_t cntLostMessage;

    /// Увеличивает счетчик ошибок без ответа.
    void incLostMessageCounter();
};

} // namespace BVP

#endif // MODBUS_H
