#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <string>

#include "resourcecontainer.h"

ResourceContainer::ResourceContainer()
{
    readFile();
}

void ResourceContainer::readFile() {
    std::ifstream myFile;
    myFile.read(FILENAME);
}

void ResourceContainer::getInfo() {
    double cpu = rInfo.getCpuUsage();
    double memory = rInfo.getMemoryUsage();

    writeFile(cpu, memory);
    pushInfo(cpu, memory);
}

void ResourceContainer::writeFile(double cpu, double memory) {
    std::time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string fecha = std::ctime(&tt);

    std::ofstream myFile;
    myFile.open(FILENAME, std::ios::app);
    myFile << memory << " "  << cpu << " " << fecha; // ctime ya coloca un salto de linea al final del string
    myFile.close();
}

void ResourceContainer::pushInfo(double cpu, double memory) {
    for (int i = 0; i < CONTAINER_SIZE - 1; i++) {
        m_cpuUsage[i] = m_cpuUsage[i+1];
        m_memoryUsage[i] = m_memoryUsage[i+1];
    }
    m_cpuUsage[CONTAINER_SIZE-1] = cpu;
    m_memoryUsage[CONTAINER_SIZE-1] = memory;
}
