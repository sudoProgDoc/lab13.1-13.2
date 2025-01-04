#include <iostream>
using namespace std;

int solver(int n, int m, float** A, float* X) {
    int i, j, k, v;
    float c, eps = 0.000001;

    for (i = 0; i < n - 1; i++) {
        v = i;
        for (j = i + 1; j < n; j++)
            if (abs(A[j][i]) > abs(A[v][i])) v = j;
        if (v != i)
        {
            for (j = 0; j <= n; j++) {
                c = A[i][j];
                A[i][j] = A[v][j];
                A[v][j] = c;
            }
        }

        //приведение к ступенчатому виду
        for (k = i + 1; k < n; k++) {
            c = A[k][i] / A[i][i];
            for (j = i; j <= n; j++)
                A[k][j] -= c * A[i][j];
        }
    }

    FILE* fm = fopen("ou.txt", "at");
    for (i = 0; i < m; i++)
    {
        for (j = 0; j <= n; j++) fprintf(fm, "%8.3f", A[i][j]);
        fprintf(fm, "\n");
    }
    fclose(fm);

    //обратный ход
    for (i = n - 1; i >= 0; i--) {
        for (k = 0; k < i; k++) {
            c = A[k][i] / A[i][i];
            A[k][n] -= c * A[i][n];
        }
    }

    

    //решение
    for (i = 0; i < n; i++)
        X[i] = A[i][n] / A[i][i];
    if (isnan(X[0])) return 0;
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
    for (i = 0; i < m; i++) {
        a[i] = new float[n + 1];
        for (j = 0; j <= n; j++) {
            fscanf(fl, "%f", &f);
            a[i][j] = f;
        }
    }

    g = solver(n, m, a, solution);
    if (g) {
        for (i = 0; i < n; i++)
            printf("%8.3f", solution[i]);
        printf("\n");
    }
    else {
        printf("ERROR\n");
    }


    for (i = 0; i < m; i++) {
        delete[] a[i];
    }
    delete[] a;
    delete[] solution;

    fclose(fl);
}
