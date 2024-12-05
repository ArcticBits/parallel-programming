
#include <iostream>
#include <cassert>
#include "cluster_classes.h"

int main() {
    // Тестирование GpuSpec
    GpuSpec gpu;
    gpu.manufacturer = "NVIDIA";
    gpu.model = "RTX 3080";
    gpu.memorySize = 10;
    gpu.coreClock = 1440;
    gpu.memoryClock = 19000;

    std::cout << "Созданные данные GPU:\n";
    gpu.Print();
    std::cout << "____________________________\n";
    gpu.Export("gpu_spec.txt");

    GpuSpec gpu_imported;
    gpu_imported.Import("gpu_spec.txt");
    std::cout << "Импортированные данные GPU:\n";
    gpu_imported.Print();
    std::cout << "____________________________\n";

    assert(gpu_imported == gpu);

    // Тестирование CpuSpec
    CpuSpec cpu;
    cpu.manufacturer = "Intel";
    cpu.model = "Core i9-10900K";
    cpu.cores = 10;
    cpu.threads = 20;
    cpu.baseClock = 3.7;
    cpu.boostClock = 5.3;

    std::cout << "Созданные данные CPU:\n";
    cpu.Print();
    std::cout << "____________________________\n";
    cpu.Export("cpu_spec.txt");

    CpuSpec cpu_imported;
    cpu_imported.Import("cpu_spec.txt");
    std::cout << "Импортированные данные CPU:\n";
    cpu_imported.Print();
    std::cout << "____________________________\n";

    assert(cpu_imported == cpu);

    // Тестирование RamSpec
    RamSpec ram;
    ram.manufacturer = "Corsair";
    ram.type = "DDR4";
    ram.capacity = 16;
    ram.speed = 3200;

    std::cout << "Созданные данные RAM:\n";
    ram.Print();
    std::cout << "____________________________\n";
    ram.Export("ram_spec.txt");

    RamSpec ram_imported;
    ram_imported.Import("ram_spec.txt");
    std::cout << "Импортированные данные RAM:\n";
    ram_imported.Print();
    std::cout << "____________________________\n";

    assert(ram_imported == ram);

    // Тестирование LanSpec
    LanSpec lan;
    lan.manufacturer = "Intel";
    lan.model = "I210-T1";
    lan.type = "Ethernet";
    lan.speed = 1000;

    std::cout << "Созданные данные LAN:\n";
    lan.Print();
    std::cout << "____________________________\n";
    lan.Export("lan_spec.txt");

    LanSpec lan_imported;
    lan_imported.Import("lan_spec.txt");
    std::cout << "Импортированные данные LAN:\n";
    lan_imported.Print();
    std::cout << "____________________________\n";

    assert(lan_imported == lan);

    // Тестирование ClusterNode
    ClusterNode node;
    node.cpu = cpu;
    node.gpu = gpu;
    node.ram = ram;
    node.lan = lan;

    std::cout << "Созданные данные узла кластера:\n";
    node.Print();
    std::cout << "____________________________\n";
    node.Export("node_spec.txt");

    ClusterNode node_imported;
    node_imported.Import("node_spec.txt");
    std::cout << "Импортированные данные узла кластера:\n";
    node_imported.Print();
    std::cout << "____________________________\n";

    assert(node_imported == node);

    // Тестирование Cluster
    Cluster cluster;
    cluster.nodes.push_back(node);
    cluster.nodes.push_back(node_imported); 

    std::cout << "Созданные данные кластера:\n";
    cluster.Print();
    std::cout << "____________________________\n";
    cluster.Export("cluster_spec.txt");

    Cluster cluster_imported;
    cluster_imported.Import("cluster_spec.txt");
    std::cout << "Импортированные данные кластера:\n";
    cluster_imported.Print();
    std::cout << "____________________________\n";

    assert(cluster_imported == cluster);

    std::cout << "\nВсе тесты успешно пройдены.\n";

    return 0;
}