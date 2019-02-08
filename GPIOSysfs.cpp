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
GPIOSysfs::~GPIOSysfs(){
	ofstream interface("/sys/class/gpio/unexport");
	interface << std::to_string(pin);
	interface.close();
}
void GPIOSysfs::SetDirection(bool value){
	ofstream interface("/sys/class/gpio/gpio"+std::to_string(pin)+"/direction");
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
	if(GetDirection()){
		ofstream interface("/sys/class/gpio/gpio"+std::to_string(pin)+"/value");
		interface <<"0";
		interface.close();
		return true;
	}
	return false;
}
bool GPIOSysfs::Set(){
	if(GetDirection()){
		ofstream interface("/sys/class/gpio/gpio"+std::to_string(pin)+"/value");
		interface <<"1";
		interface.close();
		return true;
	}
	return false;
}
bool GPIOSysfs::Toggle(){
    char * value = new char [1];
	if(GetDirection()){
		ifstream interface("/sys/class/gpio/gpio"+std::to_string(pin)+"/value");
        interface.read(value,1);
		interface.close();
		if (*value=='1'){
			ofstream r_interface("/sys/class/gpio/gpio"+std::to_string(pin)+"/value");
            r_interface <<"0";
            r_interface.close();
		}
		else{
			ofstream r_interface("/sys/class/gpio/gpio"+std::to_string(pin)+"/value");
            r_interface <<"1";
            r_interface.close();
		}
		return true;
	}
    delete[] value;
	return false;
}
bool GPIOSysfs::Read(){
    char * value = new char [1];
	if(!GetDirection()){
	ifstream interface("/sys/class/gpio/gpio"+std::to_string(pin)+"/value");
	interface.read(value,1);
	interface.close();
		return (*value=='1')? true:false ;
	}
    delete[] value;
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
