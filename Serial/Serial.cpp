//
// Created by eleps on 01.04.19.
//

#include "Serial.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <chrono>
using namespace std;
Serial::Serial(const char *pathname, speed_t speed,LogCallback cb){
    int fd;
    std::string path(pathname);
    this->m_cb=cb;
    if(CheckFilename(path)){
        throw;
    }
    Serial::paths.push_back(path);
    fd = open(pathname, O_RDWR | O_NONBLOCK | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        throw;
    }
    fcntl(fd, F_SETFL, 0);
    memset(&term, 0, sizeof(struct termios));
    if(SetSpeed(speed)){
        throw;
    }
    term.c_cflag |= (CREAD | CLOCAL);
    term.c_cflag &= ~CSIZE;
    term.c_cflag |= CS8;
    term.c_cflag &= ~CSTOPB;
    term.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    term.c_iflag &= ~(INPCK);
    term.c_oflag &= ~OPOST;
    if (tcsetattr(fd, TCSANOW, &term) < 0) {
        printf("Unable to set flags");
    }
    timeout.tv_sec  = 5;
    timeout.tv_usec = 0;

}
void Serial::PrintLog(uint8_t status, std::string text) {
    if (this->m_cb!=0) {
        m_cb(status,text);
    }
}

void Serial::PrintToCout(uint8_t status, string msg) {
    cout<<status<<msg<<endl;
}
bool Serial::ReceiveByte(std::vector<unsigned char> data){
    unsigned char buf[1];
    int rdlen;
    memset(reinterpret_cast<void*> (buf), 0x00, sizeof(unsigned char));
    rdlen = read(fd,reinterpret_cast<void*> (buf), sizeof(unsigned char));
    if (rdlen==1){
        data.push_back(buf[0]);
    }
    return (rdlen==1)? true: false;
}
bool Serial::ReceiveArray(std::vector<unsigned char> data){
    unsigned char buf[MaxLen];
    int rdlen;
    memset(reinterpret_cast<void*> (buf), 0x00, sizeof(unsigned char)*MaxLen);
    fd_set fs;
    int res;
    FD_ZERO (&fs);
    FD_SET(fd, &fs);
    res = select ( fd+1 , &fs, NULL, NULL, &timeout );
    if ((res > 0) && (FD_ISSET(fd, &fs) )) {
        rdlen = read(fd, reinterpret_cast<void *> (buf), sizeof(unsigned char) * MaxLen);
        for (int i = 0; i < rdlen; i++) {
            data.push_back(buf[i]);
        }
    }
    return (rdlen>0)? false: true;
}
bool Serial::SendByte(std::vector<unsigned char> data){
    write(fd, data.data(), sizeof(unsigned char) * 1);
    return false;
}
bool Serial::SendArray(std::vector<unsigned char> data){
    write(fd, data.data(), sizeof(unsigned char) * data.size());
    return false;
}
bool Serial::SetSpeed(speed_t speed){
    if ((cfsetispeed(&term, speed) < 0) || (cfsetospeed(&term, speed) < 0)) {
        printf("Unable to set baudrate");
        return true;
    }
    cur_speed=speed;
    return false;
}
bool Serial::CheckFilename(std::string path){
    for(unsigned int i=0; i<Serial::paths.size();i++){
        if(path.compare(Serial::paths.at(i))==0){
            cout<<"This UART is already used"<<endl;
            return true;
        }
    }
    return false;
}
