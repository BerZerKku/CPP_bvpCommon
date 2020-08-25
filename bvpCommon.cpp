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
BvpPkg::BvpPkg() : mode(MODE_slave)
{
  assert(c_sop == 0xAB);
  assert(c_dataLen == 32);
  assert(sizeof(pkgTx.sop) == 1);
  assert(sizeof(pkgTx.sequence) == 2);
  assert(sizeof(pkgTx.data) == c_dataLen);
  assert(sizeof(pkgTx.checksum) == 1);
  assert(sizeof(pkg_t) == 36);
  assert(MODE_slave == mode);

  pkgRx.sequence = pkgTx.sequence - 1;
}

// ���������� ������ ��� ��������.
bool BvpPkg::addDataToPkg(uint8_t data[], uint16_t len){
  uint16_t i = 0;
  bool state = (len <= c_dataLen);

  if (len <= c_dataLen) {
    pkgTx.sop = c_sop;

    pkgTx.sequence = sequenceTxGet();

    while(i < len) {
      pkgTx.data[i] = data[i];
      i++;
    }

    while(i < c_dataLen) {
      pkgTx.data[i] = 0;
      i++;
    }

    pkgTx.checksum = getChecksum(pkgTx);
  }

  return state;
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
    check = sequenceRxCheck(pkgRx.sequence);
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

//
bool BvpPkg::getDataFromPkg(uint8_t data[], uint16_t &len) const {
  uint16_t i = 0;
  bool state = false;

  if (checkRx() && (len < c_dataLen)) {
    while((i < len) && (i < c_dataLen)) {
      data[i] = pkgRx.data[i];
      i++;
    }
    state = true;
  }

  len = i;
  return state;
}

// ���������� ��������� �� ������ ������ ��� ������ � ��� ������.
uint8_t* BvpPkg::getRxPkg(uint16_t &size) const {
  size = sizeof(pkgRx);

  return (uint8_t *) &pkgRx;
}

// ���������� ��������� �� ������ ������ ��� �������� � ��� ������.
uint8_t* BvpPkg::getTxPkg(uint16_t &size)  const {
  size = sizeof(pkgTx);

  return (uint8_t *) &pkgTx;
}

// ��������� ����������� ����� ������.
bool BvpPkg::isChecksum(const pkg_t &pkg) const {
  return getChecksum(pkg) == pkg.checksum;
}

// ����������� ������� ������������������.
uint16_t BvpPkg::sequenceModify(uint16_t sequence) const {
  sequence = sequence + 1;
  if (sequence == 0) {
    sequence = 1;
  }

  return sequence;
}

// ��������� ����� ������������������ ��� ��������� ������.
bool BvpPkg::sequenceRxCheck(uint16_t sequence) const {
  bool check = false;

  switch(mode) {
    case MODE_slave:
      check = true;
      break;

    case MODE_master:
      check = (sequence == pkgTx.sequence);
      break;
  }

  return check;
}

// ���������� ����� ������������������ ��� ������������� ������.
uint16_t BvpPkg::sequenceTxGet() const {
  uint16_t sequence = 0;

  switch(mode) {
    case MODE_slave:
      sequence = pkgRx.sequence;
      break;

    case MODE_master:
      sequence = sequenceModify(pkgTx.sequence);
      break;
  }

  return sequence;
}

// ��������� ������ ������ "�������".
void BvpPkg::setMaster() {
  mode = MODE_master;
}

// ��������� ������ ������ "�������".
void BvpPkg::setSlave() {
  mode = MODE_slave;
}

} /* namespace BVP */
