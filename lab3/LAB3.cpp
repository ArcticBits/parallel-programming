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

using namespace std;

template<typename T>
class Vector {
    void writeExecutionTimeToFile(const std::string& functionName, double elapsedTime) {
    std::ofstream outFile("execution_times.txt", std::ios::app); // Открываем файл в режиме добавления
    if (outFile.is_open()) {
        outFile << "Функция " << functionName << ": " << std::fixed << std::setprecision(6) << elapsedTime << " секунд." << std::endl;
        outFile.close();
    } else {
        std::cerr << "Не удалось открыть файл execution_times.txt для записи." << std::endl;
    }
}
private:
    size_t sizeN = 0;

    T* mainVector;

    bool isInitialized = false;
public: 
    Vector(size_t sizeN) : sizeN(sizeN) {
        if (sizeN > 1000) {
            try {
                mainVector = new T[sizeN];
            }
            catch (const std::bad_alloc&) {
                std::cerr << "Недостаточно памяти для создания массива." << std::endl;
            }
        }            
        else
            std::cout << "Недостаточный размер массива" << std::endl;
    }

    ~Vector() { delete[] mainVector; }

    void initialize(double constValue) {
        for (int i{ 0 }; i < sizeN; i++) {
            mainVector[i] = (T) constValue;
        }
        isInitialized = true;
    }

    void initialize(double min, double max) {
        if (min >= max) {
            cout << "Ошибка: значение min должно быть меньше max." << std::endl;
            return;
        }

        for (size_t i = 0; i < sizeN; i++) {
            mainVector[i] = (T)(rand()) / RAND_MAX * (max - min) + min;
        }
        isInitialized = true;
    }

    /*void initialize(double min, double max) {
        mt19937 gen(std::random_device{}());
        uniform_real_distribution<> dist(min, max);

        for (size_t i = 0; i < sizeN; i++) {
            mainVector[i] = static_cast<T>(dist(gen));
        }
        isInitialized = true;
    }*/

    void exportToBin() {
        if (isInitialized) {
            size_t charsCount = sizeof(T) * sizeN;
            std::ofstream F("numbers.dat", std::ios::binary);
            if (F) {
                F.write(reinterpret_cast<const char*>(mainVector), charsCount);
                F.close();
                std::cout << "Данные успешно экспортированы в файл." << std::endl;
            }
            else {
                std::cerr << "Ошибка: не удалось открыть файл для записи." << std::endl;
            }
        }
        else {
            std::cout << "Элементов для вывода нет" << std::endl;
        }
    }

    void importFromBin() {
        if (!isInitialized) {
            std::ifstream F("numbers.dat", std::ios::binary);
            if (F) {
                size_t charsCount = sizeof(T) * sizeN;
                F.read(reinterpret_cast<char*>(mainVector), charsCount);
                if (F.gcount() != charsCount) {
                    std::cerr << "Ошибка: чтение из файла завершилось с ошибкой." << std::endl;
                }
                else {
                    isInitialized = true;  // Установка флага, если данные загружены успешно
                    std::cout << "Данные успешно загружены из файла." << std::endl;
                }
                F.close();
            }
            else {
                std::cerr << "Ошибка: не удалось открыть файл для чтения." << std::endl;
            }
        }
        else {
            std::cout << "Массив уже инициализирован." << std::endl;
        }
    }

