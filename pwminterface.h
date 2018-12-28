#ifndef PWMINTERFACE_H
#define PWMINTERFACE_H

#include <stdint.h>

#ifndef DEFAULT_PWM_PIN
    #define DEFAULT_PWM_PIN 0
#endif

#ifndef DEFAULT_PWM_PERIOD_MS
    #define DEFAULT_PWM_PERIOD_MS 10
#endif

#ifndef DEFAULT_PWM_DUTY_CYCLE_MS
    #define DEFAULT_PWM_DUTY_CYCLE_MS 0
#endif

class PWMInterface
{
public:
    virtual ~PWMInterface();

    // Возможность поменять параметры
    virtual bool set_params(uint32_t duty_cycle_ms, uint32_t period_ms = DEFAULT_PWM_PERIOD_MS) = 0;
    // Запуск ШИМ, сигнал пошел с заданными параметрами
    virtual bool start() const = 0;
    // Запуск ШИМ, сигнал пошел с заданными параметрами
    virtual bool stop() const = 0;
    // Узнать, запущен ли ШИМ сейчас
    virtual bool is_on() const = 0;
    // Текущий период ШИМ, мс
    virtual uint32_t period_ms() const = 0;
    // Текущий коэффициент заполнения ШИМ, мс
    virtual uint32_t duty_cycle_ms() const = 0;

};

PWMInterface::~PWMInterface(){}

#endif // PWMINTERFACE_H
