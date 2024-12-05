#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <vector>
#include <limits>
#include <iomanip>
#include <thread>
#include <mutex>
#include <stdexcept>
#include <numeric>
#include <cmath>
#include <algorithm>

using namespace std::chrono;

struct ArrayHelper {

    template<typename T>
    static T findMin(T* data, size_t size) {
        T minVal = std::numeric_limits<T>::max();
        for (size_t i = 0; i < size; ++i) {
            if (data[i] < minVal) minVal = data[i];
        }
        return minVal;
    }

    template<typename T>
    static T findMax(T* data, size_t size) {
        T maxVal = std::numeric_limits<T>::min();
        for (size_t i = 0; i < size; ++i) {
            if (data[i] > maxVal) maxVal = data[i];
        }
        return maxVal;
    }

    template<typename T>
    static T findSum(T* data, size_t size) {
        T sum = 0;
        for (size_t i = 0; i < size; ++i) {
            sum += data[i];
        }
        return sum;
    }

    template<typename T>
    static double findEuclid(T* data, size_t size) {
        T sumSq = 0;
        for (size_t i = 0; i < size; ++i) {
            sumSq += data[i] * data[i];
        }
        return std::sqrt(sumSq);
    }

    template<typename T>
    static T findManhattan(T* data, size_t size) {
        T sumAbs = 0;
        for (size_t i = 0; i < size; ++i) {
            sumAbs += std::abs(data[i]);
        }
        return sumAbs;
    }

    // Parallel methods using std::thread
    template<typename T>
    static T findMinParallel(T* data, size_t size, int numThreads) {
        std::vector<std::thread> threads;
        std::vector<T> localMins(numThreads, std::numeric_limits<T>::max());
        size_t blockSize = size / numThreads;

        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back([=, &localMins]() {
                size_t startIdx = i * blockSize;
                size_t endIdx = (i == numThreads - 1) ? size : startIdx + blockSize;
                T localMin = std::numeric_limits<T>::max();
                for (size_t j = startIdx; j < endIdx; ++j) {
                    if (data[j] < localMin) localMin = data[j];
                }
                localMins[i] = localMin;
            });
        }
        for (auto& th : threads) th.join();
        return *std::min_element(localMins.begin(), localMins.end());
    }

    template<typename T>
    static T findMaxParallel(T* data, size_t size, int numThreads) {
        std::vector<std::thread> threads;
        std::vector<T> localMaxs(numThreads, std::numeric_limits<T>::min());
        size_t blockSize = size / numThreads;

        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back([=, &localMaxs]() {
                size_t startIdx = i * blockSize;
                size_t endIdx = (i == numThreads - 1) ? size : startIdx + blockSize;
                T localMax = std::numeric_limits<T>::min();
                for (size_t j = startIdx; j < endIdx; ++j) {
                    if (data[j] > localMax) localMax = data[j];
                }
                localMaxs[i] = localMax;
            });
        }
        for (auto& th : threads) th.join();
        return *std::max_element(localMaxs.begin(), localMaxs.end());
    }

    template<typename T>
    static T findSumParallel(T* data, size_t size, int numThreads) {
        std::vector<std::thread> threads;
        std::vector<T> localSums(numThreads, 0);
        size_t blockSize = size / numThreads;

        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back([=, &localSums]() {
                size_t startIdx = i * blockSize;
                size_t endIdx = (i == numThreads - 1) ? size : startIdx + blockSize;
                T localSum = 0;
                for (size_t j = startIdx; j < endIdx; ++j) {
                    localSum += data[j];
                }
                localSums[i] = localSum;
            });
        }
        for (auto& th : threads) th.join();
        return std::accumulate(localSums.begin(), localSums.end(), static_cast<T>(0));
    }

    template<typename T>
    static double findEuclidParallel(T* data, size_t size, int numThreads) {
        std::vector<std::thread> threads;
        std::vector<T> localSums(numThreads, 0);
        size_t blockSize = size / numThreads;

        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back([=, &localSums]() {
                size_t startIdx = i * blockSize;
                size_t endIdx = (i == numThreads - 1) ? size : startIdx + blockSize;
                T localSumSq = 0;
                for (size_t j = startIdx; j < endIdx; ++j) {
                    localSumSq += data[j] * data[j];
                }
                localSums[i] = localSumSq;
            });
        }
        for (auto& th : threads) th.join();
        T totalSumSq = std::accumulate(localSums.begin(), localSums.end(), static_cast<T>(0));
        return std::sqrt(totalSumSq);
    }

    template<typename T>
    static T findManhattanParallel(T* data, size_t size, int numThreads) {
        std::vector<std::thread> threads;
        std::vector<T> localSums(numThreads, 0);
        size_t blockSize = size / numThreads;

        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back([=, &localSums]() {
                size_t startIdx = i * blockSize;
                size_t endIdx = (i == numThreads - 1) ? size : startIdx + blockSize;
                T localSumAbs = 0;
                for (size_t j = startIdx; j < endIdx; ++j) {
                    localSumAbs += std::abs(data[j]);
                }
                localSums[i] = localSumAbs;
            });
        }
        for (auto& th : threads) th.join();
        return std::accumulate(localSums.begin(), localSums.end(), static_cast<T>(0));
    }

    template<typename T>
    static T findScalarParallel(T* data1, T* data2, size_t size, int numThreads) {
        std::vector<std::thread> threads;
        std::vector<T> localSums(numThreads, 0);
        size_t blockSize = size / numThreads;

        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back([=, &localSums]() {
                size_t startIdx = i * blockSize;
                size_t endIdx = (i == numThreads - 1) ? size : startIdx + blockSize;
                T localSum = 0;
                for (size_t j = startIdx; j < endIdx; ++j) {
                    localSum += data1[j] * data2[j];
                }
                localSums[i] = localSum;
            });
        }
        for (auto& th : threads) th.join();
        return std::accumulate(localSums.begin(), localSums.end(), static_cast<T>(0));
    }
};


