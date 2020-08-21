#include "gtest/gtest.h"


#include <cstdio>
#include <iostream>

#define TEST_FRIENDS \
    FRIEND_TEST(BvpPkg_Test, calcChecksumAvant); \
    FRIEND_TEST(BvpPkg_Test, calcChecksumCompl0); \
    FRIEND_TEST(BvpPkg_Test, calcChecksumHabr); \
    FRIEND_TEST(BvpPkg_Test, checkRx); \
    FRIEND_TEST(BvpPkg_Test, isChecksum); \
    FRIEND_TEST(BvpPkg_Test, getChecksum); \
    FRIEND_TEST(BvpPkg_Test, getRxPkg); \
    FRIEND_TEST(BvpPkg_Test, getTxPkg); \
    FRIEND_TEST(BvpPkg_Test, prepareTx); \
    FRIEND_TEST(BvpPkg_Test, sequenceModify);

#include "../bvpCommon.hpp"

using namespace std;

namespace BVP {


class BvpPkg_Test: public ::testing::Test {

public:
  BvpPkg *bvpPkg;

protected:

  void SetUp() override {
    bvpPkg = new BVP::BvpPkg();
  }

  void TearDown()override {
    delete bvpPkg;
  }
};


//
TEST_F(BvpPkg_Test, calcChecksumAvant) {
  {
    uint8_t buf[] = {};
    ASSERT_EQ(0x00, bvpPkg->calcChecksumAvant(buf, sizeof(buf)));
  }

  {
    uint8_t buf[] = {0x00};
    ASSERT_EQ(0x00, bvpPkg->calcChecksumAvant(buf, sizeof(buf)));
  }

  {
    uint8_t buf[] = {0x01};
    ASSERT_EQ(0x01, bvpPkg->calcChecksumAvant(buf, sizeof(buf)));
  }

  {
    uint8_t buf[] = {0x02};
    ASSERT_EQ(0x02, bvpPkg->calcChecksumAvant(buf, sizeof(buf)));
  }

  {
    uint8_t buf[] = {0xCA};
    ASSERT_EQ(0xCA, bvpPkg->calcChecksumAvant(buf, sizeof(buf)));
  }

  {
    uint8_t buf[] = {0x00, 0x00};
    ASSERT_EQ(0x00, bvpPkg->calcChecksumAvant(buf, sizeof(buf)));
  }

  {
    uint8_t buf[] = {0xF6, 0x0B};
    ASSERT_EQ(0x01, bvpPkg->calcChecksumAvant(buf, sizeof(buf)));
  }

  {
    uint8_t buf[] = {0x00, 0x00, 0x00};
    ASSERT_EQ(0x00, bvpPkg->calcChecksumAvant(buf, sizeof(buf)));
  }

  {
    uint8_t buf[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    ASSERT_EQ(0x24, bvpPkg->calcChecksumAvant(buf, sizeof(buf)));
  }

  {
    uint8_t buf[] = {0xFF, 0xFE, 0xFD, 0xFC, 0x1F, 0x2E, 0x3C, 0x4D};
    ASSERT_EQ(0xCC, bvpPkg->calcChecksumAvant(buf, sizeof(buf)));
  }
}

//
TEST_F(BvpPkg_Test, calcChecksumCompl0) {
  {
    uint8_t buf[] = {};
    ASSERT_EQ(0x00, bvpPkg->calcChecksumCompl0(buf, sizeof(buf)));
  }

  {
    uint8_t buf[] = {0x00};
    ASSERT_EQ(0x00, bvpPkg->calcChecksumCompl0(buf, sizeof(buf)));
  }

  {
    uint8_t buf[] = {0x01};
    ASSERT_EQ(0xFF, bvpPkg->calcChecksumCompl0(buf, sizeof(buf)));
  }

  {
    uint8_t buf[] = {0x02};
    ASSERT_EQ(0xFE, bvpPkg->calcChecksumCompl0(buf, sizeof(buf)));
  }

  {
    uint8_t buf[] = {0xCA};
    ASSERT_EQ(0x36, bvpPkg->calcChecksumCompl0(buf, sizeof(buf)));
  }

  {
    uint8_t buf[] = {0x00, 0x00};
    ASSERT_EQ(0x00, bvpPkg->calcChecksumCompl0(buf, sizeof(buf)));
  }

  {
    uint8_t buf[] = {0xF6, 0x0B};
    ASSERT_EQ(0xFF, bvpPkg->calcChecksumCompl0(buf, sizeof(buf)));
  }

  {
    uint8_t buf[] = {0x00, 0x00, 0x00};
    ASSERT_EQ(0x00, bvpPkg->calcChecksumCompl0(buf, sizeof(buf)));
  }

  {
    uint8_t buf[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    ASSERT_EQ(0xDC, bvpPkg->calcChecksumCompl0(buf, sizeof(buf)));
  }

  {
    uint8_t buf[] = {0xFF, 0xFE, 0xFD, 0xFC, 0x1F, 0x2E, 0x3C, 0x4D};
    ASSERT_EQ(0xCC, bvpPkg->calcChecksumAvant(buf, sizeof(buf)));
  }
}

//
TEST_F(BvpPkg_Test, calcChecksumHabr) {

  {
    uint8_t buf[] = {};
    ASSERT_EQ(0xBA, bvpPkg->calcChecksumHabr(buf, sizeof(buf)));
  }

  {
    uint8_t buf[] = {0x00};
    ASSERT_EQ(0x11, bvpPkg->calcChecksumHabr(buf, sizeof(buf)));
  }

  {
    uint8_t buf[] = {0x01};
    ASSERT_EQ(0x21, bvpPkg->calcChecksumHabr(buf, sizeof(buf)));
  }

  {
    uint8_t buf[] = {0x02};
    ASSERT_EQ(0xCD, bvpPkg->calcChecksumHabr(buf, sizeof(buf)));
  }

  {
    uint8_t buf[] = {0xCA};
    ASSERT_EQ(0x6A, bvpPkg->calcChecksumHabr(buf, sizeof(buf)));
  }

  {
    uint8_t buf[] = {0x00, 0x00};
    ASSERT_EQ(0xBA, bvpPkg->calcChecksumHabr(buf, sizeof(buf)));
  }

  {
    uint8_t buf[] = {0xF6, 0x0B};
    ASSERT_EQ(0x01, bvpPkg->calcChecksumAvant(buf, sizeof(buf)));
  }

  {
    uint8_t buf[] = {0x00, 0x00, 0x00};
    ASSERT_EQ(0x11, bvpPkg->calcChecksumHabr(buf, sizeof(buf)));
  }

  {
    uint8_t buf[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    ASSERT_EQ(0xAE, bvpPkg->calcChecksumHabr(buf, sizeof(buf)));
  }

  {
    uint8_t buf[] = {0xFF, 0xFE, 0xFD, 0xFC, 0x1F, 0x2E, 0x3C, 0x4D};
    ASSERT_EQ(0xED, bvpPkg->calcChecksumHabr(buf, sizeof(buf)));
  }
}

//
TEST_F(BvpPkg_Test, checkRx) {
  uint16_t len = 0;
  BvpPkg::pkg_t *pkgTx = (BvpPkg::pkg_t *) bvpPkg->getTxPkg(len);
  BvpPkg::pkg_t *pkgRx = (BvpPkg::pkg_t *) bvpPkg->getRxPkg(len);

  ASSERT_FALSE(bvpPkg->checkRx());

  pkgRx->sop = bvpPkg->c_sop;
  ASSERT_FALSE(bvpPkg->checkRx());

  pkgRx->sequence = pkgTx->sequence;
  ASSERT_FALSE(bvpPkg->checkRx());

  pkgRx->checksum = bvpPkg->getChecksum(*pkgRx);
  ASSERT_TRUE(bvpPkg->checkRx());

  pkgTx->sequence += 1;
  ASSERT_FALSE(bvpPkg->checkRx());

  pkgRx->sequence = pkgTx->sequence;
  ASSERT_FALSE(bvpPkg->checkRx());
  pkgRx->checksum = bvpPkg->getChecksum(*pkgRx);
  ASSERT_TRUE(bvpPkg->checkRx());

  pkgRx->sop -= 1;
  ASSERT_FALSE(bvpPkg->checkRx());
  pkgRx->checksum = bvpPkg->getChecksum(*pkgRx);
  ASSERT_FALSE(bvpPkg->checkRx());
  pkgRx->sop = bvpPkg->c_sop;
  pkgRx->checksum = bvpPkg->getChecksum(*pkgRx);
  ASSERT_TRUE(bvpPkg->checkRx());

  pkgRx->data[0] += 1;
  ASSERT_FALSE(bvpPkg->checkRx());
  pkgRx->checksum = bvpPkg->getChecksum(*pkgRx);
  ASSERT_TRUE(bvpPkg->checkRx());

  pkgRx->data[bvpPkg->c_dataLen - 1] += 1;
  ASSERT_FALSE(bvpPkg->checkRx());
  pkgRx->checksum = bvpPkg->getChecksum(*pkgRx);
  ASSERT_TRUE(bvpPkg->checkRx());
}

//
TEST_F(BvpPkg_Test, getChecksum) {
  BvpPkg::pkg_t pkg = { 0 };
  uint8_t *buf = reinterpret_cast<uint8_t *> (&pkg);
  uint16_t size = sizeof(pkg) - sizeof(pkg.checksum);

  ASSERT_EQ(bvpPkg->calcChecksumHabr(buf, size), bvpPkg->getChecksum(pkg));

  pkg.sop = bvpPkg->c_sop;
  ASSERT_EQ(bvpPkg->calcChecksumHabr(buf, size), bvpPkg->getChecksum(pkg));

  pkg.sequence = 0x1234;
  ASSERT_EQ(bvpPkg->calcChecksumHabr(buf, size), bvpPkg->getChecksum(pkg));

  for(uint16_t i = 0; i < sizeof(pkg.data); i++) {
    pkg.data[i] = 0x31*i;
  }
  ASSERT_EQ(bvpPkg->calcChecksumHabr(buf, size), bvpPkg->getChecksum(pkg));
}

//
TEST_F(BvpPkg_Test, isChecksum) {
  BvpPkg::pkg_t pkg = { 0 };

  ASSERT_FALSE(bvpPkg->isChecksum(pkg));
  pkg.checksum = bvpPkg->getChecksum(pkg);
  ASSERT_TRUE(bvpPkg->isChecksum(pkg));

  pkg.sop = bvpPkg->c_sop;
  ASSERT_FALSE(bvpPkg->isChecksum(pkg));
  pkg.checksum = bvpPkg->getChecksum(pkg);
  ASSERT_TRUE(bvpPkg->isChecksum(pkg));

  pkg.sequence = 0xC1D1;
  ASSERT_FALSE(bvpPkg->isChecksum(pkg));
  pkg.checksum = bvpPkg->getChecksum(pkg);
  ASSERT_TRUE(bvpPkg->isChecksum(pkg));

  for(uint16_t i = 0; i < sizeof(pkg.data); i++) {
    pkg.data[i] = 0x31*i;
  }
  ASSERT_FALSE(bvpPkg->isChecksum(pkg));
  pkg.checksum = bvpPkg->getChecksum(pkg);
  ASSERT_TRUE(bvpPkg->isChecksum(pkg));
}

//
TEST_F(BvpPkg_Test, getRxPkg) {
  uint16_t len = 0;
  uint8_t *buf = nullptr;

  buf = bvpPkg->getRxPkg(len);
  ASSERT_EQ(sizeof(bvpPkg->pkgRx), len);
  ASSERT_EQ(buf, reinterpret_cast<uint8_t *> (&bvpPkg->pkgRx));
}

//
TEST_F(BvpPkg_Test, getTxPkg) {
  uint16_t len = 0;
  uint8_t *buf = nullptr;

  buf = bvpPkg->getTxPkg(len);
  ASSERT_EQ(sizeof(bvpPkg->pkgTx), len);
  ASSERT_EQ(buf, reinterpret_cast<uint8_t *> (&bvpPkg->pkgTx));
}

//
TEST_F(BvpPkg_Test, prepareTx) {
  uint16_t size = bvpPkg->c_dataLen;
  uint8_t data[size] = {0};

  // Состояние после инициализации
  ASSERT_EQ(0, bvpPkg->pkgTx.sop);
  ASSERT_EQ(0, bvpPkg->pkgTx.sequence);
  ASSERT_EQ(0, memcmp(bvpPkg->pkgTx.data, data, sizeof(data)));
  ASSERT_EQ(0, bvpPkg->pkgTx.checksum);

  // Данные в процессе не должны измениться
  for(uint8_t i = 0; i < size; i++) {
    data[i] = '1' + i%10;
    bvpPkg->pkgTx.data[i] = data[i];
  }

  // Формирование пакета
  bvpPkg->prepareTx();
  ASSERT_EQ(bvpPkg->c_sop, bvpPkg->pkgTx.sop);
  ASSERT_EQ(1, bvpPkg->pkgTx.sequence);
  ASSERT_EQ(0, memcmp(bvpPkg->pkgTx.data, data, size));
  ASSERT_EQ(bvpPkg->getChecksum(bvpPkg->pkgTx), bvpPkg->pkgTx.checksum);

  // Формирование пакета
  bvpPkg->prepareTx();
  ASSERT_EQ(bvpPkg->c_sop, bvpPkg->pkgTx.sop);
  ASSERT_EQ(2, bvpPkg->pkgTx.sequence);
  ASSERT_EQ(0, memcmp(bvpPkg->pkgTx.data, data, size));
  ASSERT_EQ(bvpPkg->getChecksum(bvpPkg->pkgTx), bvpPkg->pkgTx.checksum);

  // Формирование пакета с переполнением номера последовательности.
  bvpPkg->pkgTx.sequence = 65535;
  bvpPkg->prepareTx();
  ASSERT_EQ(bvpPkg->c_sop, bvpPkg->pkgTx.sop);
  ASSERT_EQ(1, bvpPkg->pkgTx.sequence);
  ASSERT_EQ(0, memcmp(bvpPkg->pkgTx.data, data, size));
  ASSERT_EQ(bvpPkg->getChecksum(bvpPkg->pkgTx), bvpPkg->pkgTx.checksum);


}

//
TEST_F(BvpPkg_Test, sequenceModify) {
  uint16_t sequence = 0;

  sequence = 0;
  ASSERT_EQ(1, bvpPkg->sequenceModify(sequence));

  sequence = 0;
  ASSERT_EQ(sequence + 1, bvpPkg->sequenceModify(sequence));

  sequence = 254;
  ASSERT_EQ(sequence + 1, bvpPkg->sequenceModify(sequence));

  sequence = 255;
  ASSERT_EQ(sequence + 1, bvpPkg->sequenceModify(sequence));

  sequence = 65534;
  ASSERT_EQ(sequence + 1, bvpPkg->sequenceModify(sequence));

  sequence = 65535;
  ASSERT_EQ(1, bvpPkg->sequenceModify(sequence));
}

}
