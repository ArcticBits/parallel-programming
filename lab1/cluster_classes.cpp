

#include "cluster_classes.h"
#include <iostream>
#include <fstream>

// Реализации методов GpuSpec

void GpuSpec::Print() const {
    std::cout << "Характеристики GPU:\n";
    std::cout << "Производитель: " << manufacturer << "\n";
    std::cout << "Модель: " << model << "\n";
    std::cout << "Объем памяти: " << memorySize << " ГБ\n";
    std::cout << "Частота ядра: " << coreClock << " МГц\n";
    std::cout << "Частота памяти: " << memoryClock << " МГц\n";
}

void GpuSpec::Import(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::getline(file, manufacturer);
        std::getline(file, model);
        file >> memorySize >> coreClock >> memoryClock;
        file.close();
    } else {
        std::cerr << "Не удалось открыть файл для импорта: " << filename << "\n";
    }
}

void GpuSpec::Export(const std::string& filename) const {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << manufacturer << "\n";
        file << model << "\n";
        file << memorySize << " " << coreClock << " " << memoryClock << "\n";
        file.close();
    } else {
        std::cerr << "Не удалось открыть файл для экспорта: " << filename << "\n";
    }
}

bool GpuSpec::operator==(const GpuSpec& other) const {
    return manufacturer == other.manufacturer &&
           model == other.model &&
           memorySize == other.memorySize &&
           coreClock == other.coreClock &&
           memoryClock == other.memoryClock;
}

// Реализации методов CpuSpec

void CpuSpec::Print() const {
    std::cout << "Характеристики CPU:\n";
    std::cout << "Производитель: " << manufacturer << "\n";
    std::cout << "Модель: " << model << "\n";
    std::cout << "Ядра: " << cores << "\n";
    std::cout << "Потоки: " << threads << "\n";
    std::cout << "Базовая частота: " << baseClock << " ГГц\n";
    std::cout << "Макс. частота: " << boostClock << " ГГц\n";
}

void CpuSpec::Import(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::getline(file, manufacturer);
        std::getline(file, model);
        file >> cores >> threads >> baseClock >> boostClock;
        file.close();
    } else {
        std::cerr << "Не удалось открыть файл для импорта: " << filename << "\n";
    }
}

void CpuSpec::Export(const std::string& filename) const {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << manufacturer << "\n";
        file << model << "\n";
        file << cores << " " << threads << " " << baseClock << " " << boostClock << "\n";
        file.close();
    } else {
        std::cerr << "Не удалось открыть файл для экспорта: " << filename << "\n";
    }
}

bool CpuSpec::operator==(const CpuSpec& other) const {
    return manufacturer == other.manufacturer &&
           model == other.model &&
           cores == other.cores &&
           threads == other.threads &&
           baseClock == other.baseClock &&
           boostClock == other.boostClock;
}

// Реализации методов RamSpec

void RamSpec::Print() const {
    std::cout << "Характеристики RAM:\n";
    std::cout << "Производитель: " << manufacturer << "\n";
    std::cout << "Тип: " << type << "\n";
    std::cout << "Объем: " << capacity << " ГБ\n";
    std::cout << "Скорость: " << speed << " МГц\n";
}

void RamSpec::Import(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::getline(file, manufacturer);
        std::getline(file, type);
        file >> capacity >> speed;
        file.close();
    } else {
        std::cerr << "Не удалось открыть файл для импорта: " << filename << "\n";
    }
}

void RamSpec::Export(const std::string& filename) const {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << manufacturer << "\n";
        file << type << "\n";
        file << capacity << " " << speed << "\n";
        file.close();
    } else {
        std::cerr << "Не удалось открыть файл для экспорта: " << filename << "\n";
    }
}

bool RamSpec::operator==(const RamSpec& other) const {
    return manufacturer == other.manufacturer &&
           type == other.type &&
           capacity == other.capacity &&
           speed == other.speed;
}

// Реализации методов LanSpec

void LanSpec::Print() const {
    std::cout << "Характеристики LAN:\n";
    std::cout << "Производитель: " << manufacturer << "\n";
    std::cout << "Модель: " << model << "\n";
    std::cout << "Тип: " << type << "\n";
    std::cout << "Скорость: " << speed << " Мбит/с\n";
}

void LanSpec::Import(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::getline(file, manufacturer);
        std::getline(file, model);
        std::getline(file, type);
        file >> speed;
        file.close();
    } else {
        std::cerr << "Не удалось открыть файл для импорта: " << filename << "\n";
    }
}

void LanSpec::Export(const std::string& filename) const {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << manufacturer << "\n";
        file << model << "\n";
        file << type << "\n";
        file << speed << "\n";
        file.close();
    } else {
        std::cerr << "Не удалось открыть файл для экспорта: " << filename << "\n";
    }
}

bool LanSpec::operator==(const LanSpec& other) const {
    return manufacturer == other.manufacturer &&
           model == other.model &&
           type == other.type &&
           speed == other.speed;
}

