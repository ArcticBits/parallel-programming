

#ifndef MATRIXBLOCK_H
#define MATRIXBLOCK_H

#include "Matrix.h"
#include "MatrixDense.h"
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>
#include <stdexcept>

template <typename T = double>
class MatrixBlock : public Matrix<T> {
private:
    unsigned _blockRows, _blockCols;         // Количество блоков по строкам и столбцам
    unsigned _blockSizeM, _blockSizeN;       // Размер каждого блока
    std::vector<std::vector<std::shared_ptr<MatrixDense<T>>>> blocks;

public:
    // Конструктор
    MatrixBlock(unsigned blockRows, unsigned blockCols, unsigned blockSizeM, unsigned blockSizeN)
        : _blockRows(blockRows), _blockCols(blockCols), _blockSizeM(blockSizeM), _blockSizeN(blockSizeN) {
        blocks.resize(_blockRows, std::vector<std::shared_ptr<MatrixDense<T>>>(_blockCols, nullptr));
    }

    // Конструктор копирования
    MatrixBlock(const MatrixBlock<T>& other)
        : _blockRows(other._blockRows), _blockCols(other._blockCols),
          _blockSizeM(other._blockSizeM), _blockSizeN(other._blockSizeN) {
        blocks.resize(_blockRows, std::vector<std::shared_ptr<MatrixDense<T>>>(_blockCols, nullptr));
        for (unsigned i = 0; i < _blockRows; ++i) {
            for (unsigned j = 0; j < _blockCols; ++j) {
                if (other.blocks[i][j]) {
                    blocks[i][j] = std::make_shared<MatrixDense<T>>(*other.blocks[i][j]);
                }
            }
        }
    }

    // Конструктор перемещения
    MatrixBlock(MatrixBlock<T>&& other) noexcept
        : _blockRows(other._blockRows), _blockCols(other._blockCols),
          _blockSizeM(other._blockSizeM), _blockSizeN(other._blockSizeN),
          blocks(std::move(other.blocks)) {
        other._blockRows = 0;
        other._blockCols = 0;
        other._blockSizeM = 0;
        other._blockSizeN = 0;
    }

    // Деструктор
    ~MatrixBlock() {
        // Используем shared_ptr, поэтому явное удаление не требуется
    }

    // Оператор присваивания
    MatrixBlock<T>& operator=(const MatrixBlock<T>& other) {
        if (this != &other) {
            _blockRows = other._blockRows;
            _blockCols = other._blockCols;
            _blockSizeM = other._blockSizeM;
            _blockSizeN = other._blockSizeN;

            blocks.resize(_blockRows, std::vector<std::shared_ptr<MatrixDense<T>>>(_blockCols, nullptr));
            for (unsigned i = 0; i < _blockRows; ++i) {
                for (unsigned j = 0; j < _blockCols; ++j) {
                    if (other.blocks[i][j]) {
                        blocks[i][j] = std::make_shared<MatrixDense<T>>(*other.blocks[i][j]);
                    }
                }
            }
        }
        return *this;
    }

    // Оператор перемещающего присваивания
    MatrixBlock<T>& operator=(MatrixBlock<T>&& other) noexcept {
        if (this != &other) {
            _blockRows = other._blockRows;
            _blockCols = other._blockCols;
            _blockSizeM = other._blockSizeM;
            _blockSizeN = other._blockSizeN;
            blocks = std::move(other.blocks);

            other._blockRows = 0;
            other._blockCols = 0;
            other._blockSizeM = 0;
            other._blockSizeN = 0;
        }
        return *this;
    }

    unsigned rows() const override { return _blockRows * _blockSizeM; }
    unsigned cols() const override { return _blockCols * _blockSizeN; }

    // Установка блока
    void setBlock(unsigned blockRow, unsigned blockCol, std::shared_ptr<MatrixDense<T>> block) {
        if (block->rows() != _blockSizeM || block->cols() != _blockSizeN) {
            throw std::invalid_argument("Размер блока не соответствует размеру блока матрицы.");
        }
        blocks[blockRow][blockCol] = block;
    }

    // Доступ к элементам
    T operator()(unsigned i, unsigned j) const override {
        unsigned blockRow = i / _blockSizeM;
        unsigned blockCol = j / _blockSizeN;
        unsigned localRow = i % _blockSizeM;
        unsigned localCol = j % _blockSizeN;

        if (blocks[blockRow][blockCol]) {
            return (*blocks[blockRow][blockCol])(localRow, localCol);
        } else {
            return T();
        }
    }

    // Помощник для установки элемента
    void setElement(unsigned i, unsigned j, T value) {
        unsigned blockRow = i / _blockSizeM;
        unsigned blockCol = j / _blockSizeN;
        unsigned localRow = i % _blockSizeM;
        unsigned localCol = j % _blockSizeN;

        if (!blocks[blockRow][blockCol]) {
            blocks[blockRow][blockCol] = std::make_shared<MatrixDense<T>>(_blockSizeM, _blockSizeN);
        }
        (*blocks[blockRow][blockCol])(localRow, localCol) = value;
    }

    // Операции с матрицами

    // Сложение
    Matrix<T>& operator+=(const Matrix<T>& other) override {
        if (rows() != other.rows() || cols() != other.cols()) {
            throw std::invalid_argument("Размеры матриц должны совпадать для сложения.");
        }

        for (unsigned i = 0; i < rows(); ++i) {
            for (unsigned j = 0; j < cols(); ++j) {
                T value = (*this)(i, j) + other(i, j);
                setElement(i, j, value);
            }
        }

        return *this;
    }

