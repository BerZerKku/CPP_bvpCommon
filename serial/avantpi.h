#ifndef AVANTPI_H
#define AVANTPI_H

#include "protocolAvant.h"

namespace BVP {

class TAvantPi : public TProtocolAvant {

public:
  TAvantPi();

  bool vWriteAvant() override;
  bool vReadAvant() override;

private:
  /** Формирование команды управления.
   *
   *  @return true если команда сформирована, иначе false.
   */
  bool fillComControl(uint32_t value);
};

} // namespace BVP

#endif // AVANTPI_H
