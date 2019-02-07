#include <ifstream>	
#include <ofstream>
#include "gpiointerface.h"
class GPIOSysfs :public GPIOInterface  {
	public:
	void SetDirection(bool value) override ;
	bool GetDirection() override ;
	void Reset() override ;
	void Set() override ;
	void Toggle() override ;
	bool Read() override ;
	GPIOSysfs(unsigned int pin) ;
	~GPIOSysfs();
	private:
	protected:
};
