#include <iostream>
#include <riscv_vector.h>

using namespace std;

void vector_multiply(int **a, int **b, int **result, int n) {
    for (size_t i = 0; i < n; i++) {  
        for (size_t j = 0; j < n; j ++) {  
            size_t vl = __riscv_vsetvl_e32m2(n - j);  

            vint32m2_t sum = __riscv_vmv_v_x_i32m2(0, vl); 

            for (size_t k = 0; k < n; k++) {  
                vint32m2_t va = __riscv_vmv_v_x_i32m2(a[i][k], vl);  
                vint32m2_t vb = __riscv_vle32_v_i32m2(&b[k][j], vl);  
                sum = __riscv_vadd_vv_i32m2(sum, __riscv_vmul_vv_i32m2(va, vb, vl), vl);
            }
            __riscv_vse32_v_i32m2(&result[i][j], sum, vl); 
        }
    }
}

void naive_multiply(int **a, int **b, int **result, int n)
{
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
        {
            result[i][j] = 0;
            for(int k=0; k<n; k++)
            {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void print(int **matrix, size_t n) {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    int n = 8;
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
    }
    cout << "The elements of Matrix A:" << endl;
    print(a, n);

    cout << "The elements of Matrix B:" << endl;
    print(b, n);
    
    for(int i=0;i<100;i++){
        naive_multiply(a, b, result, n);    //warmup runs
    }

    for(int i=0;i<100;i++){
        vector_multiply(a, b, result, n);    //warmup runs
    }

    naive_multiply(a, b, result, n);

    vector_multiply(a, b, result, n);

    cout << "Output Matrix:" << endl;
    print(result, n);

    for (int i = 0; i < n; i++) {
        delete[] a[i];
        delete[] b[i];
        delete[] result[i];
    }
    delete[] a;
    delete[] b;
    delete[] result;

    return 0;
}
