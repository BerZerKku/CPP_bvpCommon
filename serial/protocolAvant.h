#ifndef PROTOCOL_AVANT_H
#define PROTOCOL_AVANT_H

#include <cstdint>
#include <cstddef>
#include "serialprotocol.h"

namespace BVP {

class TProtocolAvant : public TSerialProtocol {

  /// Список состояний протокола.
  enum state_t {
    STATE_disable = 0,
    STATE_idle,
    STATE_reqSend,
    STATE_waitForSend,
    STATE_waitForReply,
    STATE_procReply,
    STATE_errorReply,
    //
    STATE_MAX
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

public:
  TProtocolAvant();
  ~TProtocolAvant() override;

  bool setEnable(bool enable) override;
  bool read() override;
  void readError() override;
  bool write() override;
  uint16_t pop(uint8_t *data[]) override;
  void sendFinished() override;
  bool push(uint8_t byte) override;  
  bool setNetAddress(uint16_t address) override;
  bool setup(uint32_t baudrate, bool parity, uint8_t stopbits) override;
  bool setTimeTick(uint32_t ticktimeus) override;
  void setID(uint32_t id) override;
  void tick() override;
  bool isConnection() const override;

private:
  /// Источник доступа протокола.
  const src_t mSrc;
  /// Текущее состояние.
  state_t mState;
  /// Текущее время
  uint32_t mTimeUs;
  /// Период вызова функции tick.
  const uint32_t mTimeTickUs;
  /// Время передачи/приема одного байта данных.
  const uint32_t mTimeOneByteUs;
  /// Количество сообщений без ответа.
  uint8_t cntLostMessage;

  /// Увеличивает счетчик ошибок без ответа.
  void incLostMessageCounter();

  /** Вычисляет CRC для массива данных.
   *
   *  @param[in] buffer Массив данных.
   *  @param[in] len Количество данных в массиве.
   *  @param[in] crc Текущее значение CRC.
   *  @return Значение CRC.
   */
  uint8_t calcCRC(const uint8_t buf[], size_t len, uint8_t crc=0x00);
};

} // namespace BVP

#endif // PROTOCOL_AVANT_H
