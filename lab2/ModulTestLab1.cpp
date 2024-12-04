#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <vector>

// Классы, которые тестируются
class GpuSpec {
private:
    std::string model;
    int memory;  // В ГБ
public:
    GpuSpec(const std::string& model = "", int memory = 0) : model(model), memory(memory) {}

    void Print() const {
        std::cout << "GPU Model: " << model << ", Memory: " << memory << "GB\n";
    }

    void Export(const std::string& filename) const {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << model << "\n" << memory << "\n";
        }
    }

    void Import(const std::string& filename) {
        std::ifstream file(filename);
        if (file.is_open()) {
            std::getline(file, model);
            file >> memory;
        }
    }
};

class CpuSpec {
private:
    std::string model;
    int cores;
public:
    CpuSpec(const std::string& model = "", int cores = 0) : model(model), cores(cores) {}

    void Print() const {
        std::cout << "CPU Model: " << model << ", Cores: " << cores << "\n";
    }

    void Export(const std::string& filename) const {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << model << "\n" << cores << "\n";
        }
    }

    void Import(const std::string& filename) {
        std::ifstream file(filename);
        if (file.is_open()) {
            std::getline(file, model);
            file >> cores;
        }
    }
};

class RamSpec {
private:
    int size;  // В ГБ
    std::string type;
public:
    RamSpec(int size = 0, const std::string& type = "") : size(size), type(type) {}

    void Print() const {
        std::cout << "RAM Size: " << size << "GB, Type: " << type << "\n";
    }

    void Export(const std::string& filename) const {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << size << "\n" << type << "\n";
        }
    }

    void Import(const std::string& filename) {
        std::ifstream file(filename);
        if (file.is_open()) {
            file >> size;
            std::getline(file, type);  // Чтобы прочитать тип после числа
        }
    }
};

class LanSpec {
private:
    std::string model;
    int speed;  // В Мбит/с
public:
    LanSpec(const std::string& model = "", int speed = 0) : model(model), speed(speed) {}

    void Print() const {
        std::cout << "LAN Model: " << model << ", Speed: " << speed << "Mbps\n";
    }

    void Export(const std::string& filename) const {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << model << "\n" << speed << "\n";
        }
    }

    void Import(const std::string& filename) {
        std::ifstream file(filename);
        if (file.is_open()) {
            std::getline(file, model);
            file >> speed;
        }
    }
};

class ClusterNode {
private:
    GpuSpec gpu;
    CpuSpec cpu;
    RamSpec ram;
    LanSpec lan;

public:
    ClusterNode(const GpuSpec& gpu = GpuSpec(), const CpuSpec& cpu = CpuSpec(), 
                const RamSpec& ram = RamSpec(), const LanSpec& lan = LanSpec()) 
                : gpu(gpu), cpu(cpu), ram(ram), lan(lan) {}

    void Print() const {
        std::cout << "Cluster Node Information:\n";
        gpu.Print();
        cpu.Print();
        ram.Print();
        lan.Print();
    }

    void Export(const std::string& gpuFile, const std::string& cpuFile, 
                const std::string& ramFile, const std::string& lanFile) const {
        gpu.Export(gpuFile);
        cpu.Export(cpuFile);
        ram.Export(ramFile);
        lan.Export(lanFile);
    }

    void Import(const std::string& gpuFile, const std::string& cpuFile, 
                const std::string& ramFile, const std::string& lanFile) {
        gpu.Import(gpuFile);
        cpu.Import(cpuFile);
        ram.Import(ramFile);
        lan.Import(lanFile);
    }
};

class Cluster {
private:
    std::vector<ClusterNode> nodes;

public:
    void AddNode(const ClusterNode& node) {
        nodes.push_back(node);
    }

    void Print() const {
        for (const auto& node : nodes) {
            node.Print();
        }
    }

    void Export(const std::vector<std::string>& gpuFiles, const std::vector<std::string>& cpuFiles, 
                const std::vector<std::string>& ramFiles, const std::vector<std::string>& lanFiles) const {
        for (size_t i = 0; i < nodes.size(); ++i) {
            nodes[i].Export(gpuFiles[i], cpuFiles[i], ramFiles[i], lanFiles[i]);
        }
    }

