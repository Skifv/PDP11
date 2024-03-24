#include <stdio.h>
#include <math.h>


#include <stdio.h>
#include <math.h>

long long binomialCoefficient(int n, int k) {
    if (k < 0 || k > n) {
        return 0; 
    }

    long long C[n + 1][k + 1];

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= i && j <= k; j++) {
            if (j == 0 || j == i) {
                C[i][j] = 1;
            } else {
                C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
            }
        }
    }

    return C[n][k];
}

float bernulli(int n, int k, float p)
{
    return (float)binomialCoefficient(n, k) * pow(p, k) * pow(1-p, n-k);
}

int main(void)
{
    int n = 98;
    float p = 1 / 9;
    for (int k = 0; k <= 98; k++)
    {
        printf("k: %d, p: %lf\n", k, bernulli(n, k, p));
    }

    return 0;
}