    T findMin() {
        auto start = std::chrono::high_resolution_clock::now();

        if (!isInitialized) throw runtime_error("Вектор не инициализирован");
        T minVal = numeric_limits<T>::max();
        for (size_t i = 0; i < sizeN; i++) {
            if (mainVector[i] < minVal) minVal = mainVector[i];
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << std::fixed << std::setprecision(6);
        std::cout << "Время выполнения findMin: " << elapsed.count() << " секунд." << std::endl;
        writeExecutionTimeToFile("findMin", elapsed.count());
        return minVal;
    }
    // Параллельная версия findMin
    T findMinParallel(int numThreads) {
        if (!isInitialized) {
            throw std::runtime_error("Массив не инициализирован!");
        }

        std::vector<std::thread> threads;
        std::vector<T> localMins(numThreads, std::numeric_limits<T>::max());
        std::mutex minMutex;

        auto start = std::chrono::high_resolution_clock::now();

        size_t blockSize = sizeN / numThreads;
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back([this, &localMins, &minMutex, blockSize, i, numThreads]() {
                size_t startIdx = i * blockSize;
                size_t endIdx = (i == (numThreads - 1)) ? sizeN : startIdx + blockSize;
                T localMin = std::numeric_limits<T>::max();
                for (size_t j = startIdx; j < endIdx; ++j) {
                    if (mainVector[j] < localMin) {
                        localMin = mainVector[j];
                    }
                }
                // Защита от конкурентного доступа
                std::lock_guard<std::mutex> lock(minMutex);
                localMins[i] = localMin;
                });
        }

        // Ждём завершения всех потоков
        for (auto& th : threads) {
            th.join();
        }

        // Находим минимальное значение среди локальных минимумах
        T minValue = *std::min_element(localMins.begin(), localMins.end());

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << std::fixed << std::setprecision(6);
        std::cout << "Время выполнения findMin параллельно: " << elapsed.count() << " секунд." << std::endl;
        writeExecutionTimeToFile("findMinParallel", elapsed.count());
        return minValue;
    }

T findMax() {
    auto start = std::chrono::high_resolution_clock::now();

    if (!isInitialized) throw runtime_error("Вектор не инициализирован");
    T maxVal = numeric_limits<T>::min();
    for (size_t i = 0; i < sizeN; i++) {
        if (mainVector[i] > maxVal) maxVal = mainVector[i];
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
     std::cout << "Время выполнения findMax: " << elapsed.count() << " секунд." << std::endl;
    // Запись времени выполнения в файл
    writeExecutionTimeToFile("findMax", elapsed.count());

    return maxVal;
}
       

    // Параллельная версия findMax
    T findMaxParallel(int numThreads) {
        if (!isInitialized) {
            throw std::runtime_error("Массив не инициализирован!");
        }

        std::vector<std::thread> threads;
        std::vector<T> localMaxs(numThreads, std::numeric_limits<T>::min());
        std::mutex maxMutex;

        auto start = std::chrono::high_resolution_clock::now();

        size_t blockSize = sizeN / numThreads;
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back([this, &localMaxs, &maxMutex, blockSize, i, numThreads]() {
                size_t startIdx = i * blockSize;
                size_t endIdx = (i == (numThreads - 1)) ? sizeN : startIdx + blockSize;
                T localMax = std::numeric_limits<T>::min();
                for (size_t j = startIdx; j < endIdx; ++j) {
                    if (mainVector[j] > localMax) {
                        localMax = mainVector[j];
                    }
                }
                // Защита от конкурентного доступа
                std::lock_guard<std::mutex> lock(maxMutex);
                localMaxs[i] = localMax;
                });
        }

        for (auto& th : threads) {
            th.join();
        }

        T maxValue = *std::max_element(localMaxs.begin(), localMaxs.end());

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << std::fixed << std::setprecision(6);
        std::cout << "Время выполнения findMax параллельно: " << elapsed.count() << " секунд." << std::endl;
        writeExecutionTimeToFile("findMaxParallel", elapsed.count());
        return maxValue;
    }

    T findAvg() {
        auto start = std::chrono::high_resolution_clock::now();

        if (!isInitialized) throw runtime_error("Вектор не инициализирован");
        T sum = findSum();
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << std::fixed << std::setprecision(6);
        std::cout << "Время выполнения findAvg: " << elapsed.count() << " секунд." << std::endl;
        writeExecutionTimeToFile("findAvg", elapsed.count());
        return sum / static_cast<T>(sizeN);
    }

