#include <stdio.h>
#include <C:\Users\Mvideo\source\repos\libMatrix\InitMatrix.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <crtdbg.h>

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void print_header(const char* title) {
    system("cls");
    printf("\n=== %s ===\n", title);
}

int get_int_input(const char* prompt, int min, int max) {
    int value;
    while (1) {
        printf("%s [%d-%d]: ", prompt, min, max);
        if (scanf_s("%d", &value) != 1 || value < min || value > max) {
            printf("Invalid input! Please try again.\n");
            clear_input_buffer();
        }
        else {
            clear_input_buffer();
            return value;
        }
    }
}

void matrix_source_menu(Matrix* m, const char* matrix_name) {
    print_header("Matrix Source Selection");
    printf("Select source for matrix %s:\n", matrix_name);
    printf("1. Load from default file\n");
    printf("2. Load from custom file\n");
    printf("3. Generate randomly\n");
    printf("4. Back to main menu\n");

    int choice = get_int_input("Your choice", 1, 4);

    if (choice == 4) return;

    switch (choice) {
    case 1: {
        const char* filename = (matrix_name[0] == 'A') ? FILE_A : FILE_B;
        Matrix loaded = file_matrix(filename);
        if (loaded.matrix) {
            free_matrix(m);
            *m = loaded;
            printf("Matrix %s loaded from %s\n", matrix_name, filename);
        }
        break;
    }
    case 2: {
        char filename[100];
        printf("Enter filename: ");
        scanf_s("%99s", filename, (unsigned)_countof(filename));
        Matrix loaded = file_matrix(filename);
        if (loaded.matrix) {
            free_matrix(m);
            *m = loaded;
            printf("Matrix %s loaded successfully!\n", matrix_name);
        }
        break;
    }
    case 3: {
        int rows = get_int_input("Enter number of rows", 1, 100);
        int cols = get_int_input("Enter number of columns", 1, 100);
        free_matrix(m);
        *m = rand_matrix(rows, cols);
        printf("Random matrix %s generated\n", matrix_name);
        break;
    }
    }
    _getch();
}

void matrix_operations_menu(Matrix* A, Matrix* B) {
    print_header("Matrix Operations");
    printf("1. Perform addition\n");
    printf("2. Perform multiplication\n");
    printf("3. Transpose matrix\n");
    printf("4. Calculate determinant\n");
    printf("5. Calculate inverse\n");
    printf("6. Raise to power\n");
    printf("7. Back to main menu\n");

    int choice = get_int_input("Select operation", 1, 7);
    if (choice == 7) return;

    switch (choice) {
    case 1: {
        Matrix result = sum_matrix(A, B, NULL);
        if (result.matrix) {
            print_header("Addition Result");
            print_matrix(&result);
            free_matrix(&result);
        }
        break;
    }
    case 2: {
        Matrix result = multi_matrix(A, B, NULL);
        if (result.matrix) {
            print_header("Multiplication Result");
            print_matrix(&result);
            free_matrix(&result);
        }
        break;
    }
    case 3: {
        print_header("Transpose Matrix");
        int target = get_int_input("Select matrix (1-A, 2-B)", 1, 2);
        Matrix* m = (target == 1) ? A : B;
        Matrix result = trans_matrix(m, NULL);
        if (result.matrix) {
            print_matrix(&result);
            free_matrix(&result);
        }
        break;
    }
    case 4: {
        print_header("Determinant Calculation");
        int target = get_int_input("Select matrix (1-A, 2-B)", 1, 2);
        Matrix* m = (target == 1) ? A : B;
        float det = det_matrix(m);
        if (!isnan(det)) printf("\nDeterminant: %.2f\n", det);
        break;
    }
    case 5: {
        print_header("Inverse Matrix");
        int target = get_int_input("Select matrix (1-A, 2-B)", 1, 2);
        Matrix* m = (target == 1) ? A : B;
        Matrix result = inv_matrix(m, NULL);
        if (result.matrix) {
            print_matrix(&result);
            free_matrix(&result);
        }
        break;
    }
    case 6: {
        print_header("Matrix Power");
        int target = get_int_input("Select matrix (1-A, 2-B)", 1, 2);
        int power = get_int_input("Enter power", 0, 10);
        Matrix* m = (target == 1) ? A : B;
        Matrix result = pow_matrix(m, power, NULL);
        if (result.matrix) {
            print_matrix(&result);
            free_matrix(&result);
        }
        break;
    }
    }
    _getch();
}

void preview_matrices(Matrix* A, Matrix* B) {
    print_header("Current Matrices");
    printf("Matrix A (%dx%d):\n", A->rows, A->columns);
    if (A->matrix) print_matrix(A);
    else printf("Not loaded\n");

    printf("\nMatrix B (%dx%d):\n", B->rows, B->columns);
    if (B->matrix) print_matrix(B);
    else printf("Not loaded\n");
    _getch();
}

int main() {
    Matrix A = { NULL, 0, 0 };
    Matrix B = { NULL, 0, 0 };

    while (1) {
        print_header("Matrix Calculator");
        printf("1. Select/Modify Matrix A\n");
        printf("2. Select/Modify Matrix B\n");
        printf("3. Preview Matrices\n");
        printf("4. Perform Operations\n");
        printf("5. Exit\n");

        int choice = get_int_input("Main menu choice", 1, 5);

        switch (choice) {
        case 1: matrix_source_menu(&A, "A"); break;
        case 2: matrix_source_menu(&B, "B"); break;
        case 3: preview_matrices(&A, &B); break;
        case 4: matrix_operations_menu(&A, &B); break;
        case 5:
            free_matrix(&A);
            free_matrix(&B);
            printf_s("leaks = %d\n", _CrtDumpMemoryLeaks());
            exit(0);
        }
    }
    return 0;
}