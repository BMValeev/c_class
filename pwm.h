#ifndef PWM_H
#define PWM_H

#include <stdint.h>

#include "pwminterface.h"

class PWM : public PWMInterface
{
public:

    // Конструктор сразу со всеми параметрами
    PWM(uint16_t pin = DEFAULT_PWM_PIN,
        uint32_t duty_cycle_ns = DEFAULT_PWM_DUTY_CYCLE_NS,
        uint32_t period_ns = DEFAULT_PWM_PERIOD_NS);

    // Деструктор для подчистки всего чего надо, в том числе остановки
    // ШИМ если он запущен
    //~PWM() override;
    ~PWM();
    // Возможность поменять параметры
    bool set_params(uint32_t duty_cycle_ns,
                    uint32_t period_ns = DEFAULT_PWM_PERIOD_NS) override;

    // Запуск ШИМ, сигнал пошел с заданными параметрами
    bool start()  override;
    // Запуск ШИМ, сигнал пошел с заданными параметрами
    bool stop()  override;
    // Узнать, запущен ли ШИМ сейчас
    bool is_on() const override;
    // Текущий пин
    uint16_t pin() const override;
    // Текущий период ШИМ, нс
    uint32_t period_ns() const override;
    // Текущий коэффициент заполнения ШИМ, нс
    uint32_t duty_cycle_ns() const override;

private:
    const uint16_t m_pin = DEFAULT_PWM_PIN;
    uint32_t m_period_ns = DEFAULT_PWM_PERIOD_NS;
    uint32_t m_duty_cycle_ns = DEFAULT_PWM_DUTY_CYCLE_NS;
    bool m_enable=false;
};

#endif // PWM_H
