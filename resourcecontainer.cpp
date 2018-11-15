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
    myFile.open(FILENAME);
    std::string linea = "";
    double cpu = 0.0;
    double memory = 0.0;

    while (myFile.peek() != EOF) {
        std::getline(myFile, linea);
        cpu = std::stod(linea.substr(0, 5));
        memory = std::stod((linea.substr(7, 5)));

        pushInfo(cpu, memory);
    }

}

void ResourceContainer::getInfo() {
    double cpu = m_rInfo.getCpuUsage();
    double memory = m_rInfo.getMemoryUsage();

    writeFile(cpu, memory);
    pushInfo(cpu, memory);
}

void ResourceContainer::writeFile(double cpu, double memory) {
    std::time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string fecha = std::ctime(&tt);
    std::string cpustr = std::to_string(cpu);
    cpustr.resize(5, '0');
    std::string memorystr = std::to_string(memory);
    memorystr.resize(5, '0');

    std::ofstream myFile;
    myFile.open(FILENAME, std::ios::app);
    myFile << cpustr << " "  << memorystr << " " << fecha; // ctime ya coloca un salto de linea al final del string
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
