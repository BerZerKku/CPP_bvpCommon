#ifndef TSERIALPROTOCOL_H
#define TSERIALPROTOCOL_H

#include <cstdint>
#include <QDebug>

namespace BVP {

class TSerialProtocol {
public:
    TSerialProtocol();
    virtual ~TSerialProtocol();

    /** Запуск или останов работы протокола.
     *
     *  !!! Настройки протокола должны быть сделаны до запуска.
     *
     *  @param[in] enable true если протокол надо запустить, иначе false.
     *  @return true если протокол запущен, иначе false.
     */
    virtual bool setEnable(bool enable) = 0;

    /** Обрабатывает принятые сообщения.
     *
     *  @return true если было принято и обработано сообщение, иначе false.
     */
    virtual bool read() = 0;

    /** Подготавливает сообщение для передачи.
     *
     *  @return true если есть сообщение для передачи, иначе false.
     */
    virtual bool write() = 0;

    /** Извлекает байт данных для передачи.
     *
     *  @param[out] byte Байт данных.
     * @return true если байт для передачи есть, иначе false.
     */
    virtual bool pop(uint8_t &byte) = 0;

    /** Добавляет принятый байт данных.
     *
     *  @param[in] byte Байт данных.
     *  @return true если байт обработан, иначе false.
     */
    virtual bool push(uint8_t byte) = 0;

    /** Устанавливает адрес устройства в локальной сети.
     *
     *  @param[in] address Адрес устройства в локальной сети.
     *  @return true если адрес установлен, иначе false.
     */
    virtual bool setNetAddress(uint16_t address) = 0;

    /** Устанавливает период вызова функции tick.
     *
     *  @param[in] ticktime Период вызова функции tick, мкс.
     *  @return true если период корректный, иначе false.
     */
    virtual bool setTimeTick(uint32_t ticktime) = 0;

    /** Устанавливает настройки последовательного порта.
     *
     *  @param[in] baudrate Скорость работы порта, бит/с.
     *  @param[in] parity true если включен контроль четности, иначе false.
     *  @param[in] stopbits Количество стоп-бит, 1 или 2.
     *  @return  true если настройки корректные, иначе false.
     */
    virtual bool setup(uint32_t baudrate, bool parity, uint8_t stopbits) = 0;

    /// Вызывается с периодом указанным в setupTick.
    virtual void tick() = 0;

    /** Устанавливает буфер и его размер.
     *
     *  @param[in] buf
     *  @param size
     */
    void setBuffer(uint8_t buf[], uint16_t size);

    /** Проверяет наличие связи.
     *
     *  @return true если связь есть, иначе false.
     */
    virtual bool isConnection() const = 0;

protected:
    uint8_t * const mBuf;   /// Буфер данных.
    const uint16_t mSize;   /// Размер буфера данных.
    uint16_t mPos;          /// Текущая позиция в буфере.
    uint16_t mLen;          /// Количество байт данных по протоколу.

};

} // namespace BVP

#endif // TSERIALPROTOCOL_H
