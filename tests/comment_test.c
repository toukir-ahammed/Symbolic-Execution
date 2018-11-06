#include <stdio.h> // header file
//a blank line
int main()//main start
{
    /* a simple program */
    int a, b;     // variable declaration
    scanf("%d%d", &a, &b); /* take input */

    /*This is a double
    line comment*/

    /*if ( a > 5 )
    {
//        if ( b ==  10)
//        {
//            printf("Test\n");
//        }
//        else
//        {
//             printf("Test\n");
//        }
    }
    else
    {
        if ( b <  10)
        {
            printf("Test\n");/*nothing
        }
        else
        {
             printf("Test\n");
        }
    }*/

    if ( a > 0 )
    {
        if ( b > 0 )
        {
            puts("Test");
        }
        else
        {
            puts("Test");
        }
    }
    return 0;   // return statement
}