template<typename T>
class VectorData {
public:
    T* data;
    size_t size;

    VectorData(size_t size) : size(size) {
        if (size > 1000) {
            data = new T[size];
        } else {
            throw std::runtime_error("Недостаточный размер массива");
        }
    }

    ~VectorData() {
        delete[] data;
    }

    void initialize(T constValue) {
        std::fill(data, data + size, constValue);
    }

    void initialize(T minVal, T maxVal) {
        if (minVal >= maxVal) {
            throw std::invalid_argument("minVal должно быть меньше maxVal");
        }
        for (size_t i = 0; i < size; ++i) {
            data[i] = static_cast<T>(rand()) / RAND_MAX * (maxVal - minVal) + minVal;
        }
    }

    void exportToBin(const std::string& filename) {
        std::ofstream outFile(filename, std::ios::binary);
        if (outFile) {
            outFile.write(reinterpret_cast<const char*>(data), sizeof(T) * size);
        } else {
            throw std::runtime_error("Не удалось открыть файл для записи");
        }
    }

    void importFromBin(const std::string& filename) {
        std::ifstream inFile(filename, std::ios::binary);
        if (inFile) {
            inFile.read(reinterpret_cast<char*>(data), sizeof(T) * size);
            if (inFile.gcount() != sizeof(T) * size) {
                throw std::runtime_error("Ошибка чтения данных из файла");
            }
        } else {
            throw std::runtime_error("Не удалось открыть файл для чтения");
        }
    }
};

template<typename T>
struct FuncResult {
    T result;
    double time;

    FuncResult(T res, double t) : result(res), time(t) {}

    void print(const std::string& functionName) {
        std::cout << functionName << " result: " << result << ", time: " << time << " секунд." << std::endl;
    }
};

class VectorHelper {
public:
    template<typename T>
    static FuncResult<T> findMin(VectorData<T>& vec) {
        auto start = high_resolution_clock::now();
        T result = ArrayHelper::findMin(vec.data, vec.size);
        auto end = high_resolution_clock::now();
        double elapsed = duration_cast<duration<double>>(end - start).count();
        return FuncResult<T>(result, elapsed);
    }

