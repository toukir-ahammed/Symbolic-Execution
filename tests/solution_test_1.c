#include<stdio.h>
int main()
{
    int x;
    scanf("%d", &x);

    if ( x > 0 )
    {
        if ( x >= 10 )
        {
            if ( x > 100 )
            {
                if ( x >= 500 )
                {
                    if ( x > 1000 )
                    {
                        printf("%d is greater than 1000\n");
                    }
                }
            }
        }
    }
    return 0;
}
