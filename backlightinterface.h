#ifndef BACKLIGHTINTERFACE_H
#define BACKLIGHTINTERFACE_H

#include <stdint.h>

class BackLightInterface {
	public:
	virtual ~BackLightInterface() = 0;
		bool IsOn();
		unsigned int GetPower();
		virtual bool SetPower(unsigned int l_power) =0;
		virtual bool Start()=0;
		virtual bool Stop()=0;
	protected:
		bool	m_enable;
		unsigned int power;
		
};

#endif // PWMINTERFACE_H
