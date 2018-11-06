#include <stdio.h>

int main()
{
	int i = 6;
	int j = 10;

	printf("Start.\n");

	while ( j > 6 )
	{
		printf("i = %d\n", i);
		j--;
	}

	if ( j > 1 )
	{
		if ( i > 0 )
		{
			printf("i = %d\n", i);

			if ( j > 7 )
			{
				printf("here\n");
			}
		}
	}

	printf("Finished.\n");

	return 0;
}


