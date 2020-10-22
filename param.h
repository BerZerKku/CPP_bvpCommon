#ifndef TPARAM_H
#define TPARAM_H

#include <stdint.h>
#include <QDebug>

namespace BVP {

enum param_t {
    // Кнопки панели виртуальных ключей.
    PARAM_vpBtnSAnSbSac = 0,
    PARAM_vpBtnSA16to01,
    PARAM_vpBtnSA32to17,
    PARAM_vpBtnSA38to33,
    PARAM_vpBtnSA64to49,
    // Состояния виртуальных ключей.
    PARAM_vpEnSanSbSac,
    PARAM_vpEnSa16to01,
    PARAM_vpEnSa32to17,
    PARAM_vpEnSa38to33,
    PARAM_vpEnSa64to49,
    PARAM_vpDsSanSbSac,
    PARAM_vpDsSa16to01,
    PARAM_vpDsSa32to17,
    PARAM_vpDsSa38to33,
    PARAM_vpDsSa64to49,
    //
    PARAM_MAX
};

/** Класс параметров.
 *
 *  Создан на подобие Одиночки (single-tone).конструктор спрятан в protected,
 *  а создание объекта new происходит при первом вызове getInstance).
 *  Но т.к. объект должен существовать всегда, его конструктор открыт и
 *  при вызове getInstance нет необходимости проверки первого запуска.
 *
 *  В каждом классе где используются параметры необходимо взять указатель
 *  на созданный объект (getInstance).
 *
 * Создавать объект где-либо нельзя, он создается сразу!!!
 */
class TParam {
    /// Параметры.
    static TParam mParam;

    struct paramFields_t {
        bool isValue = false;
        uint32_t value = 0;
    };

public:
    TParam() {}
    /// Конструктор копирования запрещен.
    TParam(TParam &other) = delete;
    /// Конструктор присваивания запрещен.
    void operator=(const TParam&) = delete;

    /** Возвращает экземпляр класса параметров.
     *
     *  @return Класс параметров.
     */
    static TParam* getInstance();

    /** Проверяет наличие установленного значения параметра.
     *
     *  @param[in] param Параметр.
     *  @return true если значение было установлено, иначе false.
     */
    bool isValueSet(param_t param) const;

    /** Возвращает значение параметра.
     *
     *  @param[in] param Параметр.
     *  @return значенеи параметра.
     */
    uint32_t getValue(param_t param);

    /** Устанавливает значение параметра.
     *
     *  @param[in] param Параметр.
     *  @param[in] value Значение параметра.
     */
    void setValue(param_t param, uint32_t value);

private:
    /// Значения параметров.
    paramFields_t params[PARAM_MAX];
};

} // namespace BVP

#endif // TPARAM_H
