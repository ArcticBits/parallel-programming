

#ifndef CLUSTER_CLASSES_H
#define CLUSTER_CLASSES_H

#include <string>
#include <vector>


class GpuSpec {
public:
    std::string manufacturer;
    std::string model;
    int memorySize; // в ГБ
    int coreClock; // в МГц
    int memoryClock; // в МГц

    void Print() const;
    void Import(const std::string& filename);
    void Export(const std::string& filename) const;

    bool operator==(const GpuSpec& other) const;
};

class CpuSpec {
public:
    std::string manufacturer;
    std::string model;
    int cores;
    int threads;
    double baseClock; // в ГГц
    double boostClock; // в ГГц

    void Print() const;
    void Import(const std::string& filename);
    void Export(const std::string& filename) const;

    bool operator==(const CpuSpec& other) const;
};


class RamSpec {
public:
    std::string manufacturer;
    std::string type; // DDR4, DDR5 и т.д.
    int capacity; // в ГБ
    int speed; // в МГц

    void Print() const;
    void Import(const std::string& filename);
    void Export(const std::string& filename) const;

    bool operator==(const RamSpec& other) const;
};


class LanSpec {
public:
    std::string manufacturer;
    std::string model;
    std::string type; // Ethernet, Wi-Fi и т.д.
    int speed; // в Мбит/с или Гбит/с

    void Print() const;
    void Import(const std::string& filename);
    void Export(const std::string& filename) const;

    bool operator==(const LanSpec& other) const;
};


class ClusterNode {
public:
    CpuSpec cpu;
    RamSpec ram;
    GpuSpec gpu;
    LanSpec lan;

    void Print() const;
    void Import(const std::string& filename);
    void Export(const std::string& filename) const;

    bool operator==(const ClusterNode& other) const;
};


class Cluster {
public:
    std::vector<ClusterNode> nodes;

    void Print() const;
    void Import(const std::string& filename);
    void Export(const std::string& filename) const;

    bool operator==(const Cluster& other) const;
};

#endif 