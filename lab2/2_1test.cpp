#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>


template <typename T = double>
class MatrixDense {
private:
    unsigned _m, _n;
    T* data;

public:
    // Конструктор
    MatrixDense(unsigned m, unsigned n) 
        : _m(m), _n(n), data(new T[m * n]{}) {}

    // Деструктор
    ~MatrixDense() {
        delete[] data;
    }

    // Оператор доступа
    T& operator()(unsigned i, unsigned j) {
        if (i >= _m || j >= _n)
            throw std::out_of_range("Индекс выходит за пределы матрицы");
        return data[i * _n + j];
    }

    const T& operator()(unsigned i, unsigned j) const {
        if (i >= _m || j >= _n)
            throw std::out_of_range("Индекс выходит за пределы матрицы");
        return data[i * _n + j];
    }

    // Оператор сложения
    MatrixDense operator+(const MatrixDense& other) const {
        if (_m != other._m || _n != other._n)
            throw std::invalid_argument("Размеры матриц должны совпадать для сложения");

        MatrixDense result(_m, _n);
        for (unsigned i = 0; i < _m * _n; ++i)
            result.data[i] = data[i] + other.data[i];

        return result;
    }

    // Оператор вычитания
    MatrixDense operator-(const MatrixDense& other) const {
        if (_m != other._m || _n != other._n)
            throw std::invalid_argument("Размеры матриц должны совпадать для вычитания");

        MatrixDense result(_m, _n);
        for (unsigned i = 0; i < _m * _n; ++i)
            result.data[i] = data[i] - other.data[i];

        return result;
    }

    // Почленное умножение
    MatrixDense operator*(const MatrixDense& other) const {
        if (_m != other._m || _n != other._n)
            throw std::invalid_argument("Размеры матриц должны совпадать для почленного умножения");

        MatrixDense result(_m, _n);
        for (unsigned i = 0; i < _m * _n; ++i)
            result.data[i] = data[i] * other.data[i];

        return result;
    }

    // Матричное умножение
    MatrixDense matmul(const MatrixDense& other) const {
        if (_n != other._m)
            throw std::invalid_argument("Для матричного умножения количество столбцов первой матрицы должно совпадать с количеством строк второй");

        MatrixDense result(_m, other._n);
        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < other._n; ++j) {
                result(i, j) = 0;
                for (unsigned k = 0; k < _n; ++k)
                    result(i, j) += (*this)(i, k) * other(k, j);
            }
        }

        return result;
    }

    // Транспонирование матрицы
    MatrixDense transpose() const {
        MatrixDense result(_n, _m);
        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < _n; ++j) {
                result(j, i) = (*this)(i, j);
            }
        }
        return result;
    }

    // Метод вывода с описанием
    void print(const std::string& description = "") const {
        if (!description.empty()) {
            std::cout << description << ":\n";
        }
        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < _n; ++j) {
                std::cout << (*this)(i, j) << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};


template <typename T = double>
class MatrixDiagonal {
private:
    unsigned size;           
    std::vector<T> data;    

public:
    // Конструктор
    MatrixDiagonal(unsigned size) 
        : size(size), data(size, 0) {}

    // Оператор доступа
    T& operator()(unsigned i, unsigned j) {
        if (i >= size || j >= size)
            throw std::out_of_range("Индекс выходит за пределы матрицы");
        if (i != j)
            throw std::invalid_argument("Можно обращаться только к диагональным элементам");
        return data[i];
    }

    const T& operator()(unsigned i, unsigned j) const {
        if (i >= size || j >= size)
            throw std::out_of_range("Индекс выходит за пределы матрицы");
        if (i != j)
            throw std::invalid_argument("Можно обращаться только к диагональным элементам");
        return data[i];
    }

    // Операция сложения
    MatrixDiagonal operator+(const MatrixDiagonal& other) const {
        if (size != other.size)
            throw std::invalid_argument("Размеры матриц должны совпадать");

        MatrixDiagonal result(size);
        for (unsigned i = 0; i < size; ++i) {
            result.data[i] = data[i] + other.data[i];
        }
        return result;
    }

    // Операция вычитания
    MatrixDiagonal operator-(const MatrixDiagonal& other) const {
        if (size != other.size)
            throw std::invalid_argument("Размеры матриц должны совпадать");

        MatrixDiagonal result(size);
        for (unsigned i = 0; i < size; ++i) {
            result.data[i] = data[i] - other.data[i];
        }
        return result;
    }

    // Почленное умножение
    MatrixDiagonal operator*(const MatrixDiagonal& other) const {
        if (size != other.size)
            throw std::invalid_argument("Размеры матриц должны совпадать");

        MatrixDiagonal result(size);
        for (unsigned i = 0; i < size; ++i) {
            result.data[i] = data[i] * other.data[i];
        }
        return result;
    }

    // Матричное умножение
    MatrixDiagonal matmul(const MatrixDiagonal& other) const {
        return (*this) * other; 
    }

    // Транспонирование
    MatrixDiagonal transpose() const {
        return *this;
    }

    void print(const std::string& description = "") const {
        if (!description.empty()) {
            std::cout << description << ":\n";
        }
        for (unsigned i = 0; i < size; ++i) {
            for (unsigned j = 0; j < size; ++j) {
                if (i == j)
                    std::cout << data[i] << " ";
                else
                    std::cout << "0 ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};

// Функция main()
int main() {
    // Работа с MatrixDense
    MatrixDense<> mat1(2, 2);
    MatrixDense<> mat2(2, 2);

    mat1(0, 0) = 1; mat1(0, 1) = 2;
    mat1(1, 0) = 3; mat1(1, 1) = 4;

    mat2(0, 0) = 5; mat2(0, 1) = 6;
    mat2(1, 0) = 7; mat2(1, 1) = 8;

    mat1.print("MatrixDense Матрица 1");
    mat2.print("MatrixDense Матрица 2");

    auto denseSum = mat1 + mat2;
    denseSum.print("Сложение плотных матриц");

    auto denseDiff = mat1 - mat2;
    denseDiff.print("Вычитание плотных матриц");

    auto denseElemMul = mat1 * mat2;
    denseElemMul.print("Почленное умножение плотных матриц");

    auto denseMatMul = mat1.matmul(mat2.transpose());
    denseMatMul.print("Матричное умножение плотных матриц");

    auto denseTranspose = mat1.transpose();
    denseTranspose.print("Транспонирование плотной матрицы 1");

    MatrixDiagonal<> diag1(3);
    MatrixDiagonal<> diag2(3);

    diag1(0, 0) = 1; diag1(1, 1) = 2; diag1(2, 2) = 3;
    diag2(0, 0) = 4; diag2(1, 1) = 5; diag2(2, 2) = 6;

    diag1.print("MatrixDiagonal Матрица 1");
    diag2.print("MatrixDiagonal Матрица 2");

    auto diagSum = diag1 + diag2;
    diagSum.print("Сложение диагональных матриц");

    auto diagDiff = diag1 - diag2;
    diagDiff.print("Вычитание диагональных матриц");

    auto diagElemMul = diag1 * diag2;
    diagElemMul.print("Почленное умножение диагональных матриц");

    auto diagMatMul = diag1.matmul(diag2);
    diagMatMul.print("Матричное умножение диагональных матриц");

    auto diagTranspose = diag1.transpose();
    diagTranspose.print("Транспонирование диагональной матрицы 1");

    return 0;
}