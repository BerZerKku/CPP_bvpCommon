#include "serialprotocol.h"

namespace BVP {

//
TSerialProtocol::TSerialProtocol() :
    mBuf(nullptr),
    mSize(0),
    mPos(0),
    mLen(0)
{

}

//
TSerialProtocol::~TSerialProtocol() {
    qDebug() << "~TSerialProtocol()";
}

//
void
TSerialProtocol::setBuffer(uint8_t buf[], uint16_t size) {
    *(const_cast<uint16_t*> (&mSize)) = size;
    *(const_cast<uint8_t**> (&mBuf)) = buf;
}

} // namespace BVP
