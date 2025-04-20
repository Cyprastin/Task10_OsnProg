#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define FILE_A "Amatrix.txt"
#define FILE_B "Bmatrix.txt"
#define FILE_RES "Result.txt"
typedef struct {
    float** matrix;
    int rows;
    int columns;
} Matrix;

Matrix file_matrix(const char* FILENAME)
{
    Matrix result = { NULL, 0, 0 };
    FILE* file;

    if (fopen_s(&file, FILENAME, "r") != 0) {
        printf("Failed to open file\n");
        return result; // Возвращение пустой структуры в случае ошибки
    }

    int rows = 0;
    int columns = 0;
    int current_columns = 0;
    int num;
    // Подсчёт строк и столбцов
    while (fscanf_s(file, "%d", &num) == 1) {
        current_columns++;
        int ch = fgetc(file);
        if (ch == '\n' || ch == EOF) {
            rows++;
            if (current_columns > columns) {
                columns = current_columns;
            }
            current_columns = 0;
        }
    }
    if (current_columns > 0) {
        rows++;
        if (current_columns > columns) {
            columns = current_columns;
        }
    }
    if (rows == 0 || columns == 0) {
        printf("File is empty or contains no numbers\n");
        if (fclose(file) != 0)
            printf("\nfile closing error\n");
        return result; // Возвращаем пустую структуру
    }

    // Выделение памяти
    float** matrix = (float**)malloc(rows * sizeof(float*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (float*)malloc(columns * sizeof(float));
    }

    // Запись данных в матрицу
    rewind(file);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (fscanf_s(file, "%f", &matrix[i][j]) != 1) {
                matrix[i][j] = 0;
            }
        }
    }
    if (fclose(file) != 0)
        printf("\nfile closing error\n");
    // Запись в структуру
    result.matrix = matrix;
    result.rows = rows;
    result.columns = columns;
    return result;
}

Matrix rand_matrix(int n, int m)
{
    srand(time(NULL));
    Matrix result = { NULL, n, m };

    //Выделяем память под массив и заполняем
    result.matrix = (float**)malloc(result.rows * sizeof(float*));
    for (int i = 0; i < result.rows; i++)
    {
        result.matrix[i] = (float*)malloc(result.columns * sizeof(float));
        for (int j = 0; j < result.columns; j++)
        {
            result.matrix[i][j] = rand() % 100;
        }
    }
    return result;
}

Matrix sum_matrix(Matrix* A, Matrix* B, const char* OUT_FILENAME)
{
    //Проверка размеров
    if (A->rows != B->rows)
    {
        Matrix null = { NULL, 0, 0 };
        return null;
    }
    if (A->columns != B->columns)
    {
        Matrix null = { NULL, 0, 0 };
        return null;
    }
    //Выделение памяти под новую матрицу
    float** matrix = (float**)malloc(A->rows * sizeof(float*));
    for (int i = 0; i < A->rows; i++) 
    {
        matrix[i] = (float*)malloc(A->columns * sizeof(float));
    }

    //суммирование матриц
    for (int i = 0; i < A->rows; i++)
    {
        for (int j = 0; j < A->columns; j++)
        {
            matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
        }
    }

    //Если есть файл то записывает в него матрицу иначе возращает матрицу
    if (OUT_FILENAME == NULL)
    {
        Matrix result = { NULL, 0, 0 };
        result.matrix = matrix;
        result.rows = A->rows;
        result.columns = A->columns;
        return result;
    }
    else
    {
        FILE* file;
        if (fopen_s(&file, OUT_FILENAME, "w") != 0) {
            printf("Failed to open file\n");
            for (int i = 0; i < A->rows; i++) {
                free(matrix[i]);
            }
            free(matrix);
            Matrix null = { NULL, 0 , 0 };
            return null;
        }
        for (int i = 0; i < A->rows; i++)
        {
            for (int j = 0; j < A->columns; j++)
            {
                fprintf_s(file, "%f\t", matrix[i][j]);
            }
            fprintf_s(file, "\n" );
        }
        for (int i = 0; i < A->rows; i++) {
            free(matrix[i]);
        }
        free(matrix);
        if (fclose(file) != 0) {
            printf("\nfile closing error\n");
        }
        Matrix null = { NULL, 0, 0 };
        return null;
    }
}

