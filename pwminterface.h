#ifndef PWMINTERFACE_H
#define PWMINTERFACE_H

#include <stdint.h>

#ifndef DEFAULT_PWM_PIN
    #define DEFAULT_PWM_PIN 3
#endif

#ifndef DEFAULT_PWM_PERIOD_NS
    #define DEFAULT_PWM_PERIOD_NS 1000000
#endif

#ifndef DEFAULT_PWM_DUTY_CYCLE_NS
    #define DEFAULT_PWM_DUTY_CYCLE_NS 900000
#endif

class PWMInterface
{
public:
    virtual ~PWMInterface() = 0;

    // Возможность поменять параметры
    virtual bool set_params(uint32_t duty_cycle_ns, uint32_t period_ns = DEFAULT_PWM_PERIOD_NS) = 0;
    // Запуск ШИМ, сигнал пошел с заданными параметрами
    virtual bool start()  = 0;
    // Запуск ШИМ, сигнал пошел с заданными параметрами
    virtual bool stop()  = 0;
    // Узнать, запущен ли ШИМ сейчас
    virtual bool is_on() const = 0;
    // Текущий пин
    virtual uint16_t pin() const = 0;
    // Текущий период ШИМ, мс
    virtual uint32_t period_ns() const = 0;
    // Текущий коэффициент заполнения ШИМ, мс
    virtual uint32_t duty_cycle_ns() const = 0;

};

#endif // PWMINTERFACE_H
