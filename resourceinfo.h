#ifndef RESOURCEINFO_H
#define RESOURCEINFO_H

#ifdef _WIN32
    #define _WIN32_WINNT 0x0501
    #include "windows.h"
#elif __linux__

#endif

class ResourceInfo
{
private:
    unsigned long long m_memoryTotal;

#ifdef _WIN32 // Windows
    void W_setMemoryTotal();
    double W_getMemoryUsage();
    double W_getCpuUsage();
    static unsigned long long fileTimeToInt64(const FILETIME &ft);
    static float calculateCpuLoad(unsigned long long idleTicks, unsigned long long totalTicks);
#elif __linux__ // Linux

#endif
    double formatNumber(double num);

public:
    ResourceInfo();
    double getMemoryTotal();
    double getMemoryUsage();
    double getCpuUsage();

};

#endif // RESOURCEINFO_H