    // Параллельная версия findAvg
    T findAvgParallel(int numThreads) {
        if (!isInitialized) {
            throw std::runtime_error("Массив не инициализирован!");
        }

        std::vector<std::thread> threads;
        std::vector<T> localSums(numThreads, 0);
        std::vector<size_t> localCounts(numThreads, 0);
        std::mutex sumMutex;

        auto start = std::chrono::high_resolution_clock::now();
        size_t blockSize = sizeN / numThreads;

        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back([this, &localSums, &localCounts, &sumMutex, blockSize, i, numThreads]() {
                size_t startIdx = i * blockSize;
                size_t endIdx = (i == (numThreads - 1)) ? sizeN : startIdx + blockSize;

                T localSum = 0;
                for (size_t j = startIdx; j < endIdx; ++j) {
                    localSum += mainVector[j];
                }

                std::lock_guard<std::mutex> lock(sumMutex);
                localSums[i] = localSum;
                localCounts[i] = endIdx - startIdx; // Количество элементов
                });
        }

        for (auto& th : threads) {
            th.join();
        }

        T totalSum = std::accumulate(localSums.begin(), localSums.end(), static_cast<T>(0));
        size_t totalCount = std::accumulate(localCounts.begin(), localCounts.end(), static_cast<size_t>(0));

