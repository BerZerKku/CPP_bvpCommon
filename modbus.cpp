#include "modbus.h"

//namespace BVP {

TModbus::TModbus(uint8_t * const buf) : TSerialProtocol(buf) {

}

TModbus::~TModbus()
{

}


bool
TModbus::setEnable(bool enable) {

}

bool
TModbus::isRead() {

}

bool
TModbus::isWrite() {

}

bool
TModbus::push(uint8_t byte) {

}

uint16_t
TModbus::setup(uint16_t baudrate, bool parity, uint8_t stopbits) {
    uint8_t nbites = 8 + stopbits + (parity ? 1 : 0);
    oneByteTimeUs = static_cast<uint16_t> ((1000000UL / baudrate) * nbites);

    return oneByteTimeUs;
}

//
bool TModbus::setupTick(uint16_t ticktimeus) {
    ticksToError = (oneByteTimeUs * 3) / (ticktimeus * 2);
    ticksToComplete = (oneByteTimeUs * 5) / (ticktimeus * 2);

    return (ticksToError > 0) && (ticksToComplete > 0);
}

bool
TModbus::pop(uint8_t &byte) {

}

void
TModbus::tick() {

}


//} // namespace BVP
