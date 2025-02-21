#include <riscv_vector.h>
#include <iostream>
#include <cstddef>
#include <chrono>

using namespace std;

void vector_add(int **A, int **B, int **C, int n)
{
    size_t vl;
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j += vl)
        {
            vl = __riscv_vsetvl_e32m2(n - j); 

            vint32m2_t va = __riscv_vle32_v_i32m2(&A[i][j], vl);
            vint32m2_t vb = __riscv_vle32_v_i32m2(&B[i][j], vl);

            vint32m2_t vc = __riscv_vadd_vv_i32m2(va, vb, vl);

            __riscv_vse32_v_i32m2(&C[i][j], vc, vl);
        }
    }
}

void naive_add(int **A, int **B, int **C, int n)
{
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
        {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

void print(int **matrix,int n)
{
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

int main()
{
    int n = 16;
    int **a = new int*[n];
    int **b = new int*[n];
    int **result = new int*[n];

    for (int i = 0; i < n; i++) {
        a[i] = new int[n];
        b[i] = new int[n];
        result[i] = new int[n];
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j] = j + 1;
            b[i][j] = j + 1;
        }
        cout << endl;
    }

    cout << "The elements of Matrix A:" << endl;
    print(a, n);

    cout << "The elements of Matrix B:" << endl;
    print(b, n);

    for(int i=0;i<100;i++){
        naive_add(a, b, result, n);    //warmup runs
    }

    for(int i=0;i<100;i++){
        vector_add(a, b, result, n);    //warmup runs
    }

    naive_add(a, b, result, n);    
    cout << "Resultant Matrix using scalar:" << endl;
    print(result, n);

    vector_add(a, b, result, n);
    cout << "Resultant Matrix using vector:" << endl;
    print(result, n);
    return 0;
}
