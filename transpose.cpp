#include <iostream>
#include <riscv_vector.h>

void transpose(float dst[4][4], float src[4][4], size_t n) 
{
    for (size_t i = 0; i < n; ++i) { 
        
        size_t avl = n;
      
        float* row_src = &src[i][0];
       
        float* row_dst = &dst[0][i];

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

void print(float matrix[4][4], size_t n) {
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    size_t n = 4;
    float count=1.0;
    float a[4][4], result[4][4];

    std::cout << "The elements of Matrix A:" << std::endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j] = count++;
        }
    }

    std::cout << "Original Matrix:" << std::endl;
    print(a, n);

    transpose(result, a, n);

    std::cout << "\nTransposed Matrix:" << std::endl;
    print(result, n);

    return 0;
}