        auto avg = totalCount > 0 ? totalSum / totalCount : 0;

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << std::fixed << std::setprecision(6);
        std::cout << "Время выполнения Average параллельно: " << elapsed.count() << " секунд." << std::endl;
        writeExecutionTimeToFile("findAvgParallel", elapsed.count());
        return avg;
    }

    T findSum() {
        auto start = std::chrono::high_resolution_clock::now();

        if (!isInitialized) throw runtime_error("Вектор не инициализирован");
        T sum = 0;
        for (size_t i = 0; i < sizeN; i++) {
            sum += mainVector[i];
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << std::fixed << std::setprecision(6);
        std::cout << "Время выполнения findSum: " << elapsed.count() << " секунд." << std::endl;
        writeExecutionTimeToFile("findSum", elapsed.count());
        return sum;
    }

    // Параллельная версия суммы
    T findSumParallel(int numThreads) {
        if (!isInitialized) {
            throw std::runtime_error("Массив не инициализирован!");
        }

        std::vector<std::thread> threads;
        std::vector<T> localSums(numThreads, 0);
        std::mutex sumMutex;

        auto start = std::chrono::high_resolution_clock::now();

        size_t blockSize = sizeN / numThreads;
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back([this, &localSums, &sumMutex, blockSize, i, numThreads]() {
                size_t startIdx = i * blockSize;
                size_t endIdx = (i == (numThreads - 1)) ? sizeN : startIdx + blockSize;
                T localSum = 0;
                for (size_t j = startIdx; j < endIdx; ++j) {
                    localSum += mainVector[j];
                }
                std::lock_guard<std::mutex> lock(sumMutex);
                localSums[i] = localSum;
                });
        }

        for (auto& th : threads) {
            th.join();
        }

        T totalSum = std::accumulate(localSums.begin(), localSums.end(), static_cast<T>(0));

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << std::fixed << std::setprecision(6);
        std::cout << "Время выполнения sum параллельно: " << elapsed.count() << " секунд." << std::endl;
        writeExecutionTimeToFile("findSumParallel", elapsed.count());
        return totalSum;
    }

    double findEuclid() {
        auto start = std::chrono::high_resolution_clock::now();

        if (!isInitialized) throw runtime_error("Вектор не инициализирован");
        T sum = 0;
        for (size_t i = 0; i < sizeN; i++) {
            sum += mainVector[i] * mainVector[i];
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << std::fixed << std::setprecision(6);
        std::cout << "Время выполнения findEuclid: " << elapsed.count() << " секунд." << std::endl;
        writeExecutionTimeToFile("findEuclid", elapsed.count());
        return sqrt(sum);
    }

    // Параллельная версия findEuclid
    T findEuclidParallel(int numThreads) {
        if (!isInitialized) {
            throw std::runtime_error("Массив не инициализирован!");
        }

        std::vector<std::thread> threads;
        std::vector<T> localSums(numThreads, 0);
        std::mutex sumMutex;

        auto start = std::chrono::high_resolution_clock::now();

        size_t blockSize = sizeN / numThreads;
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back([this, &localSums, &sumMutex, blockSize, i, numThreads]() {
                size_t startIdx = i * blockSize;
                size_t endIdx = (i == (numThreads - 1)) ? sizeN : startIdx + blockSize;
                T localSum = 0;

                for (size_t j = startIdx; j < endIdx; ++j) {
                    localSum += mainVector[j] * mainVector[j];
                }

                std::lock_guard<std::mutex> lock(sumMutex);
                localSums[i] = localSum;
                });
        }

        for (auto& th : threads) {
            th.join();
        }

        T totalSum = std::accumulate(localSums.begin(), localSums.end(), static_cast<T>(0));
        auto euclidNorm = std::sqrt(totalSum);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << std::fixed << std::setprecision(6);
        std::cout << "Время выполнения Euclidean norm параллельно: " << elapsed.count() << " секунд." << std::endl;
        writeExecutionTimeToFile("findEuclidParallel", elapsed.count());
        return euclidNorm;
    }

    T findManhattan() {
        auto start = std::chrono::high_resolution_clock::now();

        if (!isInitialized) throw runtime_error("Вектор не инициализирован");
        T sum = 0;
        for (size_t i = 0; i < sizeN; i++) {
            sum += abs(mainVector[i]);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << std::fixed << std::setprecision(6);
        std::cout << "Время выполнения findManhattan: " << elapsed.count() << " секунд." << std::endl;
         writeExecutionTimeToFile("findManhattan", elapsed.count());
        return sum;
    }

    // Параллельная версия findManhattan
    T findManhattanParallel(int numThreads) {
        if (!isInitialized) {
            throw std::runtime_error("Массив не инициализирован!");
        }

        std::vector<std::thread> threads;
        std::vector<T> localSums(numThreads, 0);
        std::mutex sumMutex;

        auto start = std::chrono::high_resolution_clock::now();

        size_t blockSize = sizeN / numThreads;
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back([this, &localSums, &sumMutex, blockSize, i, numThreads]() {
                size_t startIdx = i * blockSize;
                size_t endIdx = (i == (numThreads - 1)) ? sizeN : startIdx + blockSize;
                T localSum = 0;

                for (size_t j = startIdx; j < endIdx; ++j) {
                    localSum += std::abs(mainVector[j]); // Суммируем абсолютные значения
                }

                std::lock_guard<std::mutex> lock(sumMutex);
                localSums[i] = localSum; // Записываем локальную сумму
                });
        }

        for (auto& th : threads) {
            th.join(); // Ждем завершения всех потоков
        }

        T totalSum = std::accumulate(localSums.begin(), localSums.end(), static_cast<T>(0));

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << std::fixed << std::setprecision(6);
        std::cout << "Время выполнения Manhattan norm параллельно: " << elapsed.count() << " секунд." << std::endl;
        writeExecutionTimeToFile("findManhattanParallel", elapsed.count());
        return totalSum;
    }

    T findScalar(const Vector<T>& other) {
        auto start = std::chrono::high_resolution_clock::now();

        if (!isInitialized || !other.isInitialized) throw runtime_error("Один из векторов не инициализирован");
        if (sizeN != other.sizeN) throw invalid_argument("Вектора должны быть одного размера");

        T scalar = 0;
        for (size_t i = 0; i < sizeN; i++) {
            scalar += mainVector[i] * other.mainVector[i];
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << std::fixed << std::setprecision(6);
        std::cout << "Время выполнения findScalar: " << elapsed.count() << " секунд." << std::endl;
        writeExecutionTimeToFile("findScalar", elapsed.count());
        return scalar;
    }

    // Параллельная версия findScalar
    T findScalarParallel(const Vector<T>& other, int numThreads) {
        if (!isInitialized || !other.isInitialized) {
            throw std::runtime_error("Один из массивов не инициализирован!");
        }
        if (sizeN != other.sizeN) {
            throw std::runtime_error("Размеры массивов не совпадают!");
        }

        std::vector<std::thread> threads;
        std::vector<T> localSums(numThreads, 0);
        std::mutex sumMutex;

        auto start = std::chrono::high_resolution_clock::now();

        size_t blockSize = sizeN / numThreads;
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back([this, &other, &localSums, &sumMutex, blockSize, i, numThreads]() {
                size_t startIdx = i * blockSize;
                size_t endIdx = (i == (numThreads - 1)) ? sizeN : startIdx + blockSize;
                T localSum = 0;

                for (size_t j = startIdx; j < endIdx; ++j) {
                    localSum += mainVector[j] * other.mainVector[j];
                }

                std::lock_guard<std::mutex> lock(sumMutex);
                localSums[i] = localSum;
                });
        }

        for (auto& th : threads) {
            th.join();
        }

        T totalSum = std::accumulate(localSums.begin(), localSums.end(), static_cast<T>(0));

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << std::fixed << std::setprecision(6);
        std::cout << "Время выполнения Scalar product параллельно: " << elapsed.count() << " секунд." << std::endl;
        writeExecutionTimeToFile("findScalarParallel", elapsed.count());
        return totalSum;
    }
};


