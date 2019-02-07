#include <ifstream>	
#include <ofstream>
#include "stm32f303xc.h"

class GPIO  {
	public:
	void SetDirection(bool value);
	bool GetDirection();	
	void Reset();
	void Set();
	void Toggle();
	bool Read();
	GPIO(unsigned int pin);
	~GPIO();
	private:
		unsigned int pin;
		bool out;
	protected:
};
