#ifndef AVANT_PI_H
#define AVANT_PI_H

#include "protocolAvant.h"

namespace BVP {

class TAvantPi : public TProtocolAvant {

  /// Значение байта данных для команды COM_AVANT_control "Управление"
  enum comControlBytes_t {
      COM_CONTROL_BYTES_selfReset = 1 //<
  };

public:
  TAvantPi(regime_t regime);

  bool vWriteAvant() override;
  bool vReadAvant() override;

private:
  /** Формирование команды управления.
   *
   *  @return true если команда сформирована, иначе false.
   */
  bool fillComControl(uint32_t value);

  bool comGetError(); ///< Обработчик команды чтения неисправностей и предупр.
};

} // namespace BVP

#endif // AVANT_PI_H
