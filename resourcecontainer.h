#ifndef RESOURCECONTAINER_H
#define RESOURCECONTAINER_H

#include <string>

#include "resourceinfo.h"

#define CONTAINER_SIZE 50
#define FILENAME "ResourceUsage.log"

class ResourceContainer
{
private:
    ResourceInfo m_rInfo;

    void readFile();
    void writeFile(double cpu, double memory);
    void pushInfo(double cpu, double memory);

public:
    double m_cpuUsage[CONTAINER_SIZE] {0};
    double m_memoryUsage[CONTAINER_SIZE] {0};

    ResourceContainer();
    void getInfo();
};

#endif // RESOURCECONTAINER_H
