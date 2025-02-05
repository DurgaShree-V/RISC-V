#include <riscv_vector.h>
#include <iostream>
#include <cstddef>

using namespace std;

void vector_add(const int A[2][10], const int B[2][10], int C[2][10], const int row, const int col)
{
    size_t vl;
    for (size_t i = 0; i < row; i++)
    {
        for (size_t j = 0; j < col; j += vl)
        {
            vl = __riscv_vsetvl_e32m2(col - j); 
            cout << "value of vl = " << vl << endl;

            vint32m2_t va = __riscv_vle32_v_i32m2(&A[i][j], vl);
            vint32m2_t vb = __riscv_vle32_v_i32m2(&B[i][j], vl);

            vint32m2_t vc = __riscv_vadd_vv_i32m2(va, vb, vl);

            __riscv_vse32_v_i32m2(&C[i][j], vc, vl);
        }
    }
}

int main()
{
    const int row = 2, col = 10;
    int A[row][col] = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}};
    int B[row][col] = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}};
    int C[row][col] = {0};

    vector_add(A, B, C, row, col);

    cout << "Resultant Matrix:" << endl;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            cout << C[i][j] << " "; 
        }
        cout << endl;
    }
    return 0;
}
