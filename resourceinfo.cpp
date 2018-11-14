#include <math.h>

#include "resourceinfo.h"

ResourceInfo::ResourceInfo()
{
#ifdef _WIN32
    W_setMemoryTotal();
#elif __linux__

#endif
}

double ResourceInfo::getMemoryTotal() {
    double memory = (float)m_memoryTotal / (float)(1024 * 1024 * 1024);
    return formatNumber(memory);
}

double ResourceInfo::getMemoryUsage() {
    double memory;
#ifdef _WIN32
    memory =  W_getMemoryUsage();
#elif __linux__

#endif

    return formatNumber(memory);
}

double ResourceInfo::getCpuUsage() {
    double cpu;
#ifdef _WIN32
    cpu = W_getCpuUsage();
#elif __linux__

#endif

    return formatNumber(cpu);
}

// Redondea el número a dos decimales
double ResourceInfo::formatNumber(double num) {
    return round(num * 100) / 100;
}

#ifdef _WIN32
void ResourceInfo::W_setMemoryTotal() {
    MEMORYSTATUSEX memoryInfo;
    memoryInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memoryInfo);

    m_memoryTotal = memoryInfo.ullTotalPhys;
}

double ResourceInfo::W_getMemoryUsage() {
    MEMORYSTATUSEX memoryInfo;
    memoryInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memoryInfo);

    return (((float)(memoryInfo.ullTotalPhys - memoryInfo.ullAvailPhys))
            / (float)m_memoryTotal) * 100;
}

double ResourceInfo::W_getCpuUsage() {
    FILETIME idleTime, kernelTime, userTime;
    double cpu = GetSystemTimes(&idleTime, &kernelTime, &userTime) ?
                calculateCpuLoad(fileTimeToInt64(idleTime), fileTimeToInt64(kernelTime) +
                                 fileTimeToInt64(userTime)) : -1.0f;

    return cpu * 100;
}

unsigned long long ResourceInfo::fileTimeToInt64(const FILETIME &ft) {
    return (((unsigned long long)(ft.dwHighDateTime)) << 32) | ((unsigned long
                                                                 long)ft.dwLowDateTime);
}

float ResourceInfo::calculateCpuLoad(unsigned long long idleTicks, unsigned long long totalTicks) {
    static unsigned long long _previousTotalTicks = 0;
    static unsigned long long _previousIdleTicks = 0;
    unsigned long long totalTicksSinceLastTime = totalTicks - _previousTotalTicks;
    unsigned long long idleTicksSinceLastTime = idleTicks - _previousIdleTicks;
    float ret = 1.0f - ((totalTicksSinceLastTime > 0) ? ((float)idleTicksSinceLastTime) /
                                                        totalTicksSinceLastTime : 0);
    _previousTotalTicks = totalTicks;
    _previousIdleTicks = idleTicks;
    return ret;
}
#elif __linux

#endif
