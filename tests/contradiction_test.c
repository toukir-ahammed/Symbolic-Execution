#include <stdio.h>
int main()
{
    int a , b;
    scanf("%d%d", &a, &b);

    if (  a > 100 )
    {
        if ( a == 100 )
        {
            printf("Impossible\n");
        }
        else
        {
            printf("Possible\n");
        }
    }
    else
    {
        if ( a <= 150 )
        {
            printf("Possible\n");
        }
        else
        {
            printf("Impossible\n");
        }

    }
    return 0;
}
