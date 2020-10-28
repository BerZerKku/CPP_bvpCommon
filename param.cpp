#include "param.h"

namespace BVP {

TParam TParam::mParam;

/**
 * @brief setDirControl
 * @param params
 * @param value
 * @return
 */
bool setDirControl(TParam *params, src_t src, uint32_t &value) {
    bool ok = false;

    switch(src) {
        case SRC_pi: {
            ok = true;
        } break;

        case SRC_vkey: {
            if (value > 0) {
                value = params->getValue(PARAM_dirControl, src, ok);
                if (ok) {
                    dirControl_t dir = DIR_CONTROL_local;
                    switch(static_cast<dirControl_t> (value)) {
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

/**
 * @brief setVpBtnSAnSbSac
 * @param params
 * @param src
 * @param value
 * @return
 */
bool
setVpBtnSAnSbSac(TParam *params, src_t src, uint32_t &value) {
    bool ok = false;

    if (src == SRC_vkey) {
        param_t param = PARAM_vpBtnSAnSbSac;
        if (params->isValueSet(PARAM_vpBtnSAnSbSac)) {
            uint32_t tvalue = value ^ params->getValue(param, src, ok);

            if (tvalue & 0x00000001) {

            }

            if (tvalue & 0x00000002) {
                params->setValue(PARAM_dirControl, src, value & 0x00000002);
            }



            if (tvalue & 0x00000004) {

            }
        }
        ok = true;
    }

    return ok;
}

TParam::paramFields_t TParam::params[PARAM_MAX] = {
    {param : PARAM_dirControl,
     isValue : false, rValue : 0, wValue : 0, set : setDirControl},
    {param : PARAM_blkComPrmAll,
     isValue : false, rValue : 0, wValue : 0, set : nullptr},
    {param : PARAM_blkComPrmDir,
     isValue : false, rValue : 0, wValue : 0, set : nullptr},
    {param : PARAM_blkComPrm32to01,
     isValue : false, rValue : 0, wValue : 0, set : nullptr},
    {param : PARAM_blkComPrm64to33,
     isValue : false, rValue : 0, wValue : 0, set : nullptr},
    {param : PARAM_blkComPrd32to01,
     isValue : false, rValue : 0, wValue : 0, set : nullptr},
    {param : PARAM_blkComPrd64to33,
     isValue : false, rValue : 0, wValue : 0, set : nullptr},
    //
    {param : PARAM_vpBtnSAnSbSac,
     isValue : false, rValue : 0, wValue : 0, set : setVpBtnSAnSbSac},
    {param : PARAM_vpBtnSA32to01,
     isValue : false, rValue : 0, wValue : 0, set : nullptr},
    {param : PARAM_vpBtnSA64to33,
     isValue : false, rValue : 0, wValue : 0, set : nullptr},
};



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
    Q_ASSERT(param < PARAM_MAX);

    // TODO добавить проверку источника доступа!
    ok = isValueSet(param);

    return (param < PARAM_MAX) ? params[param].rValue : 0;
}

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
                if (param == PARAM_dirControl) {
                    qDebug() << "src = " << src << ", value = " << value;
                }

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
