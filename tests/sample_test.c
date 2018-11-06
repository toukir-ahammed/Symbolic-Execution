#include <stdio.h>
int main()
{
    int a;
    scanf("%d", &a);

    if ( a > 0 )
    {
        printf("%d is positive\n", a);
    }
    else
    {
        if ( a < 0 )
        {
            printf("%d is positive\n", a);
        }
        else
        {
            printf("It is ZERO\n");
        }
    }
    return 0;
}