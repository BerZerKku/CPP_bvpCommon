/*
 * bvpCommon.cpp
 *
 *  Created on: Aug 20, 2020
 *      Author: bear
 */

#include "bvpCommon.hpp"

#include <cassert>

namespace BVP {

const uint8_t BvpPkg::c_sop = 0xAB;   // Значение байта "Начало пакета".

// Конструктор.
BvpPkg::BvpPkg() {
  assert(c_sop == 0xAB);
  assert(c_dataLen == 32);
  assert(sizeof(pkgTx.sop) == 1);
  assert(sizeof(pkgTx.sequence) == 2);
  assert(sizeof(pkgTx.data) == c_dataLen);
  assert(sizeof(pkgTx.checksum) == 1);
  assert(sizeof(pkg_t) == 36);
}


// Вычисляет 8-битную контрольную сумму для массива 8-битных данных.
uint8_t BvpPkg::calcChecksumAvant(const uint8_t buf[], uint16_t len) const {
  uint8_t checksum = 0;

  while(len > 0) {
    checksum += (*buf++);
    len--;
  }

  return checksum;
}


// Вычисляет 8-битную контрольную сумму для массива 8-битных данных.
uint8_t BvpPkg::calcChecksumCompl0(const uint8_t buf[], uint16_t len) const {
  uint8_t checksum = 0;

  while(len > 0) {
    checksum += (*buf++);
    len--;
  }
  checksum = 0x00 - checksum;

  return checksum;
}


// Вычисляет 8-битную контрольную сумму для массива 8-битных данных.
uint8_t BvpPkg::calcChecksumHabr(const uint8_t buf[], uint16_t len) const {
  uint16_t checksum = 0xABBA;

  while(len > 0) {
    checksum += (*buf++) * 211;
    checksum = checksum ^ (checksum >> 8);
    len--;
  }

  return (checksum & 0x00FF);
}


// Проверяет принятые данные.
bool BvpPkg::checkRx() const {
  bool check = false;

  check = (pkgRx.sop == c_sop);

  if (check) {
    check = (pkgRx.sequence == pkgTx.sequence);
  }

  if (check) {
    check = isChecksum(pkgRx);
  }

  return check;
}


// Вычисляет контрольную сумму для пакета.
uint8_t BvpPkg::getChecksum(const pkg_t &pkg) const {
  uint16_t len = sizeof(pkg_t) - sizeof(pkg.checksum);
  uint8_t *data = (uint8_t *) &pkg;

  return calcChecksumHabr(data, len);
}


// Возвращает указатель на начало пакета для приема и его размер.
uint8_t* BvpPkg::getRxPkg(uint16_t &len) const {
  len = sizeof(pkgRx);

  return (uint8_t *) &pkgRx;
}


// Возвращает указатель на начало пакета для передачи и его размер.
uint8_t* BvpPkg::getTxPkg(uint16_t &len)  const {
  len = sizeof(pkgTx);

  return (uint8_t *) &pkgTx;
}

// Проверяет контрольную сумму пакета.
bool BvpPkg::isChecksum(const pkg_t &pkg) const {
  return getChecksum(pkg) == pkg.checksum;
}


// Подготовка пакета для передачи.
void BvpPkg::prepareTx() {
  pkgTx.sop = c_sop;
  pkgTx.sequence = sequenceModify(pkgTx.sequence);

  pkgTx.checksum = getChecksum(pkgTx);
}


// Увеличивает счетчик последовательности.
uint16_t BvpPkg::sequenceModify(uint16_t sequence) {
  sequence = sequence + 1;
  if (sequence == 0) {
    sequence = 1;
  }

  return sequence;
}

} /* namespace BVP */
