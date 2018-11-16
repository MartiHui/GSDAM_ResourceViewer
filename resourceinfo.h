#ifndef RESOURCEINFO_H
#define RESOURCEINFO_H

#ifdef _WIN32
    #define _WIN32_WINNT 0x0501
    #include "windows.h"
#elif __linux__
    #define NUM_CPU_STATES 10
    #include <chrono>
    #include <thread>
    #include <fstream>
    #include <iostream>
    #include "sys/types.h"
    #include "sys/sysinfo.h"
#endif

#include <string>
#include <vector>
#include <sstream>

class ResourceInfo
{
private:
    unsigned long long m_memoryTotal;

    double formatNumber(double num);

#ifdef _WIN32 // Windows
    void W_setMemoryTotal();
    double W_getMemoryUsage();
    double W_getCpuUsage();
    static unsigned long long fileTimeToInt64(const FILETIME &ft);
    static float calculateCpuLoad(unsigned long long idleTicks, unsigned long long totalTicks);
#elif __linux__ // Linux
    typedef struct CPUData {
        std::string cpu;
        size_t times[NUM_CPU_STATES];
    };
    enum CPUStates {
        S_USER = 0,
        S_NICE,
        S_SYSTEM,
        S_IDLE,
        S_IOWAIT,
        S_IRQ,
        S_SOFTIRQ,
        S_STEAL,
        S_GUEST,
        S_GUEST_NICE
    };

    void L_setMemoryTotal();
    double L_getMemoryUsage();
    double L_getCpuUsage();
    void readStatsCpu(std::vector<CPUData> &entries);
    size_t getIdleTime(const CPUData &e);
    size_t getActiveTime(const CPUData &e);
#endif

public:
    ResourceInfo();
    double getMemoryTotal();
    double getMemoryUsage();
    double getCpuUsage();

};

#endif // RESOURCEINFO_H
