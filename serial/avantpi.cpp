#include "avantpi.h"

namespace BVP {


TAvantPi::TAvantPi() {

}

//
bool
BVP::TAvantPi::vWriteAvant() {
  bool ok = false;
  uint32_t value = 0;

  value = mParam->getValue(PARAM_control, mSrc, ok);
  ok = ok && (value > 0);
  if (ok) {
    ok = fillComControl(value);
  }

  if (!ok) {
    setCom(0x32);
    ok = true;
  }

  return ok;
}

//
bool
TAvantPi::vReadAvant() {
  bool ok = false;

  if (mBuf[POS_COM] == 0x32) {

  }

  return ok;
}

//
bool
TAvantPi::fillComControl(uint32_t value) {
  bool ok = false;

  enum comControlBytes_t {
      COM_CONTROL_BYTES_selfReset = 1
  };

  Q_ASSERT(value < CTRL_MAX);
  if (value < CTRL_MAX) {

    switch(value) {
      case CTRL_resetErrors: {
        setCom(COM_AVANT_control);
        addByte(COM_CONTROL_BYTES_selfReset);
        ok = true;
      } break;

      case CTRL_resetIndication: {
        setCom(COM_AVANT_prmResetInd);
        ok = true;
      } break;

    }
  }

  qDebug() << "ok = " << ok;

  return ok;
}

} // namespace BVP
