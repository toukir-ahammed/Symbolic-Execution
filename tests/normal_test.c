#include<stdio.h>

int main()
{
    int x, y, z;

    if ( x > 10 )
    {
        if ( x < 20 )
        {
            if ( y >= 10 )
            {
                z = z + 1;
            }

        }
        else
        {
            z = z - 1;
        }

    }
    else
    {
        if ( y > 5 )
        {
            z = x + 5;
        }
        else
        {
            if ( x == 2)
            {
                z = z + 5;
            }
            else
            {
                z--;
            }

        }
    }

    printf("%d\n", z);
    return 0;
}
