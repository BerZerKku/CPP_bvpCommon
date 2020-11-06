#ifndef AVANTPI_H
#define AVANTPI_H

#include "protocolAvant.h"

namespace BVP {

class TAvantPi : public TProtocolAvant {

  /// Значение байта данных для команды COM_AVANT_control "Управление"
  enum comControlBytes_t {
      COM_CONTROL_BYTES_selfReset = 1 //<
  };

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
