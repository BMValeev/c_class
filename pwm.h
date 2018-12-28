#ifndef PWM_H
#define PWM_H

#include <stdint.h>

#include "pwminterface.h"

class PWM : public PWMInterface
{
public:
    // В конструктор передаётся номер пина
    PWM(uint16_t pin = DEFAULT_PWM_PIN);

    // Конструктор сразу со всеми параметрами
    PWM(uint16_t pin = DEFAULT_PWM_PIN,
        uint32_t duty_cycle_ms = DEFAULT_PWM_DUTY_CYCLE_MS,
        uint32_t period_ms = DEFAULT_PWM_PERIOD_MS);

    // Деструктор для подчистки всего чего надо, в том числе остановки
    // ШИМ если он запущен
    ~PWM() override;

    // Возможность поменять параметры
    bool set_params(uint32_t duty_cycle_ms,
                    uint32_t period_ms = DEFAULT_PWM_PERIOD_MS) override;

    // Запуск ШИМ, сигнал пошел с заданными параметрами
    bool start() const override;
    // Запуск ШИМ, сигнал пошел с заданными параметрами
    bool stop() const override;
    // Узнать, запущен ли ШИМ сейчас
    bool is_on() const override;
    // Текущий период ШИМ, мс
    uint32_t period_ms() const override;
    // Текущий коэффициент заполнения ШИМ, мс
    uint32_t duty_cycle_ms() const override;

private:
    const uint16_t m_pin = DEFAULT_PWM_PIN;
    uint32_t m_period_ms = DEFAULT_PWM_PERIOD_MS;
    uint32_t m_duty_cycle_ms = DEFAULT_PWM_DUTY_CYCLE_MS;

};

#endif // PWM_H
