#include <stdio.h>

int main(void)
{
    int img[3][3];
    int k = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            img[i][j] = k;
            k++;
        }
    }
    int gx[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int gy[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    for (int i = 0; i < 9; i++)
    {
        printf("%i ", gx[i]);
    }
    
    printf("\n");
    
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%i", img[i][j]);
        }
    }
    
    printf("\n");
    
}
