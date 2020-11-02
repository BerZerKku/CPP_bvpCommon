#include "protocolAvant.h"

namespace BVP {

//
TProtocolAvant::TProtocolAvant() :
  mSrc(SRC_MAX),
  mState(STATE_disable),
  mTimeUs(0),
  mTimeTickUs(0),
  mTimeOneByteUs(0),
  cntLostMessage(kMaxLostMessage) {
}

//
TProtocolAvant::~TProtocolAvant() {

}

//
bool
TProtocolAvant::setEnable(bool enable) {
  Q_ASSERT(mSrc != SRC_MAX);
  Q_ASSERT(mParam != nullptr);
  Q_ASSERT(mBuf != nullptr);
  Q_ASSERT(mSize != 0);

  if (enable) {
    mState = STATE_idle;
    mLen = 0;
    mTimeUs = 0;
  } else {
    mState = STATE_disable;
    cntLostMessage = kMaxLostMessage;
  }

  return mState != STATE_disable;
}

//
bool
TProtocolAvant::read() {
  bool isread = false;

  if (mState == STATE_procReply) {
    bool ok = true;
    uint16_t len = mLen;
    uint16_t position = 0;

    position += checkReadMsg(&mBuf[position], len, ok);

    if (ok) {
      switch(static_cast<com_t> (mBuf[position++])) {
        case COM_readHoldingRegs: {
          position += getReadReg(&mBuf[position], len,
                                 REG_READ_MIN, REG_READ_MAX-1, ok);
        } break;
        case COM_writeMultRegs : {
          // TODO добавить проверку сообщения.
        } break;
        case COM_readWriteRegs: {
          position += getReadReg(&mBuf[position], len,
                                 REG_READ_MIN, REG_READ_MAX-1, ok);
        } break;
      }
      cntLostMessage = 0;
    }

    if (!ok) {
      incLostMessageCounter();
    }

    isread = ok;
    mState = STATE_idle;
  }

  return isread;
}

//
void
TProtocolAvant::readError() {
  if (mState == STATE_procReply) {
    mState = STATE_errorReply;
    mTimeUs = 0;
  }
}

//
bool
TProtocolAvant::write() {
  if (mState == STATE_idle) {
    bool ok = true;
    uint16_t len = 0;
    com_t com = COM_readWriteRegs;

    mBuf[len++] = mNetAddress;
    mBuf[len++] = com;

    switch(com) {
      case COM_readHoldingRegs: {
        len += addReadRegMsg(&mBuf[len],
                             REG_READ_MIN, REG_READ_MAX-1, ok);
      } break;
      case COM_writeMultRegs : {
        len += addReadRegMsg(&mBuf[len],
                             REG_READ_MIN, REG_READ_MAX-1, ok);
      } break;
      case COM_readWriteRegs: {
        len += addReadRegMsg(&mBuf[len],
                             REG_READ_MIN, REG_READ_MAX-1, ok);
        len += addWriteRegMsg(&mBuf[len],
                              REG_WRITE_MIN, REG_WRITE_MAX-1, ok);
      } break;
    }

    if (ok) {
      uint16_t crc =  calcCRC(mBuf, len);
      mBuf[len++] = static_cast<uint8_t> (crc);
      mBuf[len++] = static_cast<uint8_t> (crc >> 8);

      Q_ASSERT(len < kMaxSizeFrameRtu);

      if (len > 0) {
        mLen = len;
        mPos = 0;
        mTimeUs = 0;
        mState = STATE_reqSend;
      }
    }
  }

  return mState == STATE_reqSend;
}

//
uint16_t
TProtocolAvant::pop(uint8_t *data[]) {
  uint16_t len = 0;
  *data = &mBuf[mPos];

  Q_ASSERT(mState == STATE_reqSend);

  if (mState == STATE_reqSend) {
    len = mLen;
    mState = STATE_waitForSend;
  }

  return len;
}

//
void TProtocolAvant::sendFinished() {
  Q_ASSERT(mState == STATE_waitForSend || mState == STATE_disable);

  if (mState == STATE_waitForSend) {
    mPos = 0;
    mLen = 0;
    mTimeUs = 0;
    mState = STATE_waitForReply;
  } else {
    mState = STATE_idle;
  }
}

//
bool
TProtocolAvant::push(uint8_t byte) {
  uint16_t len = mLen;

  if (mState == STATE_waitForReply) {
    if ((len == 0) || ((len < mSize) && (mTimeUs < mTimeToErrorUs))) {
      mBuf[mLen++] = byte;
      mTimeUs = 0;
    } else {
      mState = STATE_errorReply;
    }
  }

  if (mState == STATE_errorReply) {
    mTimeUs = 0;
  }

  return mLen > len;
}

//
bool
TProtocolAvant::setNetAddress(uint16_t address) {
  if ((address > 0) && (address <= 247)) {
    *(const_cast<uint8_t *> (&mNetAddress)) = static_cast<uint8_t> (address);
  }

  return mNetAddress == address;
}

//
bool
TProtocolAvant::setup(uint32_t baudrate, bool parity, uint8_t stopbits) {
  uint8_t nbites = 1 + 8 + stopbits + (parity ? 1 : 0);

  // TODO Проверить времена в железе. На ПК от них толка нет.

  *(const_cast<uint32_t*> (&mTimeOneByteUs)) =
      static_cast<uint32_t> ((1000000UL / baudrate) * nbites);

  return mTimeOneByteUs;
}

//
bool TProtocolAvant::setTimeTick(uint32_t ticktimeus) {
  *(const_cast<uint32_t*> (&mTimeTickUs)) = ticktimeus;

  return mTimeTickUs != 0;
}

//
void
TProtocolAvant::tick() {
  if (mState == STATE_disable) {
    return;
  }

  if (mTimeUs < kMaxTimeToResponseUs) {
    mTimeUs += mTimeTickUs;
  }

  if (mState == STATE_waitForReply) {
    if (mLen == 0) {
      if (mTimeUs >=  kMaxTimeToResponseUs) {
        mState = STATE_idle;
        incLostMessageCounter();
      }
    } else {
      if (mTimeUs > mTimeToCompleteUs) {
        mState = STATE_procReply;
      }
    }
  }

  if (mState == STATE_errorReply) {
    if (mTimeUs >= mTimeToCompleteUs) {
      mState = STATE_idle;
      incLostMessageCounter();
    }
  }
}

//
bool
TProtocolAvant::isConnection() const {
  return cntLostMessage < kMaxLostMessage;
}

//
void
TProtocolAvant::setID(uint32_t id) {
  Q_ASSERT(id < SRC_MAX);

  *(const_cast<src_t*> (&mSrc)) = static_cast<src_t> (id);
}

//
void
TProtocolAvant::incLostMessageCounter() {
  if (cntLostMessage < kMaxLostMessage) {
    cntLostMessage++;
  }
}

uint8_t TProtocolAvant::calcCRC(const uint8_t buf[], size_t len, uint8_t crc) {
  for(size_t i = 2; i < len; i++) {
    crc += buf[i];
  }

  return crc;
}

} // namespace BVP
