#include <iostream>
#include <riscv_vector.h>

void transpose(float *res, float *A, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            res[j * n + i] = A[i * n + j];
        }
    }
}

void reverse(float *res, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n / 2; j++) {
            float t = res[i * n + j];
            res[i * n + j] = res[i * n + (n - j - 1)];
            res[i * n + (n - j - 1)] = t;
        }
    }
}


void naive_rotate(float *a, float *result, int n) {
    transpose(result, a, n);
    reverse(result, n);
}

void print(float *matrix, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            std::cout << matrix[i*n + j] << " ";
        }
        std::cout << std::endl;
    }
}

void vector_rotate(float *src, float *dst, size_t n) 
{
    for (int i = n - 1,j= 0; i >= 0; i--,j++) {  
        
        size_t avl = n;
        float* row_src = src + i*n;
        float* row_dst = dst + j;

        while (avl > 0) {
            size_t vl = __riscv_vsetvl_e32m1(avl);
            vfloat32m1_t row = __riscv_vle32_v_f32m1(row_src, vl);
            __riscv_vsse32(row_dst, sizeof(float) * n, row, vl);
           
            avl -= vl;
            row_src += vl;
            row_dst += vl * n;
        }
    }
}

int main() {
    size_t n = 1000;
    float count = 1.0;
    
    std::cout << "Enter the value of n in nxn: ";
    std::cin >> n;

    float a[n][n], result[n][n];

    std::cout << "The elements of Matrix A:" << std::endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j] = count++;
        }
    }

    std::cout << "Original Matrix:" << std::endl;
    print(&a[0][0], n);

    for(int i=0; i<100; i++)
    {
        naive_rotate(&a[0][0], &result[0][0], n);   
    }

    for(int i=0; i<100; i++)
    {
        vector_rotate(&a[0][0], &result[0][0], n);   
    }

    naive_rotate(&a[0][0], &result[0][0], n);

    vector_rotate(&a[0][0], &result[0][0], n);

    std::cout << "\nOutput Matrix:" << std::endl;
    print(&result[0][0], n);

    return 0;
}
