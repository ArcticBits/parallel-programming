

#ifndef MATRIX_H
#define MATRIX_H

#include <string>
#include <iostream>

template <typename T = double>
class Matrix {
public:
    virtual ~Matrix() {}

    virtual unsigned rows() const = 0;
    virtual unsigned cols() const = 0;

    // Доступ к элементам
    virtual T operator()(unsigned i, unsigned j) const = 0;

    // Операции с матрицами
    virtual Matrix<T>& operator+=(const Matrix<T>& other) = 0;
    virtual Matrix<T>& operator-=(const Matrix<T>& other) = 0;
    virtual Matrix<T>* operator+(const Matrix<T>& other) const = 0;
    virtual Matrix<T>* operator-(const Matrix<T>& other) const = 0;
    virtual Matrix<T>* operator*(const Matrix<T>& other) const = 0;
    virtual Matrix<T>* elemMult(const Matrix<T>& other) const = 0;
    virtual Matrix<T>* elemDiv(const Matrix<T>& other) const = 0;
    virtual Matrix<T>* transpose() const = 0;

    // Функции импорта/экспорта
    virtual void importFromFile(const std::string& filename) = 0;
    virtual void exportToFile(const std::string& filename) const = 0;

    // Метод для печати матрицы
    virtual void print(std::ostream& os = std::cout) const = 0;
};

#endif 
