

#ifndef MATRIXDIAGONAL_H
#define MATRIXDIAGONAL_H

#include "Matrix.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>

template <typename T = double>
class MatrixDiagonal : public Matrix<T> {
private:
    unsigned _size;
    T* data; // Хранит диагональные элементы

public:
    // Конструктор
    MatrixDiagonal(unsigned size) : _size(size) {
        data = new T[_size]();
    }

    // Конструктор копирования
    MatrixDiagonal(const MatrixDiagonal<T>& other) : _size(other._size) {
        data = new T[_size];
        std::copy(other.data, other.data + _size, data);
    }

    // Конструктор перемещения
    MatrixDiagonal(MatrixDiagonal<T>&& other) noexcept : _size(other._size), data(other.data) {
        other.data = nullptr;
        other._size = 0;
    }

    // Деструктор
    ~MatrixDiagonal() {
        delete[] data;
    }

    // Оператор присваивания
    MatrixDiagonal<T>& operator=(const MatrixDiagonal<T>& other) {
        if (this != &other) {
            delete[] data;
            _size = other._size;
            data = new T[_size];
            std::copy(other.data, other.data + _size, data);
        }
        return *this;
    }

    // Оператор перемещающего присваивания
    MatrixDiagonal<T>& operator=(MatrixDiagonal<T>&& other) noexcept {
        if (this != &other) {
            delete[] data;
            _size = other._size;
            data = other.data;
            other.data = nullptr;
            other._size = 0;
        }
        return *this;
    }

    unsigned rows() const override { return _size; }
    unsigned cols() const override { return _size; }

    // Доступ к элементам
    T& operator()(unsigned i, unsigned j) {
        static T zero = T();
        if (i != j) {
            zero = T();
            return zero;
        }
        return data[i];
    }

    T operator()(unsigned i, unsigned j) const override {
        if (i != j) return T();
        return data[i];
    }



    // Сложение
    Matrix<T>& operator+=(const Matrix<T>& other) override {
        if (_size != other.rows() || _size != other.cols()) {
            throw std::invalid_argument("Размеры матриц должны совпадать для сложения.");
        }

        for (unsigned i = 0; i < _size; ++i) {
            data[i] += other(i, i);
        }
        return *this;
    }

    // Вычитание
    Matrix<T>& operator-=(const Matrix<T>& other) override {
        if (_size != other.rows() || _size != other.cols()) {
            throw std::invalid_argument("Размеры матриц должны совпадать для вычитания.");
        }

        for (unsigned i = 0; i < _size; ++i) {
            data[i] -= other(i, i);
        }
        return *this;
    }

    // Оператор сложения
    Matrix<T>* operator+(const Matrix<T>& other) const override {
        if (_size != other.rows() || _size != other.cols()) {
            throw std::invalid_argument("Размеры матриц должны совпадать для сложения.");
        }

        MatrixDiagonal<T>* result = new MatrixDiagonal<T>(*this);
        result->operator+=(other);
        return result;
    }

    // Оператор вычитания
    Matrix<T>* operator-(const Matrix<T>& other) const override {
        if (_size != other.rows() || _size != other.cols()) {
            throw std::invalid_argument("Размеры матриц должны совпадать для вычитания.");
        }

        MatrixDiagonal<T>* result = new MatrixDiagonal<T>(*this);
        result->operator-=(other);
        return result;
    }

    // Матричное умножение
    Matrix<T>* operator*(const Matrix<T>& other) const override {
        if (_size != other.rows()) {
            throw std::invalid_argument("Внутренние размеры матриц должны совпадать для умножения.");
        }

        MatrixDense<T>* result = new MatrixDense<T>(_size, other.cols());

        for (unsigned i = 0; i < _size; ++i) {
            for (unsigned j = 0; j < other.cols(); ++j) {
                result->operator()(i, j) = data[i] * other(i, j);
            }
        }
        return result;
    }

    // Почленное умножение
    Matrix<T>* elemMult(const Matrix<T>& other) const override {
        if (_size != other.rows() || _size != other.cols()) {
            throw std::invalid_argument("Размеры матриц должны совпадать для почленного умножения.");
        }

        MatrixDiagonal<T>* result = new MatrixDiagonal<T>(_size);

        for (unsigned i = 0; i < _size; ++i) {
            result->data[i] = data[i] * other(i, i);
        }
        return result;
    }

    // Почленное деление
    Matrix<T>* elemDiv(const Matrix<T>& other) const override {
        if (_size != other.rows() || _size != other.cols()) {
            throw std::invalid_argument("Размеры матриц должны совпадать для почленного деления.");
        }

        MatrixDiagonal<T>* result = new MatrixDiagonal<T>(_size);

        for (unsigned i = 0; i < _size; ++i) {
            if (other(i, i) == T()) {
                throw std::runtime_error("Деление на ноль при почленном делении матриц.");
            }
            result->data[i] = data[i] / other(i, i);
        }
        return result;
    }

    // Транспонирование
    MatrixDiagonal<T>* transpose() const override {
        // Транспонирование диагональной матрицы дает ту же матрицу
        return new MatrixDiagonal<T>(*this);
    }

    // Импорт из файла
    void importFromFile(const std::string& filename) override {
        std::ifstream infile(filename);
        if (!infile) {
            throw std::runtime_error("Не удалось открыть файл для чтения.");
        }

        std::string className;
        std::getline(infile, className);

        if (className != "MatrixDiagonal") {
            throw std::runtime_error("Файл не содержит данные MatrixDiagonal.");
        }

        unsigned size;
        infile >> size;

        delete[] data;
        _size = size;
        data = new T[_size];

        for (unsigned i = 0; i < _size; ++i) {
            infile >> data[i];
        }

        infile.close();
    }

    // Экспорт в файл
    void exportToFile(const std::string& filename) const override {
        std::ofstream outfile(filename);
        if (!outfile) {
            throw std::runtime_error("Не удалось открыть файл для записи.");
        }

        outfile << "MatrixDiagonal\n";
        outfile << _size << "\n";

        for (unsigned i = 0; i < _size; ++i) {
            outfile << data[i] << " ";
        }
        outfile << "\n";

        outfile.close();
    }

    // Метод для печати матрицы
void print(std::ostream& os = std::cout) const override {
    for (unsigned i = 0; i < _size; ++i) {
        for (unsigned j = 0; j < _size; ++j) {
            if (i == j)
                os << data[i] << "\t";
            else
                os << "0\t";
        }
        os << "\n";
    }
}
};

#endif 
