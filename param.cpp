#include "global.hpp"
#include "param.h"

namespace BVP {

TParam TParam::mParam;

//
bool
getBlkComPrm(TParam *params, src_t src, uint32_t &value) {
  bool ok = false;

  if (src == SRC_vkey) {
    uint32_t blkall = params->getValue(PARAM_blkComPrmAll, src, ok);

    if ((ok) && (blkall != ON_OFF_off)) {
        value = 0xFFFFFFFF;
    }
  }

  return ok;
}

/** Считывание сигналов управления.
 *
 *  Для SRC_pi возвращается самый приоритетный на данный момент сигнал
 *  управления ctrl_t.
 *  Для SRC_vkey возвращается текущее состояние сигналов управления.
 *
 *  @param[in] params Параметры.
 *  @param[in] src Источник доступа.
 *  @param[in/out] value Значение.
 *  @return true если значение можно считать, иначе false.
 */
bool
getControl(TParam *params, src_t src, uint32_t &value) {
  bool ok = false;

  UNUSED(params);

  switch(src) {
    case SRC_pi: {
      if (value > 0) {
        uint8_t i = 0;
        while((value & (1 << i)) == 0) {
          i++;
        }
        value = i + CTRL_MIN;
      }
      ok = true;
    } break;

    case SRC_vkey: {
      ok = true;
    } break;
    case SRC_pc: break;
    case SRC_acs: break;
    case SRC_MAX: break;
  }

  return ok;
}

//
bool
setBlkComPrmAll(TParam *params, src_t src, uint32_t &value) {
  bool ok = false;

  switch(src) {
    case SRC_pi: {
      ok = true;
    } break;

    case SRC_vkey: {
      if (value > 0) {
        uint32_t dir = params->getValue(PARAM_dirControl, src, ok);
        ok = ok && (dir == DIR_CONTROL_local);
        if (ok) {
          uint32_t v = params->getValue(PARAM_blkComPrmAll, src, ok);
          if (ok) {
            value = (v == ON_OFF_off) ? ON_OFF_on : ON_OFF_off;
          }
        }
      }
    } break;

    case SRC_pc: break;
    case SRC_acs: break;
    case SRC_MAX: break;
  }

  return ok;
}

//
bool
setBlkComPrm32to01(TParam *params, src_t src, uint32_t &value) {
  bool ok = false;

  switch(src) {
    case SRC_pi: {
      ok = true;
    } break;

    case SRC_vkey: {
      if (value > 0) {
        uint32_t dir = params->getValue(PARAM_dirControl, src, ok);
        ok = ok && (dir == DIR_CONTROL_local);

        if (ok) {
          uint32_t blkall = params->getValue(PARAM_blkComPrmAll, src, ok);
          ok = ok && (blkall == ON_OFF_off);
        }

        if (ok) {
          uint32_t v = params->getValue(PARAM_blkComPrm32to01, src, ok);
          if (ok) {
            value = v ^ value;
          }
        }
      }
    } break;

    case SRC_pc: break;
    case SRC_acs: break;
    case SRC_MAX: break;
  }

  return ok;
}

//
bool
setBlkComPrm64to33(TParam *params, src_t src, uint32_t &value) {
  bool ok = false;

  switch(src) {
    case SRC_pi: {
      ok = true;
    } break;

    case SRC_vkey: {
      if (value > 0) {
        uint32_t dir = params->getValue(PARAM_dirControl, src, ok);
        ok = ok && (dir == DIR_CONTROL_local);

        if (ok) {
          uint32_t blkall = params->getValue(PARAM_blkComPrmAll, src, ok);
          ok = ok && (blkall == ON_OFF_off);
        }

        if (ok) {
          uint32_t v = params->getValue(PARAM_blkComPrm64to33, src, ok);
          if (ok) {
            value = v ^ value;
          }
        }
      }
    } break;

    case SRC_pc: break;
    case SRC_acs: break;
    case SRC_MAX: break;
  }

  return ok;
}

//
bool
setBtnSA32to01(TParam *params, src_t src, uint32_t &value) {
  bool ok = false;

  if (src == SRC_vkey) {
    param_t param = PARAM_vpBtnSA32to01;
    if (params->isValueSet(param)) {
      uint32_t tvalue = value ^ params->getValue(param, src, ok);

      if (tvalue > 0) {
        params->setValue(PARAM_blkComPrm32to01, src, value);
      }
    }
    ok = true;
  }

  return ok;
}

//
bool
setBtnSA64to33(TParam *params, src_t src, uint32_t &value) {
  bool ok = false;


  if (src == SRC_vkey) {
    param_t param = PARAM_vpBtnSA64to33;
    if (params->isValueSet(param)) {
      uint32_t tvalue = value ^ params->getValue(param, src, ok);

      if (tvalue > 0) {
        params->setValue(PARAM_blkComPrm64to33, src, value);
      }
    }

    ok = true;
  }

  return ok;
}

/** Установка сигналов управления.
 *
 *  Запись с SRC_pi означает сброс бит по маске. Если это первая установка
 *  данного параметра, в него будет записан 0.
 *  Запись с SRC_vkey значения не 0 означает нажатие на кнопку сброса индикации,
 *  т.е. сброс индикации команд, неисправностей, предупреждений и их реле.
 *
 *  @param[in] params Параметры.
 *  @param[in] src Источник доступа.
 *  @param[in/out] value Значение.
 *  @return true если значение надо записать, иначе false.
 */
bool setControl(TParam *params, src_t src, uint32_t &value) {
  bool ok = false;
  param_t param = PARAM_vpBtnSA64to33;

  switch(src) {
    case SRC_pi: {
      if (params->isValueSet(param)) {
        uint32_t v = params->getValue(param, src, ok);

        Q_ASSERT(ok);
        if (ok) {
          value = v & ~value;
        }
      } else {
        value = 0;
        ok = true;
      }
    } break;

    case SRC_vkey: {
      if (value > 0) {
        uint32_t dir = params->getValue(PARAM_dirControl, src, ok);
        ok = ok && (dir == DIR_CONTROL_local);

        if (ok) {
          uint32_t v = params->getValue(param, src, ok);

          Q_ASSERT(ok);
          if (ok) {
            value = v | (1 << CTRL_resetIndication);
//            value = value | (1 << CTRL_resetErrors);
            // TODO добавить сброс неисправностей.
          }
        }
      }
    } break;

    case SRC_pc: break;
    case SRC_acs: break;
    case SRC_MAX: break;
  }

  return ok;
}

//
bool setDirControl(TParam *params, src_t src, uint32_t &value) {
  bool ok = false;

  switch(src) {
    case SRC_pi: {
      ok = true;
    } break;

    case SRC_vkey: {
      if (value > 0) {
        uint32_t d = params->getValue(PARAM_dirControl, src, ok);
        if (ok) {
          dirControl_t dir = DIR_CONTROL_local;
          switch(static_cast<dirControl_t> (d)) {
            case DIR_CONTROL_remote: {
              dir = DIR_CONTROL_local;
            } break;
            case DIR_CONTROL_local: {
              dir = DIR_CONTROL_remote;
            } break;
            case DIR_CONTROL_MAX: break;
          }
          value = dir;
        }
      }
    } break;

    case SRC_pc: break;
    case SRC_acs: break;
    case SRC_MAX: break;
  }

  return ok;
}

//
bool
setVpBtnSAnSbSac(TParam *params, src_t src, uint32_t &value) {
  bool ok = false;

  if (src == SRC_vkey) {
    param_t param = PARAM_vpBtnSAnSbSac;
    if (params->isValueSet(param)) {
      uint32_t tvalue = value ^ params->getValue(param, src, ok);

      if (tvalue > 0) {
        if (tvalue & TParam::VP_BTN_CONTROL_sac1) {
          params->setValue(PARAM_blkComPrmAll, src,
                           value & TParam::VP_BTN_CONTROL_sac1);
        }

        if (tvalue & TParam::VP_BTN_CONTROL_sac2) {
          params->setValue(PARAM_dirControl, src,
                           value & TParam::VP_BTN_CONTROL_sac2);
        }

        if (tvalue & TParam::VP_BTN_CONTROL_sb) {
          params->setValue(PARAM_control, src,
                           value & TParam::VP_BTN_CONTROL_sb);
        }

        if (tvalue & TParam::VP_BTN_CONTROL_san) {
          static_assert(TParam::VP_BTN_CONTROL_san == 0x0000FF00,
                        "Wrong position buttons SAnn");
          params->setValue(PARAM_blkComPrmDir, src,
                           (value & TParam::VP_BTN_CONTROL_san) >> 8);
        }
      }
    }
    ok = true;
  }

  return ok;
}

//
TParam::paramFields_t TParam::params[PARAM_MAX] = {
  //
  {param : PARAM_control,
   isValue : false, rValue : 0, wValue : 0,
   set : setControl, get : getControl},
  //
  // Параметры панели виртуальных ключей
  //
  {param : PARAM_dirControl,
   isValue : false, rValue : 0, wValue : 0,
   set : setDirControl, get : nullptr},
  //
  {param : PARAM_blkComPrmAll,
   isValue : false, rValue : 0, wValue : 0,
   set : setBlkComPrmAll, get : nullptr},
  //
  {param : PARAM_blkComPrmDir,
   isValue : false, rValue : 0, wValue : 0,
   set : nullptr, get : nullptr},
  //
  {param : PARAM_blkComPrm32to01,
   isValue : false, rValue : 0, wValue : 0,
   set : setBlkComPrm32to01, get : getBlkComPrm},
  //
  {param : PARAM_blkComPrm64to33,
   isValue : false, rValue : 0, wValue : 0,
   set : setBlkComPrm64to33, get : getBlkComPrm},
  //
  {param : PARAM_blkComPrd32to01,
   isValue : false, rValue : 0, wValue : 0,
   set : nullptr, get : nullptr},
  //
  {param : PARAM_blkComPrd64to33,
   isValue : false, rValue : 0, wValue : 0,
   set : nullptr, get : nullptr},
  //
  // Кнопки панели виртуальных ключей.
  //
  {param : PARAM_vpBtnSAnSbSac,
   isValue : false, rValue : 0, wValue : 0,
   set : setVpBtnSAnSbSac, get : nullptr},
  //
  {param : PARAM_vpBtnSA32to01,
   isValue : false, rValue : 0, wValue : 0,
   set : setBtnSA32to01, get : nullptr},
  //
  {param : PARAM_vpBtnSA64to33,
   isValue : false, rValue : 0, wValue : 0,
   set : setBtnSA64to33, get : nullptr},
};

//
TParam::TParam() {
  // TODO Подумать над инициализацией параметров которые не надо считывать.
  setValue(PARAM_control, SRC_pi, 0);
}

//
TParam*
TParam::getInstance() {
  return &TParam::mParam;
}

//
bool
TParam::isValueSet(param_t param) const {
  Q_ASSERT(param < PARAM_MAX);

  return (param < PARAM_MAX) ? params[param].isValue : false;
}

//
bool
TParam::isAccessRead(param_t param, src_t src) const {
  // TODO Сделать проверку доступа на чтение.
  return true;
}

//
bool
TParam::isAccessSet(param_t param, src_t src) const {
  // TODO Сделать првоерку доступа на запись.
  return true;
}

uint32_t
TParam::getValue(param_t param, src_t src, bool &ok) {
  uint32_t value = 0;

  Q_ASSERT(param < PARAM_MAX);

  // TODO добавить проверку источника доступа!
  ok = isAccessRead(param, src) && isValueSet(param);
  if (ok) {
    value = params[param].rValue;

    if (params[param].get != nullptr) {
      ok = params[param].get(this, src, value);
    }
  }

  return value;
}

//
bool
TParam::setValue(param_t param, src_t src, uint32_t value) {
  bool ok = false;

  Q_ASSERT(param < PARAM_MAX);
  if (param < PARAM_MAX) {
    if (isAccessSet(param, src)) {
      ok = true;

      if (params[param].set != nullptr) {
        ok = params[param].set(this, src, value);
      }

      if (ok) {
        setLocalValue(param, value);
      }
    }

  }

  return ok;
}

//
void
TParam::setLocalValue(param_t param, uint32_t value) {
  params[param].wValue = value;
  params[param].rValue = value; // FIXME rValue должно идти из БСП!
  params[param].isValue = true;
}

} // namespace BVP
