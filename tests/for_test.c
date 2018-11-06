#include <stdio.h>

int main()
{
    int i;
    int j;

    for ( i = 0; i < 10; i++ )
    {
        for ( j = 20; j >= 0; j-- )
        {
            printf("%D %d\n", i+j, i-j);
        }
    }
    return 0;
}
