#include <iostream>
#include <fstream>
#include <string>

// Класс для характеристик GPU
class GpuSpec {
private:
    std::string model;
    int memory;  
public:
    GpuSpec(const std::string &model = "", int memory = 0) : model(model), memory(memory) {}

    void Print() const {
        std::cout << "GPU Model: " << model << ", Memory: " << memory << "GB\n";
    }

    void Import(const std::string &filename) {
        std::ifstream file(filename);
        if (file.is_open()) {
            getline(file, model);
            file >> memory;
        }
    }

    void Export(const std::string &filename) const {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << model << "\n" << memory << "\n";
        }
    }
};

// Класс для характеристик CPU
class CpuSpec {
private:
    std::string model;
    int cores;
public:
    CpuSpec(const std::string &model = "", int cores = 0) : model(model), cores(cores) {}

    void Print() const {
        std::cout << "CPU Model: " << model << ", Cores: " << cores << "\n";
    }

    void Import(const std::string &filename) {
        std::ifstream file(filename);
        if (file.is_open()) {
            getline(file, model);
            file >> cores;
        }
    }

    void Export(const std::string &filename) const {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << model << "\n" << cores << "\n";
        }
    }
};

// Класс для характеристик RAM
class RamSpec {
private:
    int size;  
    std::string type;
public:
    RamSpec(int size = 0, const std::string &type = "") : size(size), type(type) {}

    void Print() const {
        std::cout << "RAM Size: " << size << "GB, Type: " << type << "\n";
    }

    void Import(const std::string &filename) {
        std::ifstream file(filename);
        if (file.is_open()) {
            file >> size;
            getline(file, type);
        }
    }

    void Export(const std::string &filename) const {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << size << "\n" << type << "\n";
        }
    }
};

// Класс для характеристик LAN
class LanSpec {
private:
    std::string model;
    int speed;  
public:
    LanSpec(const std::string &model = "", int speed = 0) : model(model), speed(speed) {}

    void Print() const {
        std::cout << "LAN Model: " << model << ", Speed: " << speed << "Mbps\n";
    }

    void Import(const std::string &filename) {
        std::ifstream file(filename);
        if (file.is_open()) {
            getline(file, model);
            file >> speed;
        }
    }

    void Export(const std::string &filename) const {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << model << "\n" << speed << "\n";
        }
    }
};

// Класс для вычислительного узла
class ClusterNode {
private:
    GpuSpec gpu;
    CpuSpec cpu;
    RamSpec ram;
    LanSpec lan;

public:
    ClusterNode(const GpuSpec &gpu = GpuSpec(), const CpuSpec &cpu = CpuSpec(), 
                const RamSpec &ram = RamSpec(), const LanSpec &lan = LanSpec()) 
                : gpu(gpu), cpu(cpu), ram(ram), lan(lan) {}

    void Print() const {
        std::cout << "Cluster Node Information:\n";
        gpu.Print();
        cpu.Print();
        ram.Print();
        lan.Print();
    }

    void Import(const std::string &gpuFile, const std::string &cpuFile, 
                const std::string &ramFile, const std::string &lanFile) {
        gpu.Import(gpuFile);
        cpu.Import(cpuFile);
        ram.Import(ramFile);
        lan.Import(lanFile);
    }

    void Export(const std::string &gpuFile, const std::string &cpuFile, 
                const std::string &ramFile, const std::string &lanFile) const {
        gpu.Export(gpuFile);
        cpu.Export(cpuFile);
        ram.Export(ramFile);
        lan.Export(lanFile);
    }
};

// Класс для вычислительного кластера
class Cluster {
private:
    std::vector<ClusterNode> nodes;

public:
    void AddNode(const ClusterNode &node) {
        nodes.push_back(node);
    }

    void Print() const {
        for (const auto &node : nodes) {
            node.Print();
        }
    }

    void Import(const std::vector<std::string> &gpuFiles, const std::vector<std::string> &cpuFiles, 
                const std::vector<std::string> &ramFiles, const std::vector<std::string> &lanFiles) {
        for (size_t i = 0; i < nodes.size(); ++i) {
            nodes[i].Import(gpuFiles[i], cpuFiles[i], ramFiles[i], lanFiles[i]);
        }
    }

    void Export(const std::vector<std::string> &gpuFiles, const std::vector<std::string> &cpuFiles, 
                const std::vector<std::string> &ramFiles, const std::vector<std::string> &lanFiles) const {
        for (size_t i = 0; i < nodes.size(); ++i) {
            nodes[i].Export(gpuFiles[i], cpuFiles[i], ramFiles[i], lanFiles[i]);
        }
    }
};

int main() {
    // Пример использования
    GpuSpec gpu("NVIDIA RTX 3080", 10);
    CpuSpec cpu("Intel i9-10900K", 10);
    RamSpec ram(32, "DDR4");
    LanSpec lan("Intel Ethernet", 1000);

    ClusterNode node(gpu, cpu, ram, lan);
    Cluster cluster;
    cluster.AddNode(node);

    cluster.Print();
    
    gpu.Export("gpu_data.txt");
    cpu.Export("cpu_data.txt");
    ram.Export("ram_data.txt");
    lan.Export("lan_data.txt");

    return 0;
}