//Vector approach - rotate by 90 degrees
#include <iostream>
#include <riscv_vector.h>

void print(float *matrix, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            std::cout << matrix[i*n + j] << " ";
        }
        std::cout << std::endl;
    }
}

void rotate(float *src, float *dst, size_t n) 
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
    size_t n;
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

    rotate(&a[0][0], &result[0][0], n);

    std::cout << "\nOutput Matrix:" << std::endl;
    print(&result[0][0], n);

    return 0;
}