int main() {
    setlocale(LC_ALL, "RUS");
    srand(static_cast<unsigned int>(time(0))); // Инициализация генератора случайных чисел

    size_t arraySize;
    std::cout << "Введите размер массива: ";
    std::cin >> arraySize;

    Vector<double> vec(arraySize);

    // Инициализация случайными числами
    double minVal, maxVal;
    std::cout << "Введите значение min и max: ";
    std::cin >> minVal >> maxVal;
    vec.initialize(minVal, maxVal);

    // Вывод результатов
    std::cout << "Минимум: " << vec.findMin() << std::endl;
    std::cout << "Максимум: " << vec.findMax() << std::endl;
    std::cout << "Среднее: " << vec.findAvg() << std::endl;
    std::cout << "Сумма: " << vec.findSum() << std::endl;
    std::cout << "Норма Евклида: " << vec.findEuclid() << std::endl;
    std::cout << "Манхэттенская норма: " << vec.findManhattan() << std::endl;

    // Вывод нового массива в файл
    std::cout << "Введите значение min и max: ";
    std::cin >> minVal >> maxVal;
    vec.initialize(minVal, maxVal);
    vec.exportToBin();

    // Создаем новый вектор и загружаем данные из файла
    Vector<double> newVec(arraySize);
    newVec.importFromBin();

    // Вывод считанных данных
    std::cout << "Минимум считанного массива: " << newVec.findMin() << std::endl;
    std::cout << "Максимум считанного массива: " << newVec.findMax() << std::endl;
    std::cout << "Среднее считанного массива: " << newVec.findAvg() << std::endl;

    // Получение скалярного произведения
    std::cout << "Введите значение min и max: ";
    std::cin >> minVal >> maxVal;
    Vector<double> newVecScalar(arraySize);
    newVecScalar.initialize(minVal, maxVal);
    std::cout << "Векторное произведение: " << newVec.findScalar(newVec) << std::endl;

    // Запрос от пользователя на количество потоков
    int numThreads;
    std::cout << "Введите количество потоков: ";
    std::cin >> numThreads;

    // Параллельные вычисления
    try {
        std::cout << "\nПараллельные вычисления:" << std::endl;
        std::cout << "Минимум: " << newVec.findMinParallel(numThreads) << std::endl;
        std::cout << "Максимум: " << newVec.findMaxParallel(numThreads) << std::endl;
        std::cout << "Сумма: " << newVec.findSumParallel(numThreads) << std::endl;
        std::cout << "Среднее: " << newVec.findAvgParallel(numThreads) << std::endl;
        std::cout << "Евклидова норма: " << newVec.findEuclidParallel(numThreads) << std::endl;
        std::cout << "Манхэттенская норма: " << newVec.findManhattanParallel(numThreads) << std::endl;

        // Скалярное произведение с самим собой
        std::cout << "Скалярное произведение с самим собой: "
            << newVec.findScalarParallel(newVec, numThreads) << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}
