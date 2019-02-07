#ifndef BACKLIGHTINTERFACE_H
#define BACKLIGHTINTERFACE_H

#include <stdint.h>

class BackLightInterface {
	public:
	virtual ~BackLightInterface() = 0;
		bool IsOn();
		unsigned int GetPower();
		virtual void SetPower(uint8_t l_power) =0;
		virtual void Start()=0;
		virtual void Stop()=0;
	private:
		bool	m_enable;
		unsigned int power;
		
};

#endif // PWMINTERFACE_H
