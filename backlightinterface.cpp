#include "backlightinterface.h"

BackLightInterface::~BackLightInterface(){}

bool BackLightInterface::IsOn(){
	return m_enable;
}
uint8_t BackLightInterface::GetPower(){
	return power;
}
