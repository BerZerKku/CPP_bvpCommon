#ifndef TPARAM_H
#define TPARAM_H

#include <stdint.h>
#include "debug.hpp"
#include "hardware.hpp"

namespace BVP {

enum param_t {
  //
  PARAM_control,          ///< Сигналы управления.
  // Параметры панели виртуальных ключей
  PARAM_dirControl,       ///< Управление ключами (SAC2).
  PARAM_blkComPrmAll,     ///< Блокировка всех выходов приемника (SAC1).
  PARAM_blkComPrmDir,     ///< Блокировка направляений выхода приемника (SAnn.x)
  PARAM_blkComPrm32to01,  ///< Блокированные команды приемника с 1 по 32.
  PARAM_blkComPrm64to33,  ///< Блокированные команды приемника с 33 по 64.
  PARAM_blkComPrd32to01,  ///< Блокированные команды передатчика с 1 по 32.
  PARAM_blkComPrd64to33,  ///< Блокированные команды передатчика с 33 по 64.
  // Кнопки панели виртуальных ключей.
  PARAM_vpBtnSAnSbSac,
  PARAM_vpBtnSA32to01,
  PARAM_vpBtnSA64to33,
  //
  PARAM_MAX
};

/// Источник доступа к параметрам.
enum src_t {
  SRC_pi = 0, ///< БСП-ПИ
  SRC_pc,     ///< Конфигуратор ПК
  SRC_acs,    ///< АСУ
  SRC_vkey,   ///< Панель виртуальных ключей.
  //
  SRC_MAX
};

/// Сигналы управления.
enum ctrl_t {
  CTRL_MIN = 1,
  //
  CTRL_resetErrors = CTRL_MIN,
  CTRL_resetIndication,
  //
  CTRL_MAX

};

/// Управление ключами
enum dirControl_t {
  DIR_CONTROL_local = 0,
  DIR_CONTROL_remote,
  //
  DIR_CONTROL_MAX
};

///
enum onOff_t {
  ON_OFF_off = 0,
  ON_OFF_on,
  //
  ON_OFF_MAX
};

///

/** Класс параметров.
 *
 *  Создан на подобие Одиночки (single-tone).конструктор спрятан в protected,
 *  а создание объекта new происходит при первом вызове getInstance).
 *  Но т.к. объект должен существовать всегда, его конструктор открыт и
 *  при вызове getInstance нет необходимости проверки первого запуска.
 *
 *  В каждом классе где используются параметры необходимо взять указатель
 *  на созданный объект (getInstance).
 *
 * Создавать объект где-либо нельзя, он создается сразу!!!
 */
class TParam {
  /// Параметры.
  static TParam mParam;

  struct paramFields_t {
    param_t param;
    bool isValue;
    uint32_t rValue;
    uint32_t wValue;
    bool (*set) (BVP::TParam*, BVP::src_t, uint32_t&);
    bool (*get) (BVP::TParam*, BVP::src_t, uint32_t&);
  };

  enum vpBtnControl_t {
    VP_BTN_CONTROL_sac1 = 0x00000001,
    VP_BTN_CONTROL_sac2 = 0x00000002,
    VP_BTN_CONTROL_sb   = 0x00000004,
    VP_BTN_CONTROL_san  = 0x0000FF00
  };

public:
  /// Конструктор.
  TParam();
  /// Конструктор копирования запрещен.
  TParam(TParam &other) = delete;
  /// Конструктор присваивания запрещен.
  void operator=(const TParam&) = delete;

  /** Возвращает экземпляр класса параметров.
     *
     *  @return Класс параметров.
     */
  static TParam* getInstance();

  /** Проверяет наличие установленного значения параметра.
   *
   *  @param[in] param Параметр.
   *  @return true если значение было установлено, иначе false.
   */
  bool isValueSet(param_t param) const;

  /** Проверяет возможность чтения параметра из указанного источника.
   *
   *  @param[in] param Параметр.
   *  @param[in] src Источник доступа.
   *  @return true если чтение разрешено, иначе false.
   */
  bool isAccessRead(param_t param, src_t src) const;

  /** Проверяет возможность установки параметра из указанного источника.
   *
   *  @param[in] param Параметр.
   *  @param[in] src Источник доступа.
   *  @return true если запись разрешена, иначе false.
   */
  bool isAccessSet(param_t param, src_t src) const;

  /** Возвращает значение параметра.
   *
   *  Значение не будет получено, если параметр еще не установлен или
   *  не достаточно прав доступа.
   *
   *  @param[in] param Параметр.
   *  @param[in] src Источник доступа.
   *  @param[out] ok true если значение считано, иначе false.
   *  @return Значение параметра.
   */
  uint32_t getValue(param_t param, src_t src, bool &ok);

  /** Устанавливает значение параметра.
   *
   *  @param[in] param Параметр.
   *  @param[in] src Источник доступа.
   *  @param[in] value Значение параметра.
   *  @return true если установлено новое значение, иначе false.
   */
  bool setValue(param_t param, src_t src, uint32_t value);

private:
  /// Значения параметров.
  static paramFields_t params[PARAM_MAX];

  friend bool getBlkComPrm(TParam *params, src_t src, uint32_t &value);
  friend bool getControl(TParam *params, src_t src, uint32_t &value);

  friend bool setBlkComPrmAll(TParam *params, src_t src, uint32_t &value);
  friend bool setBlkComPrm32to01(TParam *params, src_t src, uint32_t &value);
  friend bool setBlkComPrm64to33(TParam *params, src_t src, uint32_t &value);
  friend bool setBtnSA32to01(TParam *params, src_t src, uint32_t &value);
  friend bool setBtnSA64to33(TParam *params, src_t src, uint32_t &value);
  friend bool setControl(TParam *params, src_t src, uint32_t &value);
  friend bool setDirControl(TParam *params, src_t src, uint32_t &value);
  friend bool setVpBtnSAnSbSac(TParam *params, src_t src, uint32_t &value);

  void setLocalValue(param_t param, uint32_t value);
};

} // namespace BVP

#endif // TPARAM_H
