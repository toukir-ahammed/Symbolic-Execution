#include <stdio.h>

int main(int argc, char *argv[])
{
    int x, y, z, a, b, c, p, q, r;
    scanf("%d%d%d%d%d%d%d%d%d", &x, &y, &z, &a, &b, &c, &p, &q, &r);

    if ( x < 45 )
    {
        if ( y >= 10 )
        {
            if ( z == 15 )
            {
                printf("Here\n");
            }
        }
    }

    if ( a != 0 )
    {
        if ( b <= 100 )
        {
            if ( c >= 25 )
            {
                printf("Here\n");
            }
        }
    }

    if ( p > 0 )
    {
        if ( q < 20 )
        {
            if ( r != 10 )
            {
                printf("Here\n");
            }
        }
    }
    return 0;
}
