#include "GPIOSysfs.h"
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

GPIOSysfs::GPIOSysfs(unsigned int pin){
	ofstream interface("/sys/class/gpio/export");
	interface << std::to_string(pin);
	if (interface.fail()){
		throw "Pin Unavaliable";
	}
	interface.close();
	SetDirection(true);
	Set();
}
GPIOSysfs::~GPIO(){
	ofstream interface("/sys/class/gpio/unexport");
	interface << std::to_string(pin);
	interface.close();
}
void GPIOSysfs::SetDirection(bool value){
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
bool GPIOSysfs::GetDirection(){
	return out;
}
bool GPIOSysfs::Reset(){
	if(GetDirection){
		ofstream interface("/sys/class/gpio/gpio"+std::to_string(gpio)+"/value");
		interface <<"0";
		interface.close();
		return true;
	}
	return false;
}
bool GPIOSysfs::Set(){
	if(GetDirection){
		ofstream interface("/sys/class/gpio/gpio"+std::to_string(gpio)+"/value");
		interface <<"1";
		interface.close();
		return true;
	}
	return false;
}
bool GPIOSysfs::Toggle(){
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
bool GPIOSysfs::Read(){
	std::string value;
	if(!GetDirection){
	ifstream interface("/sys/class/gpio/gpio"+std::to_string(gpio)+"/value");
	value  << interface;
	interface.close();
		return (value=="1")? true:false ;		
	}
}
#ifdef C_CLASS_DEBUG
#include <unistd.h>
#include <iostream>
using namespace std;
int main(void)
{
try{
    GPIOSysfs pin11(67);
    cout <<'1';
    pin11.Set();
    cout <<'2';
    usleep(5*1000*1000);
    cout <<'3';
    pin11.Reset();
    usleep(5*1000*1000);
    cout <<'4';
    pin11.SetDirection(false);
    usleep(5*1000*1000);
    cout <<'5';
    cout << pin11.Read();
    usleep(5*1000*1000);
    cout <<'6';
} catch (const char* msg) {
     cerr << msg << endl;
   }
    return 1;
}
#endif
