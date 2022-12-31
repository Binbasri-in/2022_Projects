/*
examples to check scanf() function and how it works

except %c and %[^\n], scanf() ignores leading spaces for all other format specifiers

*/

#include <stdio.h>

int main(void)
{
    int i;
    float x;
    char s[80];
    char c;

    // give different combinations of spaces as input
    printf("Enter an integer, a float and a string: ");
    printf("\nscanf return: %d\n",scanf("%d %f %s", &i, &x, s));
    printf("i = %d, x = %f, s = %s\n", i, x, s);

    // give different combinations of spaces as input
    printf("Enter an integer, a float and a string: ");
    printf("\nscanf return: %d\n", scanf("%d%f%s", &i, &x, s));
    printf("i = %d, x = %f, s = %s\n", i, x, s);

    // give different combinations of spaces as input
    printf("Enter an integer, a float and a string: ");
    printf("\nscanf return: %d\n",scanf("%d %f%s", &i, &x, s));
    printf("i = %d, x = %f, s = %s\n", i, x, s);

    // give different combinations of spaces as input
    printf("Enter an integer, a float and a string: ");
    printf("\nscanf return: %d\n",scanf("%d%f %s", &i, &x, s));
    printf("i = %d, x = %f, s = %s\n", i, x, s);

    return 0;
}

/*
Output:
#### Try 1:

Enter an integer, a float and a string: 151 54.212 hello

scanf return: 3
i = 151, x = 54.212002, s = hello       
Enter an integer, a float and a string: 151 54.212 hello

scanf return: 3
i = 151, x = 54.212002, s = hello       
Enter an integer, a float and a string: 151 54.212 hello

scanf return: 3
i = 151, x = 54.212002, s = hello
Enter an integer, a float and a string: 151 54.212 hello

scanf return: 3
i = 151, x = 54.212002, s = hello

#### Try 2: (error in reading the for the second time because stdin still have characters but not a number)

Enter an integer, a float and a string: 155 132.545 hello mohammed

scanf return: 3
i = 155, x = 132.544998, s = hello
Enter an integer, a float and a string: 
scanf return: 0
i = 155, x = 132.544998, s = hello
Enter an integer, a float and a string:
scanf return: 0
i = 155, x = 132.544998, s = hello
Enter an integer, a float and a string: 
scanf return: 0
i = 155, x = 132.544998, s = hello

#### Try 3:

Enter an integer, a float and a string: 155 132.545 hello           

scanf return: 3
i = 155, x = 132.544998, s = hello
Enter an integer, a float and a string: 155    132.545   hello 

scanf return: 3
i = 155, x = 132.544998, s = hello
Enter an integer, a float and a string: 155    132.545hello    

scanf return: 3
i = 155, x = 132.544998, s = hello
Enter an integer, a float and a string: 155    132.545  hello 

scanf return: 3
i = 155, x = 132.544998, s = hello
*/