    template<typename T>
    static FuncResult<T> findMax(VectorData<T>& vec) {
        auto start = high_resolution_clock::now();
        T result = ArrayHelper::findMax(vec.data, vec.size);
        auto end = high_resolution_clock::now();
        double elapsed = duration_cast<duration<double>>(end - start).count();
        return FuncResult<T>(result, elapsed);
    }

    template<typename T>
    static FuncResult<T> findSum(VectorData<T>& vec) {
        auto start = high_resolution_clock::now();
        T result = ArrayHelper::findSum(vec.data, vec.size);
        auto end = high_resolution_clock::now();
        double elapsed = duration_cast<duration<double>>(end - start).count();
        return FuncResult<T>(result, elapsed);
    }

    template<typename T>
    static FuncResult<double> findAvg(VectorData<T>& vec) {
        auto sumResult = findSum(vec);
        double avg = sumResult.result / static_cast<double>(vec.size);
        return FuncResult<double>(avg, sumResult.time);
    }

    template<typename T>
    static FuncResult<double> findEuclid(VectorData<T>& vec) {
        auto start = high_resolution_clock::now();
        double result = ArrayHelper::findEuclid(vec.data, vec.size);
        auto end = high_resolution_clock::now();
        double elapsed = duration_cast<duration<double>>(end - start).count();
        return FuncResult<double>(result, elapsed);
    }

    template<typename T>
    static FuncResult<T> findManhattan(VectorData<T>& vec) {
        auto start = high_resolution_clock::now();
        T result = ArrayHelper::findManhattan(vec.data, vec.size);
        auto end = high_resolution_clock::now();
        double elapsed = duration_cast<duration<double>>(end - start).count();
        return FuncResult<T>(result, elapsed);
    }

    // параллельнные методы

    template<typename T>
    static FuncResult<T> findMinParallel(VectorData<T>& vec, int numThreads) {
        auto start = high_resolution_clock::now();
        T result = ArrayHelper::findMinParallel(vec.data, vec.size, numThreads);
        auto end = high_resolution_clock::now();
        double elapsed = duration_cast<duration<double>>(end - start).count();
        return FuncResult<T>(result, elapsed);
    }

    template<typename T>
    static FuncResult<T> findMaxParallel(VectorData<T>& vec, int numThreads) {
        auto start = high_resolution_clock::now();
        T result = ArrayHelper::findMaxParallel(vec.data, vec.size, numThreads);
        auto end = high_resolution_clock::now();
        double elapsed = duration_cast<duration<double>>(end - start).count();
        return FuncResult<T>(result, elapsed);
    }

    template<typename T>
    static FuncResult<T> findSumParallel(VectorData<T>& vec, int numThreads) {
        auto start = high_resolution_clock::now();
        T result = ArrayHelper::findSumParallel(vec.data, vec.size, numThreads);
        auto end = high_resolution_clock::now();
        double elapsed = duration_cast<duration<double>>(end - start).count();
        return FuncResult<T>(result, elapsed);
    }

    template<typename T>
    static FuncResult<double> findAvgParallel(VectorData<T>& vec, int numThreads) {
        auto sumResult = findSumParallel(vec, numThreads);
        double avg = sumResult.result / static_cast<double>(vec.size);
        return FuncResult<double>(avg, sumResult.time);
    }

    template<typename T>
    static FuncResult<double> findEuclidParallel(VectorData<T>& vec, int numThreads) {
        auto start = high_resolution_clock::now();
        double result = ArrayHelper::findEuclidParallel(vec.data, vec.size, numThreads);
        auto end = high_resolution_clock::now();
        double elapsed = duration_cast<duration<double>>(end - start).count();
        return FuncResult<double>(result, elapsed);
    }