    // Вычитание
    Matrix<T>& operator-=(const Matrix<T>& other) override {
        if (rows() != other.rows() || cols() != other.cols()) {
            throw std::invalid_argument("Размеры матриц должны совпадать для вычитания.");
        }

        for (unsigned i = 0; i < rows(); ++i) {
            for (unsigned j = 0; j < cols(); ++j) {
                T value = (*this)(i, j) - other(i, j);
                setElement(i, j, value);
            }
        }

        return *this;
    }

    // Оператор сложения
    Matrix<T>* operator+(const Matrix<T>& other) const override {
        if (rows() != other.rows() || cols() != other.cols()) {
            throw std::invalid_argument("Размеры матриц должны совпадать для сложения.");
        }

        MatrixBlock<T>* result = new MatrixBlock<T>(*this);
        result->operator+=(other);
        return result;
    }

    // Оператор вычитания
    Matrix<T>* operator-(const Matrix<T>& other) const override {
        if (rows() != other.rows() || cols() != other.cols()) {
            throw std::invalid_argument("Размеры матриц должны совпадать для вычитания.");
        }

        MatrixBlock<T>* result = new MatrixBlock<T>(*this);
        result->operator-=(other);
        return result;
    }

    // Матричное умножение
    Matrix<T>* operator*(const Matrix<T>& other) const override {
        if (cols() != other.rows()) {
            throw std::invalid_argument("Внутренние размеры матриц должны совпадать для умножения.");
        }

        // Для простоты вернем плотную матрицу
        MatrixDense<T>* result = new MatrixDense<T>(rows(), other.cols());

        for (unsigned i = 0; i < rows(); ++i) {
            for (unsigned j = 0; j < other.cols(); ++j) {
                T sum = T();
                for (unsigned k = 0; k < cols(); ++k) {
                    sum += (*this)(i, k) * other(k, j);
                }
                result->operator()(i, j) = sum;
            }
        }
        return result;
    }

    // Почленное умножение
    Matrix<T>* elemMult(const Matrix<T>& other) const override {
        if (rows() != other.rows() || cols() != other.cols()) {
            throw std::invalid_argument("Размеры матриц должны совпадать для почленного умножения.");
        }

        MatrixBlock<T>* result = new MatrixBlock<T>(_blockRows, _blockCols, _blockSizeM, _blockSizeN);

        for (unsigned i = 0; i < rows(); ++i) {
            for (unsigned j = 0; j < cols(); ++j) {
                T value = (*this)(i, j) * other(i, j);
                if (value != T()) {
                    result->setElement(i, j, value);
                }
            }
        }
        return result;
    }

    // Почленное деление
    Matrix<T>* elemDiv(const Matrix<T>& other) const override {
        if (rows() != other.rows() || cols() != other.cols()) {
            throw std::invalid_argument("Размеры матриц должны совпадать для почленного деления.");
        }

        MatrixBlock<T>* result = new MatrixBlock<T>(_blockRows, _blockCols, _blockSizeM, _blockSizeN);

        for (unsigned i = 0; i < rows(); ++i) {
            for (unsigned j = 0; j < cols(); ++j) {
                T denom = other(i, j);
                if (denom == T()) {
                    throw std::runtime_error("Деление на ноль при почленном делении матриц.");
                }
                T value = (*this)(i, j) / denom;
                if (value != T()) {
                    result->setElement(i, j, value);
                }
            }
        }
        return result;
    }

    // Транспонирование
    MatrixBlock<T>* transpose() const override {
        MatrixBlock<T>* result = new MatrixBlock<T>(_blockCols, _blockRows, _blockSizeN, _blockSizeM);

        for (unsigned i = 0; i < _blockRows; ++i) {
            for (unsigned j = 0; j < _blockCols; ++j) {
                if (blocks[i][j]) {
                    result->blocks[j][i] = std::shared_ptr<MatrixDense<T>>(blocks[i][j]->transpose());
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

        if (className != "MatrixBlock") {
            throw std::runtime_error("Файл не содержит данные MatrixBlock.");
        }

        infile >> _blockRows >> _blockCols >> _blockSizeM >> _blockSizeN;

        blocks.resize(_blockRows, std::vector<std::shared_ptr<MatrixDense<T>>>(_blockCols, nullptr));

        for (unsigned i = 0; i < _blockRows; ++i) {
            for (unsigned j = 0; j < _blockCols; ++j) {
                std::string hasBlock;
                infile >> hasBlock;
                if (hasBlock == "1") {
                    blocks[i][j] = std::make_shared<MatrixDense<T>>(_blockSizeM, _blockSizeN);
                    for (unsigned m = 0; m < _blockSizeM; ++m) {
                        for (unsigned n = 0; n < _blockSizeN; ++n) {
                            infile >> (*blocks[i][j])(m, n);
                        }
                    }
                }
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

        outfile << "MatrixBlock\n";
        outfile << _blockRows << " " << _blockCols << " " << _blockSizeM << " " << _blockSizeN << "\n";

        for (unsigned i = 0; i < _blockRows; ++i) {
            for (unsigned j = 0; j < _blockCols; ++j) {
                if (blocks[i][j]) {
                    outfile << "1\n";
                    for (unsigned m = 0; m < _blockSizeM; ++m) {
                        for (unsigned n = 0; n < _blockSizeN; ++n) {
                            outfile << (*blocks[i][j])(m, n) << " ";
                        }
                        outfile << "\n";
                    }
                } else {
                    outfile << "0\n";
                }
            }
        }

        outfile.close();
    }

    // Метод для печати матрицы
void print(std::ostream& os = std::cout) const override {
    for (unsigned i = 0; i < rows(); ++i) {
        for (unsigned j = 0; j < cols(); ++j) {
            os << (*this)(i, j) << "\t";
        }
        os << "\n";
    }
}
};

#endif 