Matrix multi_matrix(Matrix* A, Matrix* B, const char* OUT_FILENAME)
{
    //Условие для перемножение
    Matrix result = { NULL, 0, 0 };
    if (A->columns != B->rows) {
        return result;
    }
    //Выделение памяти под новую матрицу
    float** matrix = (float**)malloc(A->rows * sizeof(float*));
    for (int i = 0; i < A->rows; i++) {
        matrix[i] = (float*)malloc(B->columns * sizeof(float));
    }
    //Перемножение
    float temp = 0;
    for (int i = 0; i < A->rows; i++)
    {
        for (int j = 0; j < B->columns; j++)
        {
            for (int n = 0; n < A->columns; n++)
            {
                temp += A->matrix[i][n] * B->matrix[n][j];
            }
            matrix[i][j] = temp;
            temp = 0;
        }
    }
    //Если есть файл то записывает в него матрицу иначе возращает матрицу
    if (OUT_FILENAME == NULL)
    {

        result.matrix = matrix;
        result.rows = A->rows;
        result.columns = A->columns;
        return result;
    }
    else
    {
        FILE* file;
        if (fopen_s(&file, OUT_FILENAME, "w") != 0) {
            printf("Failed to open file\n");
            for (int i = 0; i < A->rows; i++) {
                free(matrix[i]);
            }
            free(matrix);
            Matrix null = { NULL, 0, 0 };
            return null;
        }
        for (int i = 0; i < A->rows; i++)
        {
            for (int j = 0; j < B->columns; j++)
            {
                fprintf_s(file, "%f\t", matrix[i][j]);
            }
            fprintf_s(file, "\n");
        }
        for (int i = 0; i < A->rows; i++) {
            free(matrix[i]);
        }
        free(matrix);
        Matrix null = { NULL, 0 , 0 };
        if (fclose(file) != 0) {
            printf("\nfile closing error\n");
        }
        return null;
    }
}

float det_matrix(Matrix* A) {
    // Проверка на квадратность матрицы
    if (A->rows != A->columns) {
        printf("\nThe matrix is not square\n");
        return NAN;
    }
    int n = A->rows;
    // Базовые случаи
    if (n == 1) {
        return A->matrix[0][0];
    }
    if (n == 2) {
        return A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
    }
    float det = 0;
    // Разложение по первой строке
    for (int col = 0; col < n; col++) {
        // Создаем подматрицу (n-1)x(n-1)
        Matrix submatrix;
        submatrix.rows = n - 1;
        submatrix.columns = n - 1;
        // Выделяем память под подматрицу
        submatrix.matrix = (float**)malloc(submatrix.rows * sizeof(float*));
        for (int i = 0; i < submatrix.rows; i++) {
            submatrix.matrix[i] = (float*)malloc(submatrix.columns * sizeof(float));
        }
        // Заполняем подматрицу
        for (int i = 1; i < n; i++) {
            int subcol = 0;
            for (int j = 0; j < n; j++) {
                if (j == col) continue;
                submatrix.matrix[i - 1][subcol++] = A->matrix[i][j];
            }
        }
        // Рекурсивный вызов и учет знака
        int sign = (col % 2 == 0) ? 1 : -1;
        det += sign * A->matrix[0][col] * det_matrix(&submatrix);
        // Освобождаем память
        for (int i = 0; i < submatrix.rows; i++) {
            free(submatrix.matrix[i]);
        }
        free(submatrix.matrix);
    }

    return det;
}

float minor_matrix(Matrix* A, int rows, int cols)
{
    if (A->rows != A->columns) {
        printf("\nThe matrix is not square\n");
        return NAN;
    }
    Matrix temp = { NULL, 0, 0 };
    temp.rows = A->rows - 1;
    temp.columns = A->rows - 1;
    // Выделяем память под подматрицу
    temp.matrix = (float**)malloc(temp.rows * sizeof(float*));
    for (int i = 0; i < temp.rows; i++) {
        temp.matrix[i] = (float*)malloc(temp.columns * sizeof(float));
    }
    // Заполняем подматрицу
    for (int i = 0, ti = 0; i < A->rows; i++) {
        if (i == rows) continue;  // Пропускаем строку rows

        for (int j = 0, tj = 0; j < A->columns; j++) {
            if (j == cols) continue;  // Пропускаем столбец cols
            #pragma warning(suppress : 6386)
            temp.matrix[ti][tj] = A->matrix[i][j];
            tj++;
        }
        ti++;
    }

    float det_minor = det_matrix(&temp);
    if ((rows + cols) % 2 != 0)
    {
        det_minor = det_minor * (-1) ;
    }
    for (int i = 0; i < temp.rows; i++) {
        free(temp.matrix[i]);
    }
    free(temp.matrix);
    return det_minor;
}