    template<typename T>
    static FuncResult<T> findManhattanParallel(VectorData<T>& vec, int numThreads) {
        auto start = high_resolution_clock::now();
        T result = ArrayHelper::findManhattanParallel(vec.data, vec.size, numThreads);
        auto end = high_resolution_clock::now();
        double elapsed = duration_cast<duration<double>>(end - start).count();
        return FuncResult<T>(result, elapsed);
    }

    template<typename T>
    static FuncResult<T> findScalarParallel(VectorData<T>& vec1, VectorData<T>& vec2, int numThreads) {
        if (vec1.size != vec2.size) {
            throw std::invalid_argument("Размеры векторов не совпадают");
        }
        auto start = high_resolution_clock::now();
        T result = ArrayHelper::findScalarParallel(vec1.data, vec2.data, vec1.size, numThreads);
        auto end = high_resolution_clock::now();
        double elapsed = duration_cast<duration<double>>(end - start).count();
        return FuncResult<T>(result, elapsed);
    }
};

int main() {
    setlocale(LC_ALL, "RUS");
    srand(static_cast<unsigned int>(time(0)));

    size_t arraySize;
    std::cout << "Введите размер массива: ";
    std::cin >> arraySize;

    VectorData<double> vec(arraySize);

    double minVal, maxVal;
    std::cout << "Введите значение min и max: ";
    std::cin >> minVal >> maxVal;
    vec.initialize(minVal, maxVal);

   
    auto minResult = VectorHelper::findMin(vec);
    minResult.print("Минимум");
    auto maxResult = VectorHelper::findMax(vec);
    maxResult.print("Максимум");
    auto avgResult = VectorHelper::findAvg(vec);
    avgResult.print("Среднее");
    auto sumResult = VectorHelper::findSum(vec);
    sumResult.print("Сумма");
    auto euclidResult = VectorHelper::findEuclid(vec);
    euclidResult.print("Норма Евклида");
    auto manhattanResult = VectorHelper::findManhattan(vec);
    manhattanResult.print("Манхэттенская норма");

    // экспорт
    vec.exportToBin("numbers.dat");

    //  импорт
    VectorData<double> newVec(arraySize);
    newVec.importFromBin("numbers.dat");

    auto newMinResult = VectorHelper::findMin(newVec);
    newMinResult.print("Минимум считанного массива");
    auto newMaxResult = VectorHelper::findMax(newVec);
    newMaxResult.print("Максимум считанного массива");
    auto newAvgResult = VectorHelper::findAvg(newVec);
    newAvgResult.print("Среднее считанного массива");

    VectorData<double> vecScalar(arraySize);
    std::cout << "Введите значение min и max для скалярного произведения: ";
    std::cin >> minVal >> maxVal;
    vecScalar.initialize(minVal, maxVal);

    auto scalarResult = VectorHelper::findScalarParallel(newVec, vecScalar, 1);
    scalarResult.print("Скалярное произведение");

    int numThreads;
    std::cout << "Введите количество потоков: ";
    std::cin >> numThreads;

    try {
        std::cout << "\nПараллельные вычисления:" << std::endl;
        auto minParResult = VectorHelper::findMinParallel(newVec, numThreads);
        minParResult.print("Минимум");
        auto maxParResult = VectorHelper::findMaxParallel(newVec, numThreads);
        maxParResult.print("Максимум");
        auto sumParResult = VectorHelper::findSumParallel(newVec, numThreads);
        sumParResult.print("Сумма");
        auto avgParResult = VectorHelper::findAvgParallel(newVec, numThreads);
        avgParResult.print("Среднее");
        auto euclidParResult = VectorHelper::findEuclidParallel(newVec, numThreads);
        euclidParResult.print("Норма Евклида");
        auto manhattanParResult = VectorHelper::findManhattanParallel(newVec, numThreads);
        manhattanParResult.print("Манхэттенская норма");

        auto scalarParResult = VectorHelper::findScalarParallel(newVec, newVec, numThreads);
        scalarParResult.print("Скалярное произведение с самим собой");
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}
