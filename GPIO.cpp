#include "GPIO.h"
#include <fstream>
#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cerrno>
#include <cstdlib>
using namespace std;

GPIO::GPIO(unsigned int pin){
	ofstream interface("/sys/class/gpio/export");
	interface << std::to_string(pin);
	if (interface.fail()){
		throw "Pin Unavaliable";
	}
	interface.close();
	SetDirection(true);
	Set();
}
GPIO::~GPIO(){
	ofstream interface("/sys/class/gpio/unexport");
	interface << std::to_string(pin);
	interface.close();
}
void GPIO::SetDirection(bool value){
	ofstream interface("/sys/class/gpio/gpio"+std::to_string(gpio)+"/direction");
	if(value){
			interface <<"out";
			out=true;
			Reset();
	}
	else{
			interface <<"in";
			out=false;
	}
	interface.close();
}
bool GPIO::GetDirection(){
	return out;
}
bool GPIO::Reset(){
	if(GetDirection){
		ofstream interface("/sys/class/gpio/gpio"+std::to_string(gpio)+"/value");
		interface <<"0";
		interface.close();
		return true;
	}
	return false;
}
bool GPIO::Set(){
	if(GetDirection){
		ofstream interface("/sys/class/gpio/gpio"+std::to_string(gpio)+"/value");
		interface <<"1";
		interface.close();
		return true;
	}
	return false;
}
bool GPIO::Toggle(){
	std::string value;
	if(GetDirection){
		ifstream interface("/sys/class/gpio/gpio"+std::to_string(gpio)+"/value");
		value  << interface;
		interface.close();
		if (value=="1"){
			ofstream r_interface("/sys/class/gpio/gpio"+std::to_string(gpio)+"/value");
			interface <<"0";
		}
		else{
			ofstream r_interface("/sys/class/gpio/gpio"+std::to_string(gpio)+"/value");
			interface <<"1";
		}
		interface.close();
		return true;
	}
	return false;
}
bool GPIO::Read(){
	std::string value;
	if(!GetDirection){
	ifstream interface("/sys/class/gpio/gpio"+std::to_string(gpio)+"/value");
	value  << interface;
	interface.close();
		return (value=="1")? true:false ;		
	}
}
