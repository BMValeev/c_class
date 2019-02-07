#include <ifstream>	
#include <ofstream>
#include "backlightinterface.h"
#define DEFAULT_PERIOD 100000
class backlight_pwm : public BackLightInterface {
	public:
	void SetPower(unsigned int l_power) override;
	void Start() override;
	void Stop() override;
	backlight_pwm(unsigned int l_pin);
	~backlight_pwm();
	void SetPeriod(unsigned int l_period);
	void SetDuty(unsigned int l_duty);
	private:
		unsigned int period;
		unsigned int duty;
		unsigned int pin;
	protected:
};
