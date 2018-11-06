#include <stdio.h>

int main()
{
    int i, j, k;
    scanf("%d%d%d", &i, &j, &k);

    while ( i > 10 )
    {
        while ( j < 5 )
        {
            while ( k == 0)
            {
                k = k/10;
            }
            j--;
        }
        i++;
    }
    return 0;
}
