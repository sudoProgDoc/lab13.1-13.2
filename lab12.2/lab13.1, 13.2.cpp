#include <iostream>
#include <cmath>
using namespace std;

int solver(int n, int m, float** A, float* X) {
    int i, j, k, v;
    bool w;
    float c, eps = 0.000001;

    for (i = 0; i < n - 1; i++)
    {
        v = i;
        for (j = i + 1; j < n; j++)
            if (abs(A[j][i]) > abs(A[v][i])) v = j;

        if (v != i) {
            for (j = 0; j <= n; j++) {
                c = A[i][j];
                A[i][j] = A[v][j];
                A[v][j] = c;
            }
        }

        // приведение к ступенчатому виду
        for (k = i + 1; k < n; k++) {
            c = A[k][i] / A[i][i];
            for (j = i; j <= n; j++)
                A[k][j] -= c * A[i][j];
        }
    }

    // запись промежуточного результата в файл
    FILE* fm = fopen("ou.txt", "at");
    for (i = 0; i < m; i++) {
        for (j = 0; j <= n; j++) fprintf(fm, "%8.3f", A[i][j]);
        fprintf(fm, "\n");
    }
    fclose(fm);

    // Обратный ход
    for (i = n - 1; i >= 0; i--) {
        if (abs(A[i][i]) < eps) {
            // Проверка на наличие свободных переменных
            w = true;
            for (j = 0; j < n; j++)
            {
                if (abs(A[i][j]) > eps)
                {
                    w = false;
                    break;
                }
            }
            if (w && abs(A[i][n]) > eps) return 0; // Нет решений
            continue; // Свободная переменная
        }
        for (k = 0; k < i; k++) {
            c = A[k][i] / A[i][i];
            A[k][n] -= c * A[i][n];
        }
    }

    // Решение
    for (i = 0; i < n; i++) {
        if (abs(A[i][i]) > eps) {
            X[i] = A[i][n] / A[i][i];
        }
        else {
            X[i] = NAN;
        }
    }

    return 1;
}

int main() {
    FILE* fl = fopen("test1.txt", "rt");

    float* solution, ** a, f;
    int n, m, i, j, g;

    fscanf(fl, "%d", &m);
    fscanf(fl, "%d", &n);
    a = new float* [m];
    solution = new float[n];
    for (i = 0; i < n; i++) solution[i] = 0;
    for (i = 0; i < m; i++)
    {
        a[i] = new float[n + 1];
        for (j = 0; j <= n; j++)
        {
            fscanf(fl, "%f", &f);
            a[i][j] = f;
        }
    }
    fclose(fl);

    g = solver(n, m, a, solution);

    if (g)
    {
        for (i = 0; i < n; i++)
            if (isnan(solution[i])) cout << "x[" << i << "] is a free variable." << endl;
            else printf("x[%d] = %8.3f\n", i + 1, solution[i]);
    }
    else printf("ERROR: No solutions\n");

    for (i = 0; i < m; i++) {
        delete[] a[i];
    }
    delete[] a;
    delete[] solution;
}
