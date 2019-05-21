//
// Created by User on 14.01.2019.
//
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <mutex>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "FlashCard.h"
using namespace std;

bool FlashCard::isMounted(){
    return flash_mounted;
}
bool FlashCard::setMountPoint(std::string name) {
    if (mountPoint == name){
        printLog(DebugLog, static_cast<std::string>(__func__) + "Device name is same: " + name);
        return false;
    }
    mPmutex.lock();
    if(find(FlashCard::mountPoints.begin(),FlashCard::mountPoints.end(),name)==FlashCard::mountPoints.end()){
        printLog(DebugLog, static_cast<std::string>(__func__) + "Device name is already used by someone: " + name);
        return true;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) + "Device name is set: " + name);
    FlashCard::mountPoints.push_back(mountPoint);
    mountPoint = name;
    mPmutex.unlock();
}
bool FlashCard::setPartition(std::string name) {
    mPmutex.lock();
    printLog(DebugLog, static_cast<std::string>(__func__) + "Device partition is set: " + name);
    patrition = name;
    mPmutex.unlock();
}
int FlashCard::fileExist(){
    struct stat   buffer;
    return (stat (mountPoint.c_str(), &buffer) == 0);
}
float FlashCard::getAvailableSpace(const char *path){
    struct statvfs stat;
    if (statvfs(path, &stat) != 0){
        return -1;
    }
    return ((float)  stat.f_bsize * (float) stat.f_bavail);
}
void FlashCard::createFolder(){
    FILE *fp;
    fp = popen((std::string("mkdir ")+mountPoint).c_str() , "r");
    fclose(fp);
}
void FlashCard::removeFolderFiles(){
    FILE *fp;
    fp = popen((std::string("rm -rf ")+mountPoint+std::string("/* 2>&1")).c_str(), "r");
    fclose(fp);
}
bool FlashCard::checkFlashSize(float value){
    float free;
    free= getAvailableSpace(SAVE_FOLDER_DEFAULT)/ (float) (1024*1024);
    std::cout<<" free space " <<free <<std::endl;
    return (free>value) ? true : false ;
}
float FlashCard::checkFlashSize(){
    float free;
    free= getAvailableSpace(SAVE_FOLDER_DEFAULT)/ (float) (1024*1024);
    return free ;
}
bool FlashCard::getStatus(){
    return status;
}
bool FlashCard::unmountFlash(){
    flash_mx.lock();
    try{
        umount(mountPoint.c_str());
        flash_mounted=false;
        flash_mx.unlock();
        return true;
    }
    catch (const std::exception&) {
        flash_mx.unlock();
        return true;
    }
}
bool FlashCard::setFS(std::string name) {
    if (name == "fat32"){
        fileSystem = FAT32;
        return false;
    }
    if (name == "ntfs"){
        fileSystem = NTFS;
        return false;
    }
    return true;
}
bool FlashCard::fixFlash(){
    if (fileSystem != NTFS){
        return false;
    }
    FILE *fp;
    char command[1000];
    flash_mx.lock();
    memset(command, 0x00, sizeof(command));
    fp =popen(FixCommand.c_str(), "r");
    sleep(2);
    fgets(command, 1000, fp);
    printLog(DebugLog, static_cast<std::string>(__func__) +  command);
    fclose(fp);
    flash_mx.unlock();
    return false;
}
bool FlashCard::deepFixFlash(){
    if (fileSystem != NTFS){
        return false;
    }
    FILE *fp;
    char command[1000];
    memset(command, 0x00, sizeof(command));
    flash_mx.lock();
    fp =popen(FixCommand.c_str(), "r");
    sleep(2);
    fgets(command, 1000, fp);
    printLog(DebugLog, static_cast<std::string>(__func__) +  command);
    fclose(fp);
    flash_mx.unlock();
    return false;
}
bool FlashCard::mountFlash() {
    switch(fileSystem){
        case NTFS: return mountNtfs();
        case FAT32: return mountFat();
        default: return true;
    }
}
bool FlashCard::mountNtfs() {
    FILE *fp;
    char command[1000];
    flash_mx.lock();
    memset(command, 0x00, sizeof(command));
    fp = popen((Patrition+DefaultDevice+std::string(" ")+mountPoint+std::string(" ")+ConsoleOut).c_str(), "r");
    fgets(command, 1000, fp);
    fclose(fp);
    printLog(DebugLog, static_cast<std::string>(__func__) +  command);
    if (strlen(command) > 30) {
        flash_mx.unlock();
        return true;
    }
    flash_mounted=true;
    flash_mx.unlock();
    return false;
}
bool FlashCard::mountFat() {
    int fp;
    char command[1000];
    flash_mx.lock();
    memset(command, 0x00, sizeof(command));
    fp =mount(patrition.c_str(),mountPoint.c_str(), "vfat",0x00,nullptr);
        if (fp) {
            printLog(DebugLog, static_cast<std::string>(__func__)+  "Mount failed" +  std::to_string(fp));
            flash_mx.unlock();
            return true;
    }
    printLog(DebugLog, static_cast<std::string>(__func__) +  "Mounted succesfully");
    flash_mounted=true;
    flash_mx.unlock();
    return false;
}
bool FlashCard::begin(LogCallback cb) {
    std::lock_guard<std::mutex> lock(mBmutex); // automatically unlocks when function is leaved, no need to call unlock
    if(mInit){
        printLog(InfoLog, static_cast<std::string>(__func__) + " FLASHCARD already initialized");
        return false;
    }
    setLogCallback(cb);
    if (!fileExist()) {
        createFolder();
    }
    unmountFlash();
    removeFolderFiles();
    if (!fileExist()) {
        isMountable();
    }
    status=true;
    std::thread x(&FlashCard::mountThread, this, this);
    x.detach();
    printLog(DebugLog, static_cast<std::string>(__func__) + " Initialized");
    return false;
}
bool FlashCard::isMountable(){
    if(!fileExist()){
        return false;
    }
    if(mountFlash()){
        unmountFlash();
        return true;
    }
    if(fixFlash()){
        unmountFlash();
        return true;
    }
    if(deepFixFlash()){
        unmountFlash();
        return true;
    }
    unmountFlash();
    return false;
}
FlashCard::FlashCard(std::string device)
        : Loggable () {
    printLog(DebugLog, static_cast<std::string>(__func__) + "FlashCard constructor called");
    if (FlashCard::Devices.find(device)==Devices.end()) throw std::logic_error("Instance already exists");
    Devices[device]=this;
}

FlashCard& FlashCard::getInstance(std::string device) {
    auto it=FlashCard::Devices.find(device);
    if(it==Devices.end()){
        Devices[device]=new FlashCard(device);
        FlashCard &Device = *FlashCard::Devices.find(device)->second;
        Device.setPartition(device);

        return *FlashCard::Devices.find(device)->second;
    }
    return *FlashCard::Devices.find(device)->second;
}



FlashCard::~FlashCard(){
    auto it=FlashCard::Devices.find(patrition);
    if(it!=Devices.end()){
        unmountFlash();
        Devices.erase(it);
    }
}

void FlashCard::mountThread(FlashCard *thing) {
    while(thing->getStatus()){
        if(!thing->isMounted()){
            thing->isMountable();
        }
        std::this_thread::sleep_for(1s);
    }
    return;
}


