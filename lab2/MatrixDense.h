

#ifndef MATRIXDENSE_H
#define MATRIXDENSE_H

#include "Matrix.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>

template <typename T = double>
class MatrixDense : public Matrix<T> {
private:
    unsigned _m, _n;
    T* data;

public:
    // Конструктор
    MatrixDense(unsigned m, unsigned n) : _m(m), _n(n) {
        data = new T[_m * _n]();
    }

    // Конструктор копирования
    MatrixDense(const MatrixDense<T>& other) : _m(other._m), _n(other._n) {
        data = new T[_m * _n];
        std::copy(other.data, other.data + _m * _n, data);
    }

    // Конструктор перемещения
    MatrixDense(MatrixDense<T>&& other) noexcept : _m(other._m), _n(other._n), data(other.data) {
        other.data = nullptr;
        other._m = other._n = 0;
    }

    // Деструктор
    ~MatrixDense() {
        delete[] data;
    }

    // Оператор присваивания
    MatrixDense<T>& operator=(const MatrixDense<T>& other) {
        if (this != &other) {
            delete[] data;
            _m = other._m;
            _n = other._n;
            data = new T[_m * _n];
            std::copy(other.data, other.data + _m * _n, data);
        }
        return *this;
    }

    // Оператор перемещающего присваивания
    MatrixDense<T>& operator=(MatrixDense<T>&& other) noexcept {
        if (this != &other) {
            delete[] data;
            _m = other._m;
            _n = other._n;
            data = other.data;
            other.data = nullptr;
            other._m = other._n = 0;
        }
        return *this;
    }

    unsigned rows() const override { return _m; }
    unsigned cols() const override { return _n; }

    // Доступ к элементам
    T& operator()(unsigned i, unsigned j) {
        return data[i * _n + j];
    }

    T operator()(unsigned i, unsigned j) const override {
        return data[i * _n + j];
    }

    // Операции с матрицами
    Matrix<T>& operator+=(const Matrix<T>& other) override {
        if (_m != other.rows() || _n != other.cols()) {
            throw std::invalid_argument("Размеры матриц должны совпадать для сложения.");
        }

        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < _n; ++j) {
                (*this)(i, j) += other(i, j);
            }
        }
        return *this;
    }

    Matrix<T>& operator-=(const Matrix<T>& other) override {
        if (_m != other.rows() || _n != other.cols()) {
            throw std::invalid_argument("Размеры матриц должны совпадать для вычитания.");
        }

        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < _n; ++j) {
                (*this)(i, j) -= other(i, j);
            }
        }
        return *this;
    }

    // Оператор сложения
    Matrix<T>* operator+(const Matrix<T>& other) const override {
        if (_m != other.rows() || _n != other.cols()) {
            throw std::invalid_argument("Размеры матриц должны совпадать для сложения.");
        }

        MatrixDense<T>* result = new MatrixDense<T>(_m, _n);

        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < _n; ++j) {
                result->operator()(i, j) = (*this)(i, j) + other(i, j);
            }
        }
        return result;
    }

    // Оператор вычитания
    Matrix<T>* operator-(const Matrix<T>& other) const override {
        if (_m != other.rows() || _n != other.cols()) {
            throw std::invalid_argument("Размеры матриц должны совпадать для вычитания.");
        }

        MatrixDense<T>* result = new MatrixDense<T>(_m, _n);

        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < _n; ++j) {
                result->operator()(i, j) = (*this)(i, j) - other(i, j);
            }
        }
        return result;
    }

    // Матричное умножение
    Matrix<T>* operator*(const Matrix<T>& other) const override {
        if (_n != other.rows()) {
            throw std::invalid_argument("Внутренние размеры матриц должны совпадать для умножения.");
        }

        MatrixDense<T>* result = new MatrixDense<T>(_m, other.cols());

        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < other.cols(); ++j) {
                T sum = T();
                for (unsigned k = 0; k < _n; ++k) {
                    sum += (*this)(i, k) * other(k, j);
                }
                (*result)(i, j) = sum;
            }
        }
        return result;
    }

    // Почленное умножение
    Matrix<T>* elemMult(const Matrix<T>& other) const override {
        if (_m != other.rows() || _n != other.cols()) {
            throw std::invalid_argument("Размеры матриц должны совпадать для почленного умножения.");
        }

        MatrixDense<T>* result = new MatrixDense<T>(_m, _n);

        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < _n; ++j) {
                result->operator()(i, j) = (*this)(i, j) * other(i, j);
            }
        }
        return result;
    }

    // Почленное деление
    Matrix<T>* elemDiv(const Matrix<T>& other) const override {
        if (_m != other.rows() || _n != other.cols()) {
            throw std::invalid_argument("Размеры матриц должны совпадать для почленного деления.");
        }

        MatrixDense<T>* result = new MatrixDense<T>(_m, _n);

        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < _n; ++j) {
                if (other(i, j) == T()) {
                    throw std::runtime_error("Деление на ноль при почленном делении матриц.");
                }
                result->operator()(i, j) = (*this)(i, j) / other(i, j);
            }
        }
        return result;
    }

    // Транспонирование
    MatrixDense<T>* transpose() const override {
        MatrixDense<T>* result = new MatrixDense<T>(_n, _m);

        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < _n; ++j) {
                result->operator()(j, i) = (*this)(i, j);
            }
        }
        return result;
    }

    // Произведение Кронекера
    MatrixDense<T>* kroneckerProduct(const MatrixDense<T>& other) const {
        unsigned m = _m * other.rows();
        unsigned n = _n * other.cols();
        MatrixDense<T>* result = new MatrixDense<T>(m, n);

        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < _n; ++j) {
                T a = (*this)(i, j);
                for (unsigned p = 0; p < other.rows(); ++p) {
                    for (unsigned q = 0; q < other.cols(); ++q) {
                        result->operator()(i * other.rows() + p, j * other.cols() + q) = a * other(p, q);
                    }
                }
            }
        }
        return result;
    }

    // Импорт из файла
    void importFromFile(const std::string& filename) override {
        std::ifstream infile(filename);
        if (!infile) {
            throw std::runtime_error("Не удалось открыть файл для чтения.");
        }

        std::string className;
        std::getline(infile, className);

        if (className != "MatrixDense") {
            throw std::runtime_error("Файл не содержит данные MatrixDense.");
        }

        unsigned m, n;
        infile >> m >> n;

        delete[] data;
        _m = m;
        _n = n;
        data = new T[_m * _n];

        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < _n; ++j) {
                infile >> (*this)(i, j);
            }
        }

        infile.close();
    }

    // Экспорт в файл
    void exportToFile(const std::string& filename) const override {
        std::ofstream outfile(filename);
        if (!outfile) {
            throw std::runtime_error("Не удалось открыть файл для записи.");
        }

        outfile << "MatrixDense\n";
        outfile << _m << " " << _n << "\n";

        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < _n; ++j) {
                outfile << (*this)(i, j) << " ";
            }
            outfile << "\n";
        }

        outfile.close();
    }

    // Метод для печати матрицы
void print(std::ostream& os = std::cout) const override {
    for (unsigned i = 0; i < _m; ++i) {
        for (unsigned j = 0; j < _n; ++j) {
            os << (*this)(i, j) << "\t";
        }
        os << "\n";
    }
}
};

#endif