Matrix trans_matrix(Matrix* A, const char*OUT_FILENAME)
{
    Matrix result = { NULL, A->columns, A->rows };
    result.matrix = (float**)malloc(result.rows * sizeof(float*));
    for (int i = 0; i < result.rows; i++) {
        result.matrix[i] = (float*)malloc(result.columns * sizeof(float));
    }
    for (int i = 0; i < result.rows; i++)
    {
        for (int j = 0; j < result.columns; j++)
        {
            result.matrix[i][j] = A->matrix[j][i];
        }
    }
    //Если есть файл то записывает в него матрицу иначе возращает матрицу
    if (OUT_FILENAME == NULL)
    {
        return result;
    }
    else
    {
        FILE* file;
        if (fopen_s(&file, OUT_FILENAME, "w") != 0) {
            printf("Failed to open file\n");
            for (int i = 0; i < result.rows; i++) {
                free(result.matrix[i]);
            }
            free(result.matrix);
            Matrix null = { NULL, 0, 0 };
            return null;
        }
        for (int i = 0; i < result.rows; i++)
        {
            for (int j = 0; j < result.columns; j++)
            {
                fprintf_s(file, "%f\t", result.matrix[i][j]);
            }
            fprintf_s(file, "\n");
        }
        for (int i = 0; i < result.rows; i++) {
            free(result.matrix[i]);
        }
        free(result.matrix);
        if (fclose(file) != 0) {
            printf("\nfile closing error\n");
        }
        Matrix null = { NULL, 0, 0 };
        return null;
    }
}

Matrix inv_matrix(Matrix* A, const char* OUT_FILENAME)
{
    if (A->rows != A->columns) {
        printf("\nThe matrix is not square\n");
        Matrix null = { NULL, 0, 0 };
        return null;
    }
    float det = det_matrix(A);
    if (det == 0)
    {
        printf_s("\nThe inverse matrix does not exist\n");
        Matrix null = { NULL, 0, 0 };
        return null;
    }
    Matrix result = { NULL, A->rows, A->columns};
    result.matrix = (float**)malloc(result.rows * sizeof(float*));
    for (int i = 0; i < result.columns; i++)
    {
        result.matrix[i] = (float*)malloc(result.columns * sizeof(float));
    }
    for (int i = 0; i < result.rows; i++)
    {
        for (int j = 0; j < result.columns; j++)
        {
            #pragma warning(suppress : 6386)
            result.matrix[j][i] = minor_matrix(A, i, j) / det;
        }
    }
    //Если есть файл то записывает в него матрицу иначе возращает матрицу
    if (OUT_FILENAME == NULL)
    {
        return result;
    }
    else
    {
        FILE* file;
        if (fopen_s(&file, OUT_FILENAME, "w") != 0) {
            printf("Failed to open file\n");
            for (int i = 0; i < result.rows; i++) {
                free(result.matrix[i]);
            }
            free(result.matrix);
            Matrix null = { NULL, 0 ,0 };
            return null;
        }
        for (int i = 0; i < A->rows; i++)
        {
            for (int j = 0; j < A->columns; j++)
            {
                fprintf_s(file, "%f\t", result.matrix[i][j]);
            }
            fprintf_s(file, "\n");
        }
        for (int i = 0; i < result.rows; i++) {
            free(result.matrix[i]);
        }
        free(result.matrix);
        if (fclose(file) != 0) {
            printf("\nfile closing error\n");
        }
        Matrix null = { NULL, 0 ,0 };
        return null;
    }
}

