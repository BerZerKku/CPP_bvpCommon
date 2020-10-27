﻿#ifndef MODBUS_H
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

    enum regRead_t {
        REG_READ_MIN = 1,
        //
        REG_READ_sanSbSac = REG_READ_MIN,
        REG_READ_sa16to01,
        REG_READ_sa32to17,
        REG_READ_sa48to33,
        REG_READ_sa64to49,
        //
        REG_READ_MAX
    };

    enum regWrite_t {
        REG_WRITE_MIN = 0x0B,
        //
        REG_WRITE_enSanSbSac = REG_WRITE_MIN,
        REG_WRITE_enLed16to01,
        REG_WRITE_enLed32to17,
        REG_WRITE_enLed48to33,
        REG_WRITE_enLed64to49,
        REG_WRITE_dsSanSbSac,
        REG_WRITE_dsLed16to01,
        REG_WRITE_dsLed32to17,
        REG_WRITE_dsLed48to33,
        REG_WRITE_dsLed64to49,
        //
        REG_WRITE_MAX
    };

    /// Список команд протокола.
    enum com_t {
        COM_readHoldingRegs = 0x03, /// Команда чтения регистров.
        COM_writeMultRegs = 0x10,   /// Команда записи регистров.
        COM_readWriteRegs = 0x17    /// Команда чтения/записи регистров.
    };


    /// Максимальное время для получения ответа (в данной реализации).
    const uint32_t kMaxTimeToResponseUs = 200000UL;
    /// Максимальное количество сообщений без ответа до потери связи.
    const uint8_t kMaxLostMessage = 5;
    /// Максимальная длинна RTU кадра
    const uint16_t kMaxSizeFrameRtu = 256;

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

    static uint16_t calcCRC(const uint8_t buf[], size_t len, uint16_t crc=0xFFFF);

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

    /** Добавляет в сообщение запрос для считывания регистров.
     *
     *  Адрес регистра в сообщении на единицу меньше номера.
     *  Если аргумент ok при вызове имеет значение false, то сразу на выход..
     *
     *  @param[out] buf Начальная позиция в буфере сообщения.
     *  @param min Номер младшего регистра.
     *  @param max Номер старшего регистра.
     *  @param[in/out] ok true если номера регистров найдены, иначе false.
     *  @return количество заполненных байт в сообщении.
     */
    uint16_t addReadRegMsg(uint8_t buf[], uint16_t min, uint16_t max, bool &ok);

    /** Добавляет в сообщение данные для записи регистров.
     *
     *  Адрес регистра в сообщении на единицу меньше номера.
     *  Если аргумент ok при вызове имеет значение false, то сразу на выход.
     *
     *  @param[out] buf Начальная позиция в буфере сообщения.
     *  @param[in] min Номер младшего регистра.
     *  @param[in] max Номер старшего регистра.
     *  @param[in/out] ok true если номера регистров найдены, иначе false.
     *  @return количество заполненных байт в сообщении.
     */
    uint16_t addWriteRegMsg(uint8_t buf[], uint16_t min, uint16_t max, bool &ok);

    /** Проверяет принятое сообщение.
     *
     *  После проверки CRC количество необработанных байт в сообщении
     *  уменьшается на 2.
     *  Если аргумент ok при вызове имеет значение false, то сразу на выход.
     *
     *  @param[in] buf Начальная позиция в буфере сообщения.
     *  @param[in/out] Количество необработанных байт в сообщении.
     *  @param[in/out] ok true если проверка пройдена, иначе false.
     *  @return количество обработанных байт в сообещнии.
     */
    uint16_t checkReadMsg(const uint8_t buf[], uint16_t &len, bool &ok);

    /** Возвращает значение для указанного номера регистра.
     *
     *  Если аргумент ok при вызове имеет значение false, то сразу на выход.
     *
     *  @param[in] number Номер регистра.
     *  @param[in/out] ok true если номер регистра найден, иначе false.
     *  @return Значенеи регистра.
     */
    uint16_t getWriteRegMsgData(uint16_t number, bool &ok) const;

    /** Извлекает значения регистров из сообщения.
     *
     *  Если аргумент ok при вызове имеет значение false, то сразу на выход.
     *
     *  @param[in] buf Начальная позиция в буфере сообщения.
     *  @param[in/out] Количество необработанных байт в сообщении.
     *  @param[in] min Номер младшего регистра.
     *  @param[in] max Номер старшего регистра.
     *  @param[in/out] ok true если номера регистров найдены, иначе false.
     *  @return количество обработанных байт в сообщении.
     */
    uint16_t getReadReg(const uint8_t buf[], uint16_t &len, uint16_t min,
                           uint16_t max, bool &ok);

    /** Извлекает значение для указанного номера регистра.
     *
     *  Если аргумент ok при вызове имеет значение false, то сразу на выход.
     *
     *  @param[in] buf Начальная позиция в буфере сообщения.
     *  @param[in] number Номер регистра.
     *  @param[in/out] ok true если данные для регистра обработаны, иначе false.
     *  @return количество обработанных байт в сообщении.
     */
    uint16_t getReadRegMsgData(const uint8_t buf[], uint16_t number, bool &ok);
};

} // namespace BVP

#endif // MODBUS_H
