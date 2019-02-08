#include "backlightinterface.h"
#define DEFAULT_PERIOD 100000
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