Matrix pow_matrix(Matrix* A, int power, const char* OUT_FILENAME) {
    // Проверка на квадратную матрицу
    if (A->rows != A->columns) {
        printf_s("\nThe matrix is not square\n");
        Matrix null = { NULL, 0, 0 };
        return null;
    }
    if (power < 0)
    {
        printf_s("\nThe power is a positive number\n");
        Matrix null = { NULL, 0, 0 };
        return null;
    }
    int size = A->rows;

    // Создаем единичную матрицу для результата
    Matrix result = { NULL, size, size };
    result.matrix = (float**)malloc(size * sizeof(float*));
    for (int i = 0; i < size; i++) {
        result.matrix[i] = (float*)calloc(size, sizeof(float));
        result.matrix[i][i] = 1.0f;
    }

    // Создаем копию исходной матрицы для возведения в степень
    Matrix temp = { NULL, size, size };
    temp.matrix = (float**)malloc(size * sizeof(float*));
    for (int i = 0; i < size; i++) {
        temp.matrix[i] = (float*)malloc(size * sizeof(float));
        for (int j = 0; j < size; j++) {
            temp.matrix[i][j] = A->matrix[i][j];
        }
    }

    // Бинарное возведение в степень
    while (power > 0) {
        if (power % 2 == 1) {
            // Умножение result на temp
            Matrix temp_result = { NULL, size, size };
            temp_result.matrix = (float**)malloc(size * sizeof(float*));
            for (int i = 0; i < size; i++) {
                temp_result.matrix[i] = (float*)calloc(size, sizeof(float));
                for (int j = 0; j < size; j++) {
                    for (int k = 0; k < size; k++) {
                        temp_result.matrix[i][j] += result.matrix[i][k] * temp.matrix[k][j];
                    }
                }
            }

            // Освобождаем старый result
            for (int i = 0; i < size; i++) free(result.matrix[i]);
            free(result.matrix);

            // Заменяем на новый
            result = temp_result;
        }

        power /= 2;
        if (power > 0) {
            // Возводим temp в квадрат
            Matrix temp_squared = { NULL, size, size };
            temp_squared.matrix = (float**)malloc(size * sizeof(float*));
            for (int i = 0; i < size; i++) {
                temp_squared.matrix[i] = (float*)calloc(size, sizeof(float));
                for (int j = 0; j < size; j++) {
                    for (int k = 0; k < size; k++) {
                        temp_squared.matrix[i][j] += temp.matrix[i][k] * temp.matrix[k][j];
                    }
                }
            }

            // Освобождаем старый temp
            for (int i = 0; i < size; i++) free(temp.matrix[i]);
            free(temp.matrix);

            // Заменяем на новый
            temp = temp_squared;
        }
    }

    // Освобождаем temp
    for (int i = 0; i < size; i++) free(temp.matrix[i]);
    free(temp.matrix);

    // Запись в файл или возврат результата
    if (OUT_FILENAME != NULL) {
        FILE* file;
        if (fopen_s(&file, OUT_FILENAME, "w") != 0) {
            printf("Failed to open file\n");
            for (int i = 0; i < size; i++) free(result.matrix[i]);
            free(result.matrix);
            Matrix null = { NULL, 0, 0 };
            return null;
        }

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                fprintf(file, "%f\t", result.matrix[i][j]);
            }
            fprintf(file, "\n");
        }

        fclose(file);
        for (int i = 0; i < size; i++) free(result.matrix[i]);
        free(result.matrix);
        Matrix null = { NULL, 0, 0 };
        return null;
    }

    return result;
}

Matrix copy_matrix(Matrix* A)
{
    Matrix result = { NULL, A->rows, A->columns };
    result.matrix = (float**)malloc(result.rows * sizeof(float*));
    for (int i = 0; i < result.rows; i++)
    {
        result.matrix[i] = (float*)malloc(result.columns * sizeof(float));
    }
    result.matrix = A->matrix;
    return result;
}

void free_matrix(Matrix* A)
{
    if (A == NULL) return;

    if (A->matrix != NULL) {
        for (int i = 0; i < A->rows; i++) {
            if (A->matrix[i] != NULL) {
                free(A->matrix[i]);  
            }
        }
        free(A->matrix);
    }
    A->matrix = NULL;
    A->rows = 0;
    A->columns = 0;
    return;
}

void print_matrix(Matrix* A)
{
    printf_s("\n");
    for (int i = 0; i < A->rows; i++)
    {
        printf_s("\n\t%c", 179);
        for (int j = 0; j < A->columns; j++)
        {
            printf_s("%.2f\t", A->matrix[i][j]);
        }
        printf_s("\b\b\b\b%c", 179);
    }
    printf_s("\n");
}
