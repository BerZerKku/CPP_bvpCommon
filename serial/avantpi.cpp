#include "avantpi.h"

namespace BVP {


TAvantPi::TAvantPi() {

}

//
bool
BVP::TAvantPi::vWriteAvant() {
  bool ok = false;
  uint32_t value = 0;

  // Сигналы управления
  value = mParam->getValue(PARAM_control, mSrc, ok);
  ok = ok && (value > 0);
  if (ok) {
    ok = fillComControl(value);
  }

  // Если других команд нет, то команда опроса
  if (!ok) {
    setCom(COM_AVANT_getError);
    ok = true;
  }

  return ok;
}

//
bool
TAvantPi::vReadAvant() {
  bool ok = false;

  if (mBuf[POS_COM] == 0x31) {
    uint16_t pos = POS_DATA;
    uint32_t value = 0;

    if (mBuf[POS_DATA_LEN] >= 20) {
      value = mBuf[pos++];
      value = (value << 8) + mBuf[pos++];
      mParam->setValue(PARAM_defError, mSrc, value);
      value = mBuf[pos++];
      value = (value << 8) + mBuf[pos++];
      mParam->setValue(PARAM_defWarning, mSrc, value);
      value = mBuf[pos++];
      value = (value << 8) + mBuf[pos++];
      mParam->setValue(PARAM_prmError, mSrc, value);
      value = mBuf[pos++];
      value = (value << 8) + mBuf[pos++];
      mParam->setValue(PARAM_prmWarning, mSrc, value);
      value = mBuf[pos++];
      value = (value << 8) + mBuf[pos++];
      mParam->setValue(PARAM_prdError, mSrc, value);
      value = mBuf[pos++];
      value = (value << 8) + mBuf[pos++];
      mParam->setValue(PARAM_prdWarning, mSrc, value);
      value = mBuf[pos++];
      value = (value << 8) + mBuf[pos++];
      mParam->setValue(PARAM_glbError, mSrc, value);
      value = mBuf[pos++];
      value = (value << 8) + mBuf[pos++];
      mParam->setValue(PARAM_glbWarning, mSrc, value);
      pos += 4;
      Q_ASSERT(pos == (POS_DATA + 20));
    }

    if (mBuf[POS_DATA_LEN] >= 28) {
      value = mBuf[pos++];
      value = (value << 8) + mBuf[pos++];
      mParam->setValue(PARAM_defRemoteError, mSrc, value);
      value = mBuf[pos++];
      value = (value << 8) + mBuf[pos++];
      mParam->setValue(PARAM_prmRemoteError, mSrc, value);
      value = mBuf[pos++];
      value = (value << 8) + mBuf[pos++];
      mParam->setValue(PARAM_prdRemoteError, mSrc, value);
      value = mBuf[pos++];
      value = (value << 8) + mBuf[pos++];
      mParam->setValue(PARAM_glbRemoteError, mSrc, value);
      Q_ASSERT(pos == (POS_DATA + 28));
    }
  }

  return ok;
}

//
bool
TAvantPi::fillComControl(uint32_t value) {
  bool ok = false;

  Q_ASSERT(value < CTRL_MAX);
  if (value < CTRL_MAX) {

    switch(static_cast<ctrl_t> (value)) {

      case CTRL_resetErrors: {
        setCom(COM_AVANT_setControl);
        addByte(COM_CONTROL_BYTES_selfReset);
        ok = true;
      } break;

      case CTRL_resetIndication: {
        setCom(COM_AVANT_setPrmResetInd);
        ok = true;
      } break;

      case CTRL_reset: {
        setCom(COM_AVANT_setControl);
        addByte(COM_CONTROL_BYTES_selfReset);
        ok = true;
      } break;

      case CTRL_MAX: break;

    }
  }

  return ok;
}

} // namespace BVP
