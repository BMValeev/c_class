
#include "backlightinterface.h"
class backlight_driver : public BackLightInterface {
	public:
	bool SetPower(unsigned int l_power) override;
	bool Start() override;
	bool Stop() override;
	backlight_driver();
	~backlight_driver();
	private:
		unsigned int MaxResolution;
	protected:
};
