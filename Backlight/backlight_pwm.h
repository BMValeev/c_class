#ifndef BACKLIGHT_PWM_H
#define BACKLIGHT_PWM_H

#include "backlightinterface.h"
#define DEFAULT_PERIOD_NS 100000
#define DEFAULT_POWER 100

class backlight_pwm : public BackLightInterface {
	public:
	bool SetPower(unsigned int l_power) override;
	bool Start() override;
	bool Stop() override;
	backlight_pwm(unsigned int l_pin);
	~backlight_pwm();
	bool SetPeriod(unsigned int l_period);
	bool SetDuty(unsigned int l_duty);
	private:
		unsigned int period;
		unsigned int duty;
		unsigned int pin;
	protected:
};

#endif // BACKLIGHT_PWM_H