// Реализации методов ClusterNode

void ClusterNode::Print() const {
    std::cout << "Характеристики узла кластера:\n";
    cpu.Print();
    ram.Print();
    gpu.Print();
    lan.Print();
}

void ClusterNode::Import(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        // CPU
        std::getline(file, cpu.manufacturer);
        std::getline(file, cpu.model);
        file >> cpu.cores >> cpu.threads >> cpu.baseClock >> cpu.boostClock;
        file.ignore();

        // RAM
        std::getline(file, ram.manufacturer);
        std::getline(file, ram.type);
        file >> ram.capacity >> ram.speed;
        file.ignore();

        // GPU
        std::getline(file, gpu.manufacturer);
        std::getline(file, gpu.model);
        file >> gpu.memorySize >> gpu.coreClock >> gpu.memoryClock;
        file.ignore();

        // LAN
        std::getline(file, lan.manufacturer);
        std::getline(file, lan.model);
        std::getline(file, lan.type);
        file >> lan.speed;

        file.close();
    } else {
        std::cerr << "Не удалось открыть файл для импорта: " << filename << "\n";
    }
}

void ClusterNode::Export(const std::string& filename) const {
    std::ofstream file(filename);
    if (file.is_open()) {
        // CPU
        file << cpu.manufacturer << "\n";
        file << cpu.model << "\n";
        file << cpu.cores << " " << cpu.threads << " " << cpu.baseClock << " " << cpu.boostClock << "\n";

        // RAM
        file << ram.manufacturer << "\n";
        file << ram.type << "\n";
        file << ram.capacity << " " << ram.speed << "\n";

        // GPU
        file << gpu.manufacturer << "\n";
        file << gpu.model << "\n";
        file << gpu.memorySize << " " << gpu.coreClock << " " << gpu.memoryClock << "\n";

        // LAN
        file << lan.manufacturer << "\n";
        file << lan.model << "\n";
        file << lan.type << "\n";
        file << lan.speed << "\n";

        file.close();
    } else {
        std::cerr << "Не удалось открыть файл для экспорта: " << filename << "\n";
    }
}

bool ClusterNode::operator==(const ClusterNode& other) const {
    return cpu == other.cpu &&
           ram == other.ram &&
           gpu == other.gpu &&
           lan == other.lan;
}

// Реализации методов Cluster

void Cluster::Print() const {
    std::cout << "Характеристики кластера:\n";
    for (size_t i = 0; i < nodes.size(); ++i) {
        std::cout << "\nУзел " << i + 1 << ":\n";
        nodes[i].Print();
    }
}

void Cluster::Import(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        int nodeCount;
        file >> nodeCount;
        file.ignore();
        nodes.resize(nodeCount);
        for (int i = 0; i < nodeCount; ++i) {
            ClusterNode& node = nodes[i];

            // CPU
            std::getline(file, node.cpu.manufacturer);
            std::getline(file, node.cpu.model);
            file >> node.cpu.cores >> node.cpu.threads >> node.cpu.baseClock >> node.cpu.boostClock;
            file.ignore();

            // RAM
            std::getline(file, node.ram.manufacturer);
            std::getline(file, node.ram.type);
            file >> node.ram.capacity >> node.ram.speed;
            file.ignore();

            // GPU
            std::getline(file, node.gpu.manufacturer);
            std::getline(file, node.gpu.model);
            file >> node.gpu.memorySize >> node.gpu.coreClock >> node.gpu.memoryClock;
            file.ignore();

            // LAN
            std::getline(file, node.lan.manufacturer);
            std::getline(file, node.lan.model);
            std::getline(file, node.lan.type);
            file >> node.lan.speed;
            file.ignore();
        }
        file.close();
    } else {
        std::cerr << "Не удалось открыть файл для импорта: " << filename << "\n";
    }
}

void Cluster::Export(const std::string& filename) const {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << nodes.size() << "\n";
        for (const auto& node : nodes) {
            // CPU
            file << node.cpu.manufacturer << "\n";
            file << node.cpu.model << "\n";
            file << node.cpu.cores << " " << node.cpu.threads << " " << node.cpu.baseClock << " " << node.cpu.boostClock << "\n";

            // RAM
            file << node.ram.manufacturer << "\n";
            file << node.ram.type << "\n";
            file << node.ram.capacity << " " << node.ram.speed << "\n";

            // GPU
            file << node.gpu.manufacturer << "\n";
            file << node.gpu.model << "\n";
            file << node.gpu.memorySize << " " << node.gpu.coreClock << " " << node.gpu.memoryClock << "\n";

            // LAN
            file << node.lan.manufacturer << "\n";
            file << node.lan.model << "\n";
            file << node.lan.type << "\n";
            file << node.lan.speed << "\n";
        }
        file.close();
    } else {
        std::cerr << "Не удалось открыть файл для экспорта: " << filename << "\n";
    }
}

bool Cluster::operator==(const Cluster& other) const {
    return nodes == other.nodes;
}
