#include "backlightinterface.h"

BackLightInterface::~BackLightInterface(){}

bool BackLightInterface::IsOn(){
	return m_enable;
}
unsigned int BackLightInterface::GetPower(){
	return power;
}
