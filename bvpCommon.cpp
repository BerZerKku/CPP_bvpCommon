/*
 * bvpCommon.cpp
 *
 *  Created on: Aug 20, 2020
 *      Author: bear
 */

#include "bvpCommon.hpp"

#include <cassert>

namespace BVP {

const uint8_t BvpPkg::c_sop = 0xAB;   // �������� ����� "������ ������".

// �����������.
BvpPkg::BvpPkg() {
  assert(c_sop == 0xAB);
  assert(c_dataLen == 32);
  assert(sizeof(pkgTx.sop) == 1);
  assert(sizeof(pkgTx.sequence) == 2);
  assert(sizeof(pkgTx.data) == c_dataLen);
  assert(sizeof(pkgTx.checksum) == 1);
  assert(sizeof(pkg_t) == 36);
}


// ��������� 8-������ ����������� ����� ��� ������� 8-������ ������.
uint8_t BvpPkg::calcChecksumAvant(const uint8_t buf[], uint16_t len) const {
  uint8_t checksum = 0;

  while(len > 0) {
    checksum += (*buf++);
    len--;
  }

  return checksum;
}


// ��������� 8-������ ����������� ����� ��� ������� 8-������ ������.
uint8_t BvpPkg::calcChecksumCompl0(const uint8_t buf[], uint16_t len) const {
  uint8_t checksum = 0;

  while(len > 0) {
    checksum += (*buf++);
    len--;
  }
  checksum = 0x00 - checksum;

  return checksum;
}


// ��������� 8-������ ����������� ����� ��� ������� 8-������ ������.
uint8_t BvpPkg::calcChecksumHabr(const uint8_t buf[], uint16_t len) const {
  uint16_t checksum = 0xABBA;

  while(len > 0) {
    checksum += (*buf++) * 211;
    checksum = checksum ^ (checksum >> 8);
    len--;
  }

  return (checksum & 0x00FF);
}


// ��������� �������� ������.
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


// ��������� ����������� ����� ��� ������.
uint8_t BvpPkg::getChecksum(const pkg_t &pkg) const {
  uint16_t len = sizeof(pkg_t) - sizeof(pkg.checksum);
  uint8_t *data = (uint8_t *) &pkg;

  return calcChecksumHabr(data, len);
}


// ���������� ��������� �� ������ ������ ��� ������ � ��� ������.
uint8_t* BvpPkg::getRxPkg(uint16_t &len) const {
  len = sizeof(pkgRx);

  return (uint8_t *) &pkgRx;
}


// ���������� ��������� �� ������ ������ ��� �������� � ��� ������.
uint8_t* BvpPkg::getTxPkg(uint16_t &len)  const {
  len = sizeof(pkgTx);

  return (uint8_t *) &pkgTx;
}

// ��������� ����������� ����� ������.
bool BvpPkg::isChecksum(const pkg_t &pkg) const {
  return getChecksum(pkg) == pkg.checksum;
}


// ���������� ������ ��� ��������.
void BvpPkg::prepareTx() {
  pkgTx.sop = c_sop;
  pkgTx.sequence = sequenceModify(pkgTx.sequence);

  pkgTx.checksum = getChecksum(pkgTx);
}


// ����������� ������� ������������������.
uint16_t BvpPkg::sequenceModify(uint16_t sequence) {
  sequence = sequence + 1;
  if (sequence == 0) {
    sequence = 1;
  }

  return sequence;
}

} /* namespace BVP */
