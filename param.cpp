#include "param.h"

namespace BVP {

TParam TParam::mParam;

//
TParam*
TParam::getInstance(){
    return &TParam::mParam;
}

//
bool
TParam::isValueSet(param_t param) const {
    Q_ASSERT(param < PARAM_MAX);

    return (param < PARAM_MAX) ? params[param].isValue : false;
}

uint32_t
TParam::getValue(param_t param) {
    Q_ASSERT(param < PARAM_MAX);

    return (param < PARAM_MAX) ? params[param].value : 0;
}

void
TParam::setValue(param_t param, uint32_t value) {
    Q_ASSERT(param < PARAM_MAX);

    if (param < PARAM_MAX) {
        params[param].value = value;
        params[param].isValue = true;
    }
}

} // namespace BVP
