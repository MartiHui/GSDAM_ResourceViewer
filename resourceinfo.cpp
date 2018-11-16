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
    memory = W_getMemoryUsage();
#elif __linux__
    memory = L_getMemoryUsage();
#endif

    return formatNumber(memory);
}

double ResourceInfo::getCpuUsage() {
    double cpu;
#ifdef _WIN32
    cpu = W_getCpuUsage();
#elif __linux__
    cpu = L_getCpuUsage();
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
void ResourceInfo::L_setMemoryTotal() {
    struct sysinfo memInfo;

    sysinfo (&memInfo);
    long long totalPhysMem = memInfo.totalram;
    //Multiply in next statement to avoid int overflow on right hand side...
    totalPhysMem *= memInfo.mem_unit;

    m_memoryTotal = totalPhysMem;
}

double ResourceInfo::L_getMemoryUsage() {
    struct sysinfo memInfo;

    sysinfo (&memInfo);
    long long physMemUsed = memInfo.totalram - memInfo.freeram;
    //Multiply in next statement to avoid int overflow on right hand side...
    physMemUsed *= memInfo.mem_unit;

    return ((float)physMemUsed / m_memoryTotal) * 100;
}

double ResourceInfo::L_getCpuUsage() {
    std::vector<CPUData> entries1;
    std::vector<CPUData> entries2;

    readStatsCpu(entries1);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    readStatsCpu(entries2);

    double usage = 0.0;
    const size_t NUM_ENTRIES = entries1.size();
    for (size_t i = 0; i < NUM_ENTRIES; ++i) {
        const CPUData &e1 = entries1[i];
        const CPUData &e2 = entries2[i];
        if (e1.cpu == "tot") {
            usage = (double)(getActiveTime(e2) - getActiveTime(e1));
            break;
        }
    }

    return usage;
}


void ResourceInfo::readStatsCpu(std::vector<CPUData> &entries) {
    std::ifstream fileStat("proc/stat");
    std::string line;

    const std::string STR_CPU("cpu");
    const std::size_t LEN_STR_CPU = STR_CPU.size();
    const std::string STR_TOT("tot");

    while (std::getline(fileStat, line)) {
        if (!line.compare(0, LEN_STR_CPU, STR_CPU)) {
            std::istringstream ss(line);

            entries.emplace_back(CPUData());
            CPUData &entry = entries.back();

            ss >> entry.cpu;

            if (entry.cpu.size() > LEN_STR_CPU) {
                entry.cpu.erase(0, LEN_STR_CPU);
            } else {
                entry.cpu = STR_TOT;
            }

            for (int i= 0; i < NUM_CPU_STATES; i++) {
                ss >> entry.times[i];
            }
        }
    }
}

size_t ResourceInfo::getIdleTime(const CPUData &e) {
    return e.times[S_IDLE] + e.times[S_IOWAIT];
}

size_t ResourceInfo::getActiveTime(const CPUData &e) {
    return e.times[S_USER] +
        e.times[S_NICE] +
        e.times[S_SYSTEM] +
        e.times[S_IRQ] +
        e.times[S_SOFTIRQ] +
        e.times[S_STEAL] +
        e.times[S_GUEST] +
        e.times[S_GUEST_NICE];
}
#endif