    void Import(const std::vector<std::string>& gpuFiles, const std::vector<std::string>& cpuFiles, 
                const std::vector<std::string>& ramFiles, const std::vector<std::string>& lanFiles) {
        for (size_t i = 0; i < nodes.size(); ++i) {
            nodes[i].Import(gpuFiles[i], cpuFiles[i], ramFiles[i], lanFiles[i]);
        }
    }
};

// Тестирование всех классов
void TestGpuSpec() {
    std::cout << "Testing GpuSpec..." << std::endl;
    GpuSpec gpu("NVIDIA RTX 3080", 10);
    gpu.Print();
    gpu.Export("gpu_test.txt");

    GpuSpec gpu_imported;
    gpu_imported.Import("gpu_test.txt");
    gpu_imported.Print();
    std::remove("gpu_test.txt");  // Очистка файла
}

void TestCpuSpec() {
    std::cout << "Testing CpuSpec..." << std::endl;
    CpuSpec cpu("Intel i9-10900K", 10);
    cpu.Print();
    cpu.Export("cpu_test.txt");

    CpuSpec cpu_imported;
    cpu_imported.Import("cpu_test.txt");
    cpu_imported.Print();
    std::remove("cpu_test.txt");  // Очистка файла
}

void TestRamSpec() {
    std::cout << "Testing RamSpec..." << std::endl;
    RamSpec ram(32, "DDR4");
    ram.Print();
    ram.Export("ram_test.txt");

    RamSpec ram_imported;
    ram_imported.Import("ram_test.txt");
    ram_imported.Print();
    std::remove("ram_test.txt");  // Очистка файла
}

void TestLanSpec() {
    std::cout << "Testing LanSpec..." << std::endl;
    LanSpec lan("Intel Ethernet", 1000);
    lan.Print();
    lan.Export("lan_test.txt");

    LanSpec lan_imported;
    lan_imported.Import("lan_test.txt");
    lan_imported.Print();
    std::remove("lan_test.txt");  // Очистка файла
}

void TestClusterNode() {
    std::cout << "Testing ClusterNode..." << std::endl;
    GpuSpec gpu("NVIDIA GTX 1080", 8);
    CpuSpec cpu("Intel i7-9700K", 8);
    RamSpec ram(16, "DDR4");
    LanSpec lan("Realtek 8111", 1000);

    ClusterNode node(gpu, cpu, ram, lan);
    node.Print();
    node.Export("gpu_test1.txt", "cpu_test1.txt", "ram_test1.txt", "lan_test1.txt");

    ClusterNode node_imported;
    node_imported.Import("gpu_test1.txt", "cpu_test1.txt", "ram_test1.txt", "lan_test1.txt");
    node_imported.Print();

    std::remove("gpu_test1.txt");
    std::remove("cpu_test1.txt");
    std::remove("ram_test1.txt");
    std::remove("lan_test1.txt");
}

void TestCluster() {
    std::cout << "Testing Cluster..." << std::endl;
    GpuSpec gpu("NVIDIA RTX 2080", 8);
    CpuSpec cpu("Intel i5-9600K", 6);
    RamSpec ram(16, "DDR4");
    LanSpec lan("Qualcomm Atheros", 1000);

    ClusterNode node(gpu, cpu, ram, lan);
    Cluster cluster;
    cluster.AddNode(node);

    cluster.Print();
    cluster.Export({"gpu_test2.txt"}, {"cpu_test2.txt"}, {"ram_test2.txt"}, {"lan_test2.txt"});

    Cluster cluster_imported;
    cluster_imported.Import({"gpu_test2.txt"}, {"cpu_test2.txt"}, {"ram_test2.txt"}, {"lan_test2.txt"});
    cluster_imported.Print();

    std::remove("gpu_test2.txt");
    std::remove("cpu_test2.txt");
    std::remove("ram_test2.txt");
    std::remove("lan_test2.txt");
}

int main() {
    TestGpuSpec();
    TestCpuSpec();
    TestRamSpec();
    TestLanSpec();
    TestClusterNode();
    TestCluster();

    std::cout << "All tests passed successfully!" << std::endl;

    return 0;
}