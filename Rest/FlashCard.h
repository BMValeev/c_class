//
// Created by User on 14.01.2019.
//
#define DEFAULT_PATRITION 	 "ntfs-3g "
#define DEFAULT_SDA 	 "/dev/sda1 "
#define NTFS_FIX 	 "ntfsfix /dev/sda1 2>&1 "
#define MIN_FREE_SIZE           150.0
#define MIN_SNAP_SIZE           10.0
#ifndef SAVE_FOLDER_DEFAULT
#define SAVE_FOLDER_DEFAULT  	 "/mnt/flash"
#endif
#include <mutex>
#include <vector>
#include <thread>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <map>
#include <cstdio>
#include <cerrno>
#include <cstdlib>
#include <fstream>
#include "Rest/defs.h"
#include "../Rest/loggable.h"

using namespace std;
#ifndef GSTREAMER_FLASHCARD_H
#define GSTREAMER_FLASHCARD_H

/*
class FlashCard {
private:
    std::string Patrition=DEFAULT_PATRITION;
    std::string DefaultDevice=DEFAULT_SDA;
    std::string FixCommand=NTFS_FIX;
    std::string ConsoleOut=" 2>&1 ";
    std::string SaveFolder=SAVE_FOLDER_DEFAULT;
    float MinVideoSize=MIN_FREE_SIZE;
    float MinSnapSize=MIN_SNAP_SIZE;
    bool    flash_mounted;
    int fileExist(std::string filename);
    float getAvailableSpace(const char *path);
    std::mutex flash_mx,mounting_mx;

public:
    FlashCard();
    ~FlashCard();
    bool getStatus();
    bool isMounted();
    bool isMountable();
    bool checkFlashSize(unsigned int value);
    unsigned int checkFlashSize();
    bool mountFlash();
    bool unmountFlash();

protected:
};
*/
class FlashCard : public Loggable {
public:


    static FlashCard & getInstance(std::string device);
    uint32_t log(LogCallback cb = printToCout);

    bool isInitialized(std::string device) const { FlashCard::Devices.find(device)!=Devices.end(); }
    bool begin( LogCallback cb = printToCout);
    bool getStatus();//
    bool isMounted();//
    bool isMountable();
    bool checkFlashSize(float value);//
    float checkFlashSize();//
    bool mountFlash();//
    bool unmountFlash();//
    bool setFS(std::string name);//
    bool setMountPoint(std::string name);//
    bool setPartition(std::string name);//
protected:
    FlashCard(std::string device);//
    ~FlashCard();
    bool fixFlash();//
    bool deepFixFlash();//
    void mountThread(FlashCard *thing);//
    std::thread *x;
    void createFolder();//
    void removeFolderFiles();//
    int fileExist();//
    float getAvailableSpace(const char *path);//
    bool mountNtfs();
    bool mountFat();
private:
    bool status;
    static std::map<std::string,FlashCard*> Devices;
    static vector<std::string> mountPoints;
    std::string mountPoint;
    std::string patrition;
    unsigned int fileSystem;

    std::string Patrition=DEFAULT_PATRITION;
    std::string DefaultDevice=DEFAULT_SDA;
    std::string SaveFolder=SAVE_FOLDER_DEFAULT;
    bool    flash_mounted;
    std::mutex flash_mx,mounting_mx,mPmutex,mBmutex;
    float MinVideoSize=MIN_FREE_SIZE;
    float MinSnapSize=MIN_SNAP_SIZE;
    std::string FixCommand=NTFS_FIX;
    std::string ConsoleOut=" 2>&1 ";
    bool mInit=false;
    enum FS{
        NTFS,
        FAT32
    };
};
#endif //GSTREAMER_FLASHCARD_H
