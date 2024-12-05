// main.cpp

#include "MatrixDense.h"
#include "MatrixDiagonal.h"
#include "MatrixBlock.h"
#include <iostream>
#include <fstream>  
#include <random>   

int main() {
    try {
        // Создаём генератор случайных чисел
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(-10, 10);  


        std::cout << "=== MatrixDense ===\n";

        // Создаем случайные плотные матрицы A и B размером 10x10 с целыми числами
        MatrixDense<int> A(10, 10);
        MatrixDense<int> B(10, 10);

        for (unsigned i = 0; i < A.rows(); ++i) {
            for (unsigned j = 0; j < A.cols(); ++j) {
                A(i, j) = dis(gen);
                B(i, j) = dis(gen);
            }
        }

        // Операции
        Matrix<int>* C_dense = A * B;
        Matrix<int>* D_dense = A.elemMult(B);
        Matrix<int>* E_dense = A.transpose();
        Matrix<int>* F_dense = A.elemMult(B);  

        // Операции сложения и вычитания
        MatrixDense<int> G_dense = A;
        MatrixDense<int> H_dense = A;
        G_dense += B;
        H_dense -= B;

        // Произведение Кронекера
        MatrixDense<int>* K_dense = A.kroneckerProduct(B);


        std::ofstream outfile_dense("MatrixDense.txt");
        if (!outfile_dense) {
            throw std::runtime_error("Не удалось открыть файл MatrixDense.txt для записи.");
        }

        // Записываем матрицы и результаты операций в файл
        outfile_dense << "Матрица A:\n";
        A.print(outfile_dense);

        outfile_dense << "\nМатрица B:\n";
        B.print(outfile_dense);

        outfile_dense << "\nМатрица C_dense = A * B:\n";
        C_dense->print(outfile_dense);

        outfile_dense << "\nМатрица D_dense = A почленное умножение B:\n";
        D_dense->print(outfile_dense);

        outfile_dense << "\nМатрица E_dense = транспонированная(A):\n";
        E_dense->print(outfile_dense);

        outfile_dense << "\nМатрица F_dense = A elemMult B (замена elemDiv для целых чисел):\n";
        F_dense->print(outfile_dense);

        outfile_dense << "\nМатрица G_dense = A + B:\n";
        G_dense.print(outfile_dense);

        outfile_dense << "\nМатрица H_dense = A - B:\n";
        H_dense.print(outfile_dense);

        outfile_dense << "\nМатрица K_dense = KroneckerProduct(A, B):\n";
        K_dense->print(outfile_dense);

        outfile_dense.close();

        std::cout << "Сгенерированные матрицы и результаты операций для MatrixDense сохранены в файл MatrixDense.txt\n";

        // Освобождаем память
        delete C_dense;
        delete D_dense;
        delete E_dense;
        delete F_dense;
        delete K_dense;


        std::cout << "\n=== MatrixDiagonal ===\n";

        // Создаем случайные диагональные матрицы D1 и D2 размером 10x10 с целыми числами
        MatrixDiagonal<int> D1(10);
        MatrixDiagonal<int> D2(10);

        for (unsigned i = 0; i < D1.rows(); ++i) {
            D1(i, i) = dis(gen);
            D2(i, i) = dis(gen);
        }

        // Операции
        Matrix<int>* C_diag = D1 * D2;
        Matrix<int>* D_diag = D1.elemMult(D2);
        Matrix<int>* E_diag = D1.transpose();
        Matrix<int>* F_diag = D1.elemMult(D2);  

        // Операции сложения и вычитания
        MatrixDiagonal<int> G_diag = D1;
        MatrixDiagonal<int> H_diag = D1;
        G_diag += D2;
        H_diag -= D2;

   
        std::ofstream outfile_diag("MatrixDiagonal.txt");
        if (!outfile_diag) {
            throw std::runtime_error("Не удалось открыть файл MatrixDiagonal.txt для записи.");
        }

        // Записываем матрицы и результаты операций в файл
        outfile_diag << "Матрица D1:\n";
        D1.print(outfile_diag);

        outfile_diag << "\nМатрица D2:\n";
        D2.print(outfile_diag);

        outfile_diag << "\nМатрица C_diag = D1 * D2:\n";
        C_diag->print(outfile_diag);

        outfile_diag << "\nМатрица D_diag = D1 почленное умножение D2:\n";
        D_diag->print(outfile_diag);

        outfile_diag << "\nМатрица E_diag = транспонированная(D1):\n";
        E_diag->print(outfile_diag);

        outfile_diag << "\nМатрица F_diag = D1 elemMult D2 (замена elemDiv для целых чисел):\n";
        F_diag->print(outfile_diag);

        outfile_diag << "\nМатрица G_diag = D1 + D2:\n";
        G_diag.print(outfile_diag);

        outfile_diag << "\nМатрица H_diag = D1 - D2:\n";
        H_diag.print(outfile_diag);

        outfile_diag.close();

        std::cout << "Сгенерированные матрицы и результаты операций для MatrixDiagonal сохранены в файл MatrixDiagonal.txt\n";

        // Освобождаем память
        delete C_diag;
        delete D_diag;
        delete E_diag;
        delete F_diag;


        std::cout << "\n=== MatrixBlock ===\n";

        // Создаем блоковую матрицу размером 10x10 с блоками 2x2
        MatrixBlock<int> B1(5, 5, 2, 2);  // 5x5 блоков, каждый размером 2x2

        // Заполняем блоки случайными числами
        for (unsigned i = 0; i < 5; ++i) {
            for (unsigned j = 0; j < 5; ++j) {
                std::shared_ptr<MatrixDense<int>> block = std::make_shared<MatrixDense<int>>(2, 2);
                for (unsigned m = 0; m < 2; ++m) {
                    for (unsigned n = 0; n < 2; ++n) {
                        block->operator()(m, n) = dis(gen);
                    }
                }
                B1.setBlock(i, j, block);
            }
        }


        MatrixBlock<int> B2 = B1;

        // Операции
        Matrix<int>* C_block = B1 * B2;
        Matrix<int>* D_block = B1.elemMult(B2);
        Matrix<int>* E_block = B1.transpose();
        Matrix<int>* F_block = B1.elemMult(B2);  

        // Операции сложения и вычитания
        MatrixBlock<int> G_block = B1;
        MatrixBlock<int> H_block = B1;
        G_block += B2;
        H_block -= B2;

        std::ofstream outfile_block("MatrixBlock.txt");
        if (!outfile_block) {
            throw std::runtime_error("Не удалось открыть файл MatrixBlock.txt для записи.");
        }

        // Записываем матрицы и результаты операций в файл
        outfile_block << "Матрица B1:\n";
        B1.print(outfile_block);

        outfile_block << "\nМатрица B2:\n";
        B2.print(outfile_block);

        outfile_block << "\nМатрица C_block = B1 * B2:\n";
        C_block->print(outfile_block);

        outfile_block << "\nМатрица D_block = B1 почленное умножение B2:\n";
        D_block->print(outfile_block);

        outfile_block << "\nМатрица E_block = транспонированная(B1):\n";
        E_block->print(outfile_block);

        outfile_block << "\nМатрица F_block = B1 elemMult B2 (замена elemDiv для целых чисел):\n";
        F_block->print(outfile_block);

        outfile_block << "\nМатрица G_block = B1 + B2:\n";
        G_block.print(outfile_block);

        outfile_block << "\nМатрица H_block = B1 - B2:\n";
        H_block.print(outfile_block);

        outfile_block.close();

        std::cout << "Сгенерированные матрицы и результаты операций для MatrixBlock сохранены в файл MatrixBlock.txt\n";

        // Освобождаем память
        delete C_block;
        delete D_block;
        delete E_block;
        delete F_block;

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}
