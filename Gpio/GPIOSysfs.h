#include "gpiointerface.h"
class GPIOSysfs :public GPIOInterface  {
	public:
	void SetDirection(bool value) override ;
	bool GetDirection() override ;
	bool Reset() override ;
	bool Set() override ;
	bool Toggle() override ;
	bool Read() override ;
	GPIOSysfs(unsigned int pin) ;
	~GPIOSysfs();
	private:
	protected:
};
