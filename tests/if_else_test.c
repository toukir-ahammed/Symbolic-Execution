#include <stdio.h>

int main()
{
    int x, y;
    scanf("%d%d", &x, &y);

    if ( x > 0 )
    {
        if ( y > 0 )
        {
            printf("Product is positive\n");
        }
        else
        {
            if ( y == 0 )
            {
                printf("Product is ZERO\n");
            }
            else
            {
                printf("Product is negative\n");
            }
        }
    }
    else
    {
        if ( x < 0 )
        {
            if ( y > 0 )
            {
                printf("Product is negative\n");
            }
            else
            {
                if ( y == 0 )
                {
                    printf("Product is ZERO\n");
                }
                else
                {
                    printf("Product is positive\n");
                }
            }

        }
        else
        {
            printf("Product is ZERO\n");
        }

    }


    return 0;
}
