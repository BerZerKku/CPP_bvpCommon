#ifndef PROTOCOL_AVANT_H
#define PROTOCOL_AVANT_H

#include <cstdint>
#include <cstddef>
#include "serialprotocol.h"

namespace BVP {

enum comAvant_t {
  COM_AVANT_time = 0x32,
  COM_AVANT_control = 0x72,
  COM_AVANT_prmResetInd = 0x9A
};

class TProtocolAvant : public TSerialProtocol {

  /// Список состояний протокола.
  enum state_t {
    STATE_disable = 0,
    STATE_idle,
    STATE_reqSend,
    STATE_waitSendFinished,
    STATE_waitForReply,
    STATE_procReply,
    STATE_errorReply,
    //
    STATE_MAX
  };

  /// Максимальное время для получения ответа (в данной реализации), мкс.
  const uint32_t kMaxTimeToResponseUs = 300000UL;
  /// Максимальное количество сообщений без ответа до потери связи.
  const uint8_t kMaxLostMessage = 5;
  /// Минимальная длина кадра.
  const uint16_t kMinLenFrame = 5;  // pmbl1 + pmbl2 + cm + data_len + check_sum

public:

  TProtocolAvant();
  ~TProtocolAvant() override;

  bool setEnable(bool enable) override;
  bool isEnable() const override;
  bool read() override;
  void readError() override;
  bool write() override;
  uint16_t pop(uint8_t *data[]) override;
  void sendFinished() override;
  bool vPush(uint8_t byte) override;
  bool setNetAddress(uint16_t address) override;
  bool vSetup(uint32_t baudrate, bool parity, uint8_t stopbits) override;
  void setID(uint32_t id) override;
  void vTick() override;
  bool isConnection() const override;

protected:
  /// Структура посылки
  enum pos_t {
    POS_PMBL_55 = 0,    ///< Первый байт преамбулы 0x55
    POS_PMBL_AA,        ///< Второй байт преамбулы 0xAA
    POS_COM,            ///< Команда.
    POS_DATA_LEN,       ///< Количество байт данных.
    POS_DATA            ///< Первый байт данных.
  };

  const src_t mSrc;       ///< Источник доступа протокола.
  state_t mState;         ///< Текущее состояние.
  uint8_t cntLostMessage; ///< Количество сообщений без ответа.

  /// Увеличивает счетчик ошибок без ответа.
  void incLostMessageCounter();

  /** Устанавливает команду для передачи.
   *
   *  Сбрасывает количество байт данных для передачи.
   *
   *  @param[in] com Команда.
   */
  void setCom(uint8_t com);

  /** Добавляет байт данных в указанную позицию сообщения.
   *
   *  @param[in] byte Байт данных.
   *  @param[in] nbyte Номер байта данных начиная с 1. Если 0 то в конец сообщения.
   */
  void addByte(uint8_t byte, uint16_t nbyte=0);

  /** Вычисляет контрольную сумму для массива данных.
   *
   *  Преамбула в подсчете контрольной суммы не участвует, поэтому передавать
   *  сюда не надо.
   *
   *  @param[in] buffer Массив данных.
   *  @param[in] len Количество данных в массиве.
   *  @param[in] crc Текущее значение контрольной суммы.
   *  @return Значение контрольной суммы.
   */
  uint8_t calcCRC(const uint8_t buf[], size_t len, uint8_t crc=0x00);

  /** Подготавливает сообщение для передачи.
   *
   *  Подготовка для протокола.
   *
   *  @return true если есть сообщение для передачи, иначе false.
   */
  virtual bool vWriteAvant() = 0;

  /** Обрабатывает принятые сообщения.
   *
   *  Обработка для протокола.
   *
   *  @return true если было принято и обработано сообщение, иначе false.
   */
  virtual bool vReadAvant() = 0;
};

} // namespace BVP

#endif // PROTOCOL_AVANT_H
