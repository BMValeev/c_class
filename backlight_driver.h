#include <ifstream>	
#include <ofstream>
#include "backlight.h"
class backlight_driver : public backlight {
	public:
	void SetPower(unsigned int l_power) override;
	void Start() override;
	void Stop() override;
	backlight_driver();
	~backlight_driver();
	private:
		unsigned int MaxResolution;
	protected:
